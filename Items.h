#pragma once

#include "Model.h"
#include "Shader.h"
#include "Utils.h"
#include <map>
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameProperties.h"

class _ItemType_{
      public:
            std::map<int,string> names;
            const int HAND=0;
            const int GRASS_BLOCK=1;
            const int DIAMOND_PICKAXE=2;
            _ItemType_(){
                  names[HAND]=("");
                  names[GRASS_BLOCK]=("Grass Block");
                  names[DIAMOND_PICKAXE]=("Diamond Pickaxe");
            }
}ItemType;


class Item{

      public:
            Item(){}

      private:
            int id=ItemType.HAND;

};

std::map<int,Item> itemStore;