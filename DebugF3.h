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
#include "GUI.h"


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
      sst<<camera.getPosition(true).x;
      sst>>tmp;
      debugLine1+=tmp;

      //y
      debugLine1+="     y: ";
      sst.clear();
      sst<<camera.getPosition(true).y;
      sst>>tmp;
      debugLine1+=tmp;

      //z
      debugLine1+="    z: ";
      sst.clear();
      sst<<camera.getPosition(true).z;
      sst>>tmp;
      debugLine1+=tmp;

      //xb
      debugLine1+="     ( ACC x: ";
      sst.clear();
      sst<<camera.getPosition(false).x;
      sst>>tmp;
      debugLine1+=tmp;

      //yb
      debugLine1+="     y: ";
      sst.clear();
      sst<<camera.getPosition(false).y;
      sst>>tmp;
      debugLine1+=tmp;

      //zb
      debugLine1+="    z: ";
      sst.clear();
      sst<<camera.getPosition(false).z;
      sst>>tmp;
      debugLine1+=tmp;
      debugLine1+=" )";


      RenderText(textShader, debugLine1, 20.0f, screenHeight-60.0f, 0.40f, glm::vec3(1.0f, 1.0f, 1.0f));


      //line 2
      debugLine1="";
      tmp="";
      
   

      RenderText(textShader, debugLine1, 20.0f, screenHeight-80.0f, 0.40f, glm::vec3(1.0f, 1.0f, 1.0f));
}