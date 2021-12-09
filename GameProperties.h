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
#include "Utils.h"
#include "Blocks.h"
#include "IndependentModels.h"
#include "Text.h"
#include "GUI.h"


//game properties
bool gameMode=0;
int sel=0;
const float blockLength=0.5405f;
int sceneSize=30;
glm::vec3 spawnPos=glm::vec3(0.0f, 4.0f*blockLength, 0.0f);