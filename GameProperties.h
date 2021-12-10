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



//game properties
bool gameMode=0;
float blockScale=1.00f;
// const float blockLength=blockScale;
const float blockLength=0.5405f*blockScale;
const float playerHeight=2.0f*blockLength;
// const float playerHeight=3.66f*blockLength;
// const float blockLength=0.5405f;
int sceneSize=30;

//if chunkSize=128,then the pos range is [-64,63).
int chunkSize=64;

int targetingDist=16;
// glm::vec3 spawnPos=glm::vec3(0.0f, 4.0f*blockLength, 0.0f);
glm::vec3 spawnPos=glm::vec3(0.0f, 0.5f, 0.0f);

bool isFullScreen=true;
bool inGUI=false;
