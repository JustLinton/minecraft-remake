// Std. Includes
#include <string>

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


glm::mat4 unitMat(    // 单位矩阵
	glm::vec4(1, 0, 0, 0),
	glm::vec4(0, 1, 0, 0),
	glm::vec4(0, 0, 1, 0),
	glm::vec4(0, 0, 0, 1)
);

//game mode
bool gameMode=0;

// Properties
GLuint screenWidth = 1920, screenHeight = 1080;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int sceneSize=20;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Minecraft By Linton", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

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
    // Shader shader("shaders/blinn-phong-texture.vs", "shaders/blinn-phong-texture.frag");
    Shader shader("shaders/model_loading.vs", "shaders/model_loading.frag");

    // Load models
    Model pickaxeModel("models/mc_diamondpickaxe/Diamond-Pickaxe.obj");
    Model chestModel("models/mc_chest/chest.obj");
    Model grassBlockModel("models/mc_grassblock/Grass_Block.obj");
    Model horseModel("models/mc_horse/source/horse.fbx");
    Model torchModel("models/mc-torch/Torch.obj");
    Model doorModel("models/mc_door/Wooden-Door.obj");
    
    // Draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear the colorbuffer(MC天空色)
        glClearColor(175/256.0, 221/256.0, 254/256.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();   // <-- Don't forget this one!
        // Transformation matrices
        glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        GLint lightPosLoc    = glGetUniformLocation(shader.Program, "light.position");
        GLint viewPosLoc     = glGetUniformLocation(shader.Program, "viewPos");
        glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);
        // Set lights properties
        glm::vec3 lightColor(1.0f);

        GLint lightAmbientLoc = glGetUniformLocation(shader.Program, "light.ambient");
        GLint lightDiffuseLoc = glGetUniformLocation(shader.Program, "light.diffuse");
        GLint lightSpecularLoc = glGetUniformLocation(shader.Program, "light.specular");

        glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
        glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);// 让我们把这个光调暗一点，这样会看起来更自然
        glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

        glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"),   1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"),   1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(shader.Program, "material.specular"),  0.5f, 0.5f, 0.5f); // Specular doesn't have full effect on this object's material
        glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);


        // Draw the loaded model
        glm::mat4 model = unitMat;


        //草方块地板
        for(int i=-sceneSize/2;i<=sceneSize/2;i++)
            for(int j=-sceneSize/2;j<=sceneSize/2;j++){
                model=unitMat;
                model = glm::translate(model, glm::vec3(0.0f-0.5405*j, -1.65f, 3.0f-0.5405*i)); // Translate it down a bit so it's at the center of the scene
                model = glm::scale(model, glm::vec3(0.27f, 0.27f, 0.27f));	// It's a bit too big for our scene, so scale it down
                model = glm::rotate(model,glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                grassBlockModel.Draw(shader);
            }

        model = unitMat;
        model = glm::translate(model, glm::vec3(0.5f, -0.65f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model,glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model,glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        pickaxeModel.Draw(shader);


        model = unitMat;
        model = glm::translate(model, glm::vec3(0.5f, -0.95f, 1.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));	// It's a bit too big for our scene, so scale it down
        // model = glm::rotate(model,glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        // model = glm::rotate(model,glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        torchModel.Draw(shader);


        model=unitMat;
        model = glm::translate(model, glm::vec3(-0.5f, -0.85f, 0.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model,glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        chestModel.Draw(shader);

        model=unitMat;
        model = glm::translate(model, glm::vec3(-4.0f, -0.4f, 3.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.04f, 0.04f, 0.04f));	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model,glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        horseModel.Draw(shader);

        model=unitMat;
        model = glm::translate(model, glm::vec3(-0.5f, -0.43f, 7.0f)); // Translate it down a bit so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.31f, 0.31f, 0.31f));	// It's a bit too big for our scene, so scale it down
        model = glm::rotate(model,glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        doorModel.Draw(shader);

    
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
    switch (gameMode)
    {
    case 0:
        if(keys[GLFW_KEY_W])
            camera.ProcessKeyboard(FORWARD_0, deltaTime);
        if(keys[GLFW_KEY_S])
            camera.ProcessKeyboard(BACKWARD_0, deltaTime);
        if(keys[GLFW_KEY_A])
            camera.ProcessKeyboard(LEFT_0, deltaTime);
        if(keys[GLFW_KEY_D])
            camera.ProcessKeyboard(RIGHT_0, deltaTime);

        // if(keys[GLFW_KEY_SPACE])
        //     camera.ProcessKeyboard(UP, deltaTime);

        // if(keys[GLFW_KEY_LEFT_SHIFT])
        //     camera.ProcessKeyboard(DOWN, deltaTime);
        break;
    case 1:
         // Camera controls
        if(keys[GLFW_KEY_W])
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if(keys[GLFW_KEY_S])
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if(keys[GLFW_KEY_A])
            camera.ProcessKeyboard(LEFT, deltaTime);
        if(keys[GLFW_KEY_D])
            camera.ProcessKeyboard(RIGHT, deltaTime);

        if(keys[GLFW_KEY_SPACE])
            camera.ProcessKeyboard(UP, deltaTime);

        if(keys[GLFW_KEY_LEFT_SHIFT])
            camera.ProcessKeyboard(DOWN, deltaTime);
        break;
    
    default:
        break;
    }
   

}



// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;	
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

#pragma endregion