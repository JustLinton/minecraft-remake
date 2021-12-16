#pragma once
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

#include "IndependentModels.h"
#include "Camera.h"
#include "Model.h"
#include "Utils.h"
#include "Items.h"
#include "Blocks.h"

class FirstPerson{
      public:

            FirstPerson(Shader& shader_,float screenWidth_,float screenHeight_):modelUiShader(shader_),screenWidth(screenWidth_),screenHeight(screenHeight_){}

            void renderFirstPerson(int type=ItemType.HAND){
                  modelUiShader.Use();
                  projection = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
                  model=unitMat;
                  glUniformMatrix4fv(glGetUniformLocation(modelUiShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

                  if(type==ItemType.HAND)drawHand();
                  if(type==ItemType.GRASS_BLOCK)drawBlock(BlockType.GRASS_BLOCK);
                  if(type==ItemType.DIRT)drawBlock(BlockType.DIRT);
                  if(type==ItemType.OAK_PLANK)drawBlock(BlockType.OAK_PLANK);
                  if(type==ItemType.DIAMOND_PICKAXE)drawPickaxe();
            }

      private:
            glm::mat4 projection=unitMat;
            glm::mat4 view=unitMat;
            glm::mat4 model=unitMat;
            Shader& modelUiShader;
            float screenWidth;
            float screenHeight;

            void drawBlock(int type){
                  model = glm::translate(model, glm::vec3(blockLength*0.5, blockLength*(-0.55), blockLength*(-1))); 
                  model = glm::scale(model, glm::vec3(0.065f));
                  model = glm::rotate(model,glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  model = glm::rotate(model,glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                  model = glm::rotate(model,glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                  glUniformMatrix4fv(glGetUniformLocation(modelUiShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                  blockStore[type].getModel().Draw(modelUiShader);
                  
            }

            void drawPickaxe(){
                  model = glm::translate(model,glm::vec3(0.3f, -0.17f, -0.5f)); // Translate it down a bit so it's at the center of the scene
                  model = glm::scale(model,  glm::vec3(0.013f, 0.013f, 0.013f));	// It's a bit too big for our scene, so scale it down
                  model = glm::rotate(model,glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                  model = glm::rotate(model,glm::radians(-80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                  model = glm::rotate(model,glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
                  glUniformMatrix4fv(glGetUniformLocation(modelUiShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                  pickaxeModel.Draw(modelUiShader);
            }

            void drawHand(){}

};


