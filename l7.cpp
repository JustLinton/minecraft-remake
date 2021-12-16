// Std. Includes
#include <string>
#include <sstream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>
#include <ft2build.h>
#include FT_FREETYPE_H

//game imports
#include "World.h"
#include "GameProperties.h"
#include "Utils.h"
#include "Blocks.h"
#include "Items.h"
#include "IndependentModels.h"
#include "Text.h"
#include "HandSlotGUI.h"
#include "DebugF3.h"
#include "Player.h"
#include "FirstPerson.h"
// #include "ModelUI.h"

// Properties
GLuint screenWidth = 1920, screenHeight = 1080;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void Do_Movement();

// Camera
Camera camera(spawnPos);
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // isFullScreen=false;

    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    

    GLFWmonitor* pmonitor = isFullScreen?glfwGetPrimaryMonitor():nullptr;

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Minecraft By Linton", pmonitor, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

    // Setup some OpenGL options
    glEnable(GL_DEPTH_TEST);

        // Setup and compile our shaders
    Shader shader("shaders/blinn-phong-texture.vs", "shaders/blinn-phong-texture.frag");
    // Shader shader("shaders/model_loading.vs", "shaders/model_loading.frag");
    Shader textShader("shaders/text.vs", "shaders/text.frag");
    Shader guiShader("shaders/gui.vs", "shaders/gui.frag");
    Shader modelUiShader("shaders/ui_model.vs", "shaders/ui_model.frag");

    initTextRenderer(screenWidth,screenHeight,textShader);
    handSlotGUI.initGUIRenderer(screenWidth,screenHeight,guiShader);
    initBlocks();
    initIndependentModels();
    FirstPerson firstPerson(modelUiShader,screenWidth,screenHeight);
    player.setFirstPerson(&firstPerson);

    // Draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // float stamp=0.0f;


    //init map
    world.readMapFile();

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // stamp+=deltaTime;
        // std::cout<<stamp<<std::endl;

        // Check and call events
        glfwPollEvents();
        Do_Movement();
        camera.updateCamPosition(deltaTime);

        // Clear the colorbuffer(MC天空色)
        glClearColor(175/256.0, 221/256.0, 254/256.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();   // <-- Don't forget this one!

        // Transformation matrices
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        glm::vec3 lightPos(130.0f, 60.0f, 130.0f);
        GLint lightPosLoc    = glGetUniformLocation(shader.Program, "light.position");
        GLint viewPosLoc     = glGetUniformLocation(shader.Program, "viewPos");
        glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);
        // Set lights properties
        glm::vec3 lightColor(1.0f);

        GLint lightAmbientLoc = glGetUniformLocation(shader.Program, "light.ambient");
        GLint lightDiffuseLoc = glGetUniformLocation(shader.Program, "light.diffuse");
        GLint lightSpecularLoc = glGetUniformLocation(shader.Program, "light.specular");

        glUniform3f(lightAmbientLoc, 0.45f, 0.45f, 0.45f);
        // glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);// 让我们把这个光调暗一点，这样会看起来更自然
        glUniform3f(lightDiffuseLoc, 1.0f, 1.0f, 1.0f);
        glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

        glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"),   0.3f, 0.3f, 0.3f);
        // glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"),   1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(shader.Program, "material.specular"),  0.5f, 0.5f, 0.5f); // Specular doesn't have full effect on this object's material
        // glUniform3f(glGetUniformLocation(shader.Program, "material.specular"),  0.0f, 0.0f, 0.0f); // 决定了高光的颜色
        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);

        glUniform1f(glGetUniformLocation(shader.Program, "blockLength"), blockLength);

        // Draw the loaded model
        glm::mat4 model = unitMat;

        //可以开始渲染方块了

        // 草方块地板
        // for(int i=-sceneSize/2;i<=sceneSize/2;i++)
        //     for(int j=-sceneSize/2;j<=sceneSize/2;j++){
        //         blockStore[BlockType.GRASS_BLOCK].render(shader,glm::vec3(i,0,j));
        //     }

        //if chunkSize=128,then the pos range is [-64,63).

    BlockLocation pl=player.getBlockLocation();

        for(int x=-chunkSize/2;x<=chunkSize/2-1;x++)
            for(int y=-chunkSize/2;y<=chunkSize/2-1;y++)
                for(int z=-chunkSize/2;z<=chunkSize/2-1;z++){
                        // int type=world.chunkBlocks[getBlockRenderIndex(x)][getBlockRenderIndex(y)][getBlockRenderIndex(z)];
                        int type=world.getBlockTypeAt(BlockLocation(x,y,z));
                        if(type==0)continue;
                        //targeting shading
                        bool targeted = (player.getTargetBlockLocation().x==x&&player.getTargetBlockLocation().y==y&&player.getTargetBlockLocation().z==z&&player.getIsTargeting());    

                        vector<BlockLocation> nearBlockLocs;
                        if(x>=pl.x-4&&x<=pl.x+5&&y>=pl.y-4&&y<=pl.y+5&&z>=pl.z-4&&z<=pl.z+5){
                            for(int xx=x-2;xx<=x+2;xx++)
                                for(int yy=y-2;yy<=y+2;yy++)
                                    for(int zz=z-2;zz<=z+2;zz++){
                                            if(!world.checkIfBlockLocValid(BlockLocation(xx,yy,zz)))continue;
                                            int type=world.getBlockTypeAt(BlockLocation(xx,yy,zz));
                                            if(type==0)continue;
                                            nearBlockLocs.push_back(BlockLocation(xx,yy,zz));
                                        }
                        }

                        blockStore[type].render(shader,glm::vec3(x,y,z),targeted,nearBlockLocs);
                        // blockStore[type].render(shader,glm::vec3(x,y,z),targeted);

                }


        // blockStore[BlockType.GRASS_BLOCK].render(shader,glm::vec3(0,2,1),false);
        // blockStore[BlockType.GRASS_BLOCK].render(shader,glm::vec3(0,0,-1));
        // blockStore[BlockType.GRASS_BLOCK].render(shader,glm::vec3(0,0,-3));
        // blockStore[BlockType.GRASS_BLOCK].render(shader,glm::vec3(0,0,-9));


        model = unitMat;
        model = glm::translate(model, glm::vec3(0.5f, 0.85f, 1.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// It's a bit too big for our scene, so scale it down
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        torchModel.Draw(shader);


        model=unitMat;
        model = glm::translate(model, glm::vec3(-0.5f, -0.85f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model,glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        chestModel.Draw(shader);

        // model=unitMat;
        // model = glm::translate(model, glm::vec3(-4.0f, -0.4f, 3.0f)); // Translate it down a bit so it's at the center of the scene
        // model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));	// It's a bit too big for our scene, so scale it down
        // model = glm::rotate(model,glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        // horseModel.Draw(shader);

        model=unitMat;
        model = glm::translate(model, glm::vec3(-0.5f, -0.43f, 7.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.31f, 0.31f, 0.31f));	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model,glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        doorModel.Draw(shader);

        // model = unitMat;
        // model = glm::translate(model, pos); // Translate it down a bit so it's at the center of the scene
        // model = glm::scale(model, sca);	// It's a bit too big for our scene, so scale it down
        // model = glm::rotate(model,glm::radians(rad), rotAxis);
        // glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        // pickaxeModel.Draw(shader);


        // model=unitMat;
        // model = glm::translate(model, glm::vec3(blockLength*0, blockLength*(-0.5), blockLength*(-1))); 

        // model = glm::scale(model, glm::vec3(0.15f*blockScale));
        //     // model = glm::scale(model, glm::vec3(0.27f, 0.27f, 0.27f));
        //     // model = glm::scale(model, glm::vec3(blockScale,blockScale,blockScale));
        // model = glm::rotate(model,glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        // blockStore[BlockType.GRASS_BLOCK].getModel().Draw(shader);

        //=================
        //====gui and text======
        //=================
        RenderText(textShader, "+", screenWidth/2.0f-10.0f, screenHeight/2.0f+0.0f, 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
        RenderText(textShader, "Minecraft    0.2.1", 20.0f, screenHeight-35.0f, 0.45f, glm::vec3(1.0f, 1.0f, 1.0f));
        
        renderDebug(textShader,screenWidth,screenHeight,camera);

        // RenderGUI(guiShader, "img/itembarsel.png" ,screenWidth/2.0f-182.0f*1.75f, 80.0f, 24.0f,24.0f,3.5f, 0.25f);
        // RenderGUI(guiShader ,screenWidth/2.0f-182.0f*1.75f, 30.0f, 182.0f,22.0f,3.5f,0.75f);
        handSlotGUI.RenderGUI(guiShader,screenWidth,screenHeight);

        player.renderFirstPerson();
        

        // modelUiShader.Use();
        // projection = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        // view = camera.GetViewMatrix();
        // glUniformMatrix4fv(glGetUniformLocation(modelUiShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // glUniformMatrix4fv(glGetUniformLocation(modelUiShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // model = unitMat;
        // model = glm::translate(model,glm::vec3(0.3f, -0.1f, -0.5f)); // Translate it down a bit so it's at the center of the scene
        // model = glm::scale(model,  glm::vec3(0.013f, 0.013f, 0.013f));	// It's a bit too big for our scene, so scale it down
        // model = glm::rotate(model,glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        // model = glm::rotate(model,glm::radians(-80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::rotate(model,glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // glUniformMatrix4fv(glGetUniformLocation(modelUiShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        // pickaxeModel.Draw(modelUiShader);


        //用于定位，正确绘制第一人称
        //   for(int x=-chunkSize/2;x<=chunkSize/2-1;x++)
        //     for(int y=-chunkSize/2;y<=chunkSize/2-1;y++)
        //         for(int z=-chunkSize/2;z<=chunkSize/2-1;z++){
        //                 int type=mapManager.chunkBlocks[getBlockRenderIndex(x)][getBlockRenderIndex(y)][getBlockRenderIndex(z)];
        //                 if(type==0)continue;
        //                 //targeting shading
        //                 if(player.target.x==x&&player.target.y==y&&player.target.z==z&&player.isTargeting)
        //                     blockStore[type].render(modelUiShader,glm::vec3(x,y,z),true);
        //                 else
        //                     blockStore[type].render(modelUiShader,glm::vec3(x,y,z),false);
        //         }


     

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

#pragma region "User input"


// Moves/alters the camera positions based on user input
void Do_Movement()
{   
    bool isPressingKey=(keys[GLFW_KEY_W]||keys[GLFW_KEY_S]||keys[GLFW_KEY_A]||keys[GLFW_KEY_D]);

    camera.ProcessKeyboard(FORWARD, deltaTime,keys[GLFW_KEY_W],gameMode,isPressingKey);
    camera.ProcessKeyboard(BACKWARD, deltaTime,keys[GLFW_KEY_S],gameMode,isPressingKey);
    camera.ProcessKeyboard(LEFT, deltaTime,keys[GLFW_KEY_A],gameMode,isPressingKey);
    camera.ProcessKeyboard(RIGHT, deltaTime,keys[GLFW_KEY_D],gameMode,isPressingKey);
    camera.ProcessKeyboard(JUMP, deltaTime,keys[GLFW_KEY_SPACE],gameMode,isPressingKey);  
    camera.ProcessKeyboard(SNEAK, deltaTime,keys[GLFW_KEY_LEFT_SHIFT],gameMode,isPressingKey); 
    camera.ProcessKeyboard(SPRINT, deltaTime,keys[GLFW_KEY_LEFT_CONTROL],gameMode,isPressingKey); 
    camera.ProcessKeyboard(TEST, deltaTime,keys[GLFW_KEY_0],gameMode,isPressingKey); 
}


void quitGame(GLFWwindow* window){
    if(world.saveMapFile())
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        quitGame(window);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;	
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{   
	if (action == GLFW_PRESS) switch(button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
                if(inGUI){
                    std::cout<<lastX<<' '<<lastY<<'\n';
                }else
				    camera.ProcessMouseClick(ATTACK, deltaTime,true,gameMode);    
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
                if(inGUI){

                }else
                camera.ProcessMouseClick(USE, deltaTime,true,gameMode); 
				break;
			default:
				return;
			}
	return;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(inGUI){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        lastX = xpos;
        lastY = ypos;
        return;

    }else{
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if(firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos; 
        
        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    
}	

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // camera.ProcessMouseScroll(yoffset);
    if(yoffset<0){
        if(player.getHandSlot()+1>8)player.setHandSlot(8);
        else player.setHandSlot(player.getHandSlot()+1);
    } else{
        if(player.getHandSlot()-1<0)player.setHandSlot(0);
        else player.setHandSlot(player.getHandSlot()-1);
    }
}

#pragma endregion