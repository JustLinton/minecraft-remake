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
                  else if(sel==3)
                        firstPerson->renderFirstPerson(ItemType.DIRT);
                  else if(sel==4)
                        firstPerson->renderFirstPerson(ItemType.OAK_PLANK);
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
             //Rawosition????????????????????????????????????????????????blocklength??????????????????????????????????????????
            glm::vec3 rawPosition;
            //OtherVelocity: ??????????????????????????????????????????????????????????????????add??????
            Velocity OtherVelocity;
            //ObjectiveVelocity: ????????????????????????????????????????????????????????????????????????????????????
            Velocity ObjectiveVelocity;
            bool isSneaking;
            bool isFlying;
            bool isSprinting;
            // Eular Angles
            GLfloat Yaw;
            GLfloat Pitch;
            // Camera options
            GLfloat MovementSpeed;

            BlockLocation target;
            //0:x+ 1:x- 2:z+ 3:z- 4:y+ 5:y- 
            int targetFace;
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
                        if(isTargeting){
                              BlockLocation faceOn=BlockLocation(0,1,0);
                              switch (targetFace)
                              {
                                    case 0:
                                          faceOn=BlockLocation(1,0,0);
                                          break;

                                    case 1:
                                          faceOn=BlockLocation(-1,0,0);
                                          break;
                                    
                                    case 2:
                                          faceOn=BlockLocation(0,0,1);
                                          break;

                                    case 3:
                                          faceOn=BlockLocation(0,0,-1);
                                          break;

                                    case 4:
                                          faceOn=BlockLocation(0,1,0);
                                          break;

                                    case 5:
                                          faceOn=BlockLocation(0,-1,0);
                                          break;

                                    default:
                                          break;
                              }
                              
                              BlockLocation placeLoc=target+faceOn;
                              BlockLocation playerConfilct1=getBlockLocation()+BlockLocation(0,1,0);
                              BlockLocation playerConfilct2=getBlockLocation()+BlockLocation(0,2,0);

                              if(world.checkIfBlockLocValid(placeLoc)){
                                    if(world.getBlockTypeAt(placeLoc)==0&&playerConfilct1!=placeLoc&&playerConfilct2!=placeLoc){
                                          if(handSlot==2) world.setBlock(target+faceOn,1);
                                          if(handSlot==3) world.setBlock(target+faceOn,2);
                                          if(handSlot==4) world.setBlock(target+faceOn,3);
                                         
                                    }
                                    
                              }
                              
                        }
                              
                  }
            }

            //??????Camera?????????????????????????????????????????????????????????friend?????????
            friend class Camera;
};

Player player;