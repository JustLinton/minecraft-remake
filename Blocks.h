#pragma once

#include "Model.h"
#include "Shader.h"
#include "Utils.h"
#include <map>
#include <GL/glew.h>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameProperties.h"
#include "Types.h"

class _BlockType_{
      public:
            std::map<int,string> names;
            const int AIR=0;
            const int GRASS_BLOCK=1;
            const int DIRT=2;
            const int OAK_PLANK=3;
            _BlockType_(){
                  names[AIR]=("Minecraft:AIR");
                  names[GRASS_BLOCK]=("Minecraft:GRASS_BLOCK");
                  names[DIRT]=("Minecraft:DIRT");
                  names[OAK_PLANK]=("Minecraft:OAK_PLANK");
            }
}BlockType;


class Block{

      public:
            Model getModel(){return blockModel;}

            Block(){}

            Block(int id_,string path):id(id_),length(blockLength){
                  blockModel=Model(path);
            }

            void render(Shader shader,glm::vec3 pos,bool targeted,const vector<BlockLocation>& nearLocs){
            // void render(Shader shader,glm::vec3 pos,bool targeted){

                  if(id==0)return;

                  //切记，一定要在view、projection、shader、lightposition等都定义完毕后，再来调用该函数！
                  glm::mat4 model=unitMat;

                  model = glm::translate(model, glm::vec3(length*pos.x, length*(pos.y), length*pos.z)); 

                  model = glm::scale(model, glm::vec3(0.27f*blockScale, 0.27f*blockScale, 0.27f*blockScale));
                  // model = glm::scale(model, glm::vec3(0.27f, 0.27f, 0.27f));
                  // model = glm::scale(model, glm::vec3(blockScale,blockScale,blockScale));
                  model = glm::rotate(model,glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                  glUniform1i(glGetUniformLocation(shader.Program, "targeted"), targeted?1:0);
                  glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

                  nearBlockIndex=0;
                  for(BlockLocation nl:nearLocs){
                        tmpStr=nearBlocksPosStr;
                        tmpStrSst="";
                        tmpStr+="[";
                        sst.clear();
                        sst<<nearBlockIndex;
                        sst>>tmpStrSst;
                        tmpStr+=tmpStrSst;
                        tmpStr+="]";
                        glUniform3f(glGetUniformLocation(shader.Program, tmpStr.c_str()), nl.x,nl.y,nl.z);
                        ++nearBlockIndex;    
                  }
                  glUniform1i(glGetUniformLocation(shader.Program, "nearBlocksMaxIndex"), nearBlockIndex);

                  blockModel.Draw(shader);
                  glUniform1i(glGetUniformLocation(shader.Program, "targeted"), 0);
            }

      private:
            int id=BlockType.AIR;
            float length=blockLength;
            Model blockModel;

            std::stringstream sst;
            string nearBlocksPosStr="nearBlocksPos";
            string tmpStr;
            string tmpStrSst;
            int nearBlockIndex=0;

};

std::map<int,Block> blockStore;


void initBlocks(){
      //blocksRegister
      blockStore[BlockType.AIR]=Block();
      blockStore[BlockType.GRASS_BLOCK]=Block(BlockType.GRASS_BLOCK,"models/mc_grassblock/Grass_Block.obj");
      blockStore[BlockType.DIRT]=Block(BlockType.DIRT,"models/blocks/block_dirt/Grass_Block.obj");
      blockStore[BlockType.OAK_PLANK]=Block(BlockType.OAK_PLANK,"models/blocks/block_oak_plank/Grass_Block.obj");

}