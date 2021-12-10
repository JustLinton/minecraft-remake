#pragma once
#include "Types.h"

// Std. Includes
#include <vector>
#include <map>
#include <algorithm>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameProperties.h"
#include "FirstPerson.h"
#include "World.h"

class Player
{
      public:

            Player(){
                  this->rawPosition = spawnPos;
                  this->Yaw = -90.0f;
                  this->Pitch = 0.0f;
                  this->MovementSpeed=2.7f*blockScale;
                  target=glm::vec3(0.0f);
                  isTargeting=false;
                  isOnTheGround=false;
                  handSlot=0;
            }

            void setFirstPerson(FirstPerson* fp){
                  firstPerson=fp;
            }

            void renderFirstPerson(){
                  int sel=getHandSlot();
                  if(sel==1)
                        firstPerson->renderFirstPerson(ItemType.DIAMOND_PICKAXE);
                  else if(sel==2)
                        firstPerson->renderFirstPerson(ItemType.GRASS_BLOCK);
                  else  firstPerson->renderFirstPerson();
            }


            void setLocation(Location loc){
                  this->rawPosition=loc*V3(blockLength);
            }

            Location getLocation(){
                  return (this->rawPosition)/V3(blockLength);
            }

            BlockLocation getBlockLocation(){
                  // glm::vec3 res = player.rawPosition/blockLength;
                  Location res = getLocation();
                  // res.z/=0.5405f;
                  res.x=castToBlockInt(res.x);
                  res.y=castToBlockInt(res.y);
                  res.z=castToBlockInt(res.z);

                  return BlockLocation(res.x,res.y,res.z);
            }

            void setBlockLocation(BlockLocation loc){
                  setLocation(Location(loc.x,loc.y,loc.z));
            }

            void giveVelocity(Velocity velo){
                  OtherVelocity+=velo;
            }

            void setVelocity(Velocity velo){
                  OtherVelocity=velo;
            }

            Velocity getVelocity(){
                  return OtherVelocity;
            }

            glm::vec3 getRawLocation(){
                  return rawPosition;
            }

            void setRawLocation(glm::vec3 loc){
                  rawPosition=loc;
            }

            BlockLocation getTargetBlockLocation(){
                  return target;
            }

            bool getIsTargeting(){
                  return isTargeting;
            }

            bool getIsFlying(){
                  return isFlying;
            }

            bool getIsSprinting(){
                  return isSprinting;
            }

            void setHandSlot(int slot){
                  handSlot=slot;
            }

            int getHandSlot(){
                  return handSlot;
            }


      private:
             //Rawosition是世界坐标中的原始位置。需要根据blocklength进行放缩才能得到正确的坐标。
            glm::vec3 rawPosition;
            //OtherVelocity: 通常意义上说的“速度”。包括弹力、重力、以及add的。
            Velocity OtherVelocity;
            //ObjectiveVelocity: 因玩家主动前后左右移动产生的速度，用于在其速度上进行叠加
            Velocity ObjectiveVelocity;
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

            int handSlot;
            FirstPerson* firstPerson;

            int castToBlockInt(float a){
                  int flr=std::floor(a);
                  if(a>flr+0.5)return flr+1;
                  return flr;
            }

            void processClick(bool left){
                  if(left){
                        if(isTargeting)
                              world.setBlock(target,0);
                  }else{
                        if(isTargeting)
                              world.setBlock(target+glm::vec3(0.0f,1.0f,0.0f),1);
                  }
            }

            //因为Camera负责接收键盘信息，并处理玩家状态，所以friend一下。
            friend class Camera;
};

Player player;