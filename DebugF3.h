#pragma once

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
#include "GameProperties.h"
#include "Utils.h"
#include "Blocks.h"
#include "IndependentModels.h"
#include "Text.h"
#include "HandSlotGUI.h"
#include "Player.h"


void renderDebug(Shader& textShader,float screenWidth,float screenHeight,Camera& camera){
      stringstream sst;
      string debugLine1="";
      string tmp;

      //line 1

      debugLine1="";
      tmp="";
      
      //x
      debugLine1+="x: ";
      sst.clear();
      sst<<player.getBlockLocation().x;
      sst>>tmp;
      debugLine1+=tmp;

      //y
      debugLine1+="     y: ";
      sst.clear();
      sst<<player.getBlockLocation().y;
      sst>>tmp;
      debugLine1+=tmp;

      //z
      debugLine1+="    z: ";
      sst.clear();
      sst<<player.getBlockLocation().z;
      sst>>tmp;
      debugLine1+=tmp;

      //xb
      debugLine1+="     ( ACC x: ";
      sst.clear();
      sst<<player.getLocation().x;
      sst>>tmp;
      debugLine1+=tmp;

      //yb
      debugLine1+="     y: ";
      sst.clear();
      sst<<player.getLocation().y;
      sst>>tmp;
      debugLine1+=tmp;

      //zb
      debugLine1+="    z: ";
      sst.clear();
      sst<<player.getLocation().z;
      sst>>tmp;
      debugLine1+=tmp;
      debugLine1+=" )";


      RenderText(textShader, debugLine1, 20.0f, screenHeight-60.0f, 0.40f, glm::vec3(1.0f, 1.0f, 1.0f));


      //line 2
      debugLine1="";
      tmp="";


      debugLine1+="target:    ";

      BlockLocation target=player.getTargetBlockLocation();
      bool isTargeting=player.getIsTargeting();

      if(isTargeting){
            //x
            sst.clear();
            sst<<target.x;
            sst>>tmp;
            debugLine1+=tmp;

            //x
            debugLine1+=" , ";
            sst.clear();
            sst<<target.y;
            sst>>tmp;
            debugLine1+=tmp;
            
            //x
            debugLine1+=" , ";
            sst.clear();
            sst<<target.z;
            sst>>tmp;
            debugLine1+=tmp;

            debugLine1+="     ";
            debugLine1+=BlockType.names[1];
            
      }else{
             //x
            debugLine1+=BlockType.names[0];
      }

      RenderText(textShader, debugLine1, 20.0f, screenHeight-86.0f, 0.40f, glm::vec3(1.0f, 1.0f, 1.0f));
}