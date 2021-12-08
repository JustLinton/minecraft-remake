#pragma once

#include "Model.h"
#include "Shader.h"
#include "Utils.h"
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class _BlockType_{
      public:
            _BlockType_(){}
            const int AIR=0;
            const int GRASS_BLOCK=1;
}BlockType;

class Block{

      public:
            Model getModel(){return blockModel;}

            Block(){}

            Block(int id_,char* path,float length_=0.5405f):id(id_),length(length_){
                  blockModel=Model(path);
            }

            void render(Shader shader,BlockPosition pos){
                  //切记，一定要在view、projection、shader、lightposition等都定义完毕后，再来调用该函数！
                  glm::mat4 model=unitMat;

                  model = glm::translate(model, glm::vec3(length*pos.x, length*pos.y, length*pos.z)); 

                  model = glm::scale(model, glm::vec3(0.27f, 0.27f, 0.27f));
                  model = glm::rotate(model,glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

                  blockModel.Draw(shader);
            }

      private:
            int id=BlockType.AIR;
            float length=0.5405f;
            Model blockModel;

};

std::map<int,Block> blockStore;


void initBlocks(){
      //blocksRegister
      blockStore[BlockType.AIR]=Block();
      blockStore[BlockType.GRASS_BLOCK]=Block(BlockType.GRASS_BLOCK,"models/mc_grassblock/Grass_Block.obj");

}