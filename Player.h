#pragma once

// Std. Includes
#include <vector>
#include <map>
#include <algorithm>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameProperties.h"

class Player
{
      public:
      //Rawosition是世界坐标中的原始位置。需要根据blocklength进行放缩才能得到正确的坐标。
      glm::vec3 rawPosition;
      glm::vec3 Velocity;
      glm::vec3 OtherVelocity;
      bool isSneaking;
      bool isFlying;
      bool isSprinting;
      // Eular Angles
      GLfloat Yaw;
      GLfloat Pitch;
      // Camera options
      GLfloat MovementSpeed;

      glm::vec3 target;
      bool isTargeting;
      bool isOnTheGround;

      Player(){
            this->rawPosition = spawnPos;
            this->Yaw = -90.0f;
            this->Pitch = 0.0f;
            this->MovementSpeed=2.7f*blockScale;
            target=glm::vec3(0.0f);
            isTargeting=false;
            isOnTheGround=false;
      }
};

Player player;