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
#include "Player.h"
#include "World.h"


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    //creative
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP,
    SNEAK,
    SPRINT,
    ATTACK,
    USE,
    TEST
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat STANDARDSPEED      =  3.0f*blockScale;
const GLfloat SENSITIVTY =  0.17f;
const GLfloat ZOOM       =  45.0f;
const GLfloat SNEAKINGY=-0.34f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 FrontWithoutY;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // glm::vec3 Velocity;
    // glm::vec3 OtherVelocity;
    std::map<char,float> pressTimeAcc;
    std::map<char,float> releaseTimeAcc;
    // bool isSneaking;
    // bool isFlying;
    // bool isSprinting;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    // GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
        initPhysics();
        initOperation();
    }
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
        initPhysics();
        initOperation();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

  
    void initOperation(){
        player.isFlying=false;
        player.isSneaking=false;
        player.isSprinting=false;
    }

    void initTiming(char c){
        pressTimeAcc[c]=0.0f;
        releaseTimeAcc[c]=0.0f;
    }

    void initPhysics(){
        initTiming('w');
        initTiming('s');
        initTiming('a');
        initTiming('d');
        //'.' is space.
        initTiming('.');
        //',' is lshift.
        initTiming(',');
        //'`' is lshift.
        initTiming('`');
        //'=' is the time of sprint.
        initTiming('=');
        player.ObjectiveVelocity=glm::vec3(0.0f);
        player.OtherVelocity=glm::vec3(0.0f);

        player.ObjectiveVelocity=glm::vec3(0.0f);
        player.OtherVelocity=glm::vec3(0.0f);
    }


    //0w 1s  2a  3d  4space 5shift
    float curFlyX[6]={0.0f};
    bool isPressingWSAD=false;

    float sprintZoomStartFunction(float x,GLfloat deltaTime){
        if(x<=1.0f)return 45.2f+1.0f*x*x*0.5f;
        return 45.7f;
    }

    float sprintZoomEndFunction(float x,GLfloat deltaTime){
        return std::max(45.2f,45.2f+(1.0f-6.0f*x)*0.5f);
    }

    float walkSpeedFunction(float x,GLfloat deltaTime,float MovementSpeed){
        if(x<=0.2f)return 5.0f*x*MovementSpeed* deltaTime;
        return MovementSpeed* deltaTime;
    }

    float flySpeedFunction(float x,GLfloat deltaTime,float MovementSpeed,int key){
        curFlyX[key]=x;
        // if(x<=0.5f)return 4.0f*x*x*MovementSpeed* deltaTime;
        if(x<=0.3f)return 3.3f*x*MovementSpeed* deltaTime;
        return MovementSpeed* deltaTime;
    }

    float stopWalkingSpeedFunction(float x,GLfloat deltaTime,float MovementSpeed){
        return std::max(0.0f,(1.0f-6.0f*x)*MovementSpeed* deltaTime);
    }

    float stopFlyingSpeedFunction(float x,GLfloat deltaTime,float MovementSpeed,int key){
        return std::max(0.0f,flySpeedFunction(curFlyX[key],deltaTime,MovementSpeed,key) + ((isPressingWSAD?-20.0f:-3.3f)*x*x)*MovementSpeed* deltaTime);
    }

    

    void updateVelocity(GLfloat deltaTime){
        GLfloat frameVelocity = player.MovementSpeed * deltaTime;
        //玩家主动操作方面的速度已经在ProcessKeyboard中计算。
        //在game loop中，updateCamPosition在ProcessKeyboard之后调用，
        //所以可以在updateVelocity中作出对速度的最后修改，然后在updateCamPosition最终计算出位置。
        
        //水平方向上，由于阻力，减少速度
        player.OtherVelocity.z+=player.OtherVelocity.z<0?std::min(-player.OtherVelocity.z,0.2f*frameVelocity):std::max(-player.OtherVelocity.z,-0.2f*frameVelocity);
        player.OtherVelocity.x+=player.OtherVelocity.x<0?std::min(-player.OtherVelocity.x,0.2f*frameVelocity):std::max(-player.OtherVelocity.x,-0.2f*frameVelocity);

        if(!player.isFlying){
            //垂直方向上，加重力
            player.OtherVelocity.y-=frameVelocity*0.08;
            //限制最大下落速度
            if(player.OtherVelocity.y<=-6.4*frameVelocity)player.OtherVelocity.y=-6.4*frameVelocity;
        }
    
        // std::cout<<OtherVelocity.y<<'\n';
        
    }


    void processHit(glm::vec3& sumVelocity,glm::vec3 blockPos){
        float threashold=0.3f;
        
        // float antiTransThreashold=0.3f;
        // float antiTransOffset=0.15f;

        int hit=0;
        glm::vec3 playerPos=player.getLocation();
        glm::vec3 blockUb=blockPos+glm::vec3(0.5,0.5,0.5);
        glm::vec3 blockLb=blockPos-glm::vec3(0.5,0.5,0.5);

        //hit 7
            if(playerPos.z==blockUb.z||playerPos.z==blockLb.z||playerPos.x==blockUb.x||playerPos.x==blockLb.x)
                    hit=7;

        //hit 5
            if(
                // sumVelocity.y<0
                // ((playerPos.y-blockUb.y)<=threashold)
                // &&((playerPos.y-blockUb.y)>0)

                (playerPos.y<=blockUb.y)
                &&(playerPos.y>=blockUb.y-1)

            ){

                hit=5;

                if(playerPos.x<blockLb.x)
                    hit=0;

                if(playerPos.x>blockUb.x)
                    hit=0;

                if(playerPos.z<blockLb.z)
                    hit=0;

                if(playerPos.z>blockUb.z)
                    hit=0;

            }

        //hit 1
        if(
            sumVelocity.z>0
            &&((blockLb.z-playerPos.z)<=threashold)
            &&((blockLb.z-playerPos.z)>0)
        ){

            hit=1;

            if(playerPos.x<blockLb.x)
                hit=0;

            if(playerPos.x>blockUb.x)
                hit=0;

            if(playerPos.y<blockLb.y-1)
                hit=0;

            if(playerPos.y>=blockUb.y)
                hit=0;
        }

         //hit 3
        if(
            sumVelocity.z<0
            &&((playerPos.z-blockUb.z)<=threashold)
            &&((playerPos.z-blockUb.z)>0)
        ){

            hit=3;

            if(playerPos.x<blockLb.x)
                hit=0;

            if(playerPos.x>blockUb.x)
                hit=0;

            if(playerPos.y<blockLb.y-1)
                hit=0;

            if(playerPos.y>=blockUb.y)
                hit=0;
        }


        //hit 2
        if(
            sumVelocity.x>0
            &&((blockLb.x-playerPos.x)<=threashold)
            &&((blockLb.x-playerPos.x)>0)
        ){

            hit=2;

            if(playerPos.z<blockLb.z)
                hit=0;

            if(playerPos.z>blockUb.z)
                hit=0;

            if(playerPos.y<blockLb.y-1)
                hit=0;

            if(playerPos.y>=blockUb.y)
                hit=0;
        }

        //hit 4
        if(
            sumVelocity.x<0
            &&((playerPos.x-blockUb.x)<=threashold)
            &&((playerPos.x-blockUb.x)>0)
        ){

            hit=4;

            if(playerPos.z<blockLb.z)
                hit=0;

            if(playerPos.z>blockUb.z)
                hit=0;

            if(playerPos.y<blockLb.y-1)
                hit=0;

            if(playerPos.y>=blockUb.y)
                hit=0;
        }

        


            //hit 6
        if(
            sumVelocity.y>0
            &&((blockLb.y-playerPos.y-1)<=threashold)
            &&((blockLb.y-playerPos.y-1)>0)
        ){

            hit=6;

            if(playerPos.x<blockLb.x)
                hit=0;

            if(playerPos.x>blockUb.x)
                hit=0;

            if(playerPos.z<blockLb.z)
                hit=0;

            if(playerPos.z>blockUb.z)
                hit=0;

            if(hit==6){
                // player.rawPosition.y=(blockLb.y-2)*blockLength;
                Location playerLocation=player.getLocation();
                playerLocation-=V3(0.0f,2.0f,0.0f);
                player.setLocation(playerLocation);

                sumVelocity.y=0;
            }
               
        }

  
            

        if(hit==1||hit==3)sumVelocity.z=0;
        if(hit==2||hit==4)sumVelocity.x=0;
        if(hit==5&&sumVelocity.y<=0){
            // if(sumVelocity.y!=0) 
            // if(sumVelocity.y<0)sumVelocity.y=0;
            //重力加速重置
            player.OtherVelocity.y=0;
            // player.rawPosition.y=blockUb.y;
            //  std::cout<<blockUb.y<<'\n';
        }

        if(hit==7){
            sumVelocity.z=sumVelocity.x=0;
        }


        //near 1
        // if(((blockLb.z-playerPos.z)<=antiTransThreashold)
        //     &&((blockLb.z-playerPos.z)>0)){
        //         this->Position=glm::vec3(0.0f,0.0f,antiTransOffset*(-1.0f));
        //     }else this->Position=glm::vec3(0.0f,0.0f,0.0f);

        // if(hit!=0)
        // std::cout<<hit<<endl;
       
    }

    void processTargeting(){
        glm::vec3 targetingLine=glm::normalize(this->Front);
        glm::vec3 targetingLineN=glm::normalize(this->Front);
        // glm::vec3 playerPos=getPlayerPosition(false)+glm::vec3(0.0f,playerHeight,0.0f);
        glm::vec3 camPos=this->Position;

        float step=0.01f;
        float faceJudgeTherashold=0.007f;

        camPos-=glm::vec3(0.0f,playerHeight*0.24,0.0f);

        for(int dist=0;dist<=targetingDist*16;dist++){

            targetingLine=targetingLineN;
            targetingLine.x=dist*targetingLine.x*step;
            targetingLine.y=dist*targetingLine.y*step;
            targetingLine.z=dist*targetingLine.z*step;
            targetingLine+=camPos;
            targetingLine/=glm::vec3(blockLength);

            // int tx=std::round(targetingLine.x);
            // int ty=std::round(targetingLine.y);
            // int tz=std::round(targetingLine.z);

            int tx=player.castToBlockInt(targetingLine.x);
            int ty=player.castToBlockInt(targetingLine.y);
            int tz=player.castToBlockInt(targetingLine.z);

            // std::cout<<dist<<" : "<<(targetingLine.x)<<' '<<(targetingLine.y)<<' '<<(targetingLine.z)<<tx<<' '<<ty<<' '<<tz<<'\n';
       
            if(tx>=chunkSize/2||tx<-chunkSize/2||ty>=chunkSize/2||ty<-chunkSize/2||tz>=chunkSize/2||tz<-chunkSize/2)
                continue;

           
            if(world.chunkBlocks[getBlockRenderIndex(tx)][getBlockRenderIndex(ty)][getBlockRenderIndex(tz)]!=0){
                player.target=glm::vec3(tx,ty,tz);
                player.isTargeting=true;

                if(std::abs(targetingLine.x-(tx+0.5))<=faceJudgeTherashold)
                    player.targetFace=0;

                if(std::abs(targetingLine.x-(tx-0.5))<=faceJudgeTherashold)
                    player.targetFace=1;

                if(std::abs(targetingLine.y-(ty+0.5))<=faceJudgeTherashold)
                    player.targetFace=4;

                if(std::abs(targetingLine.y-(ty-0.5))<=faceJudgeTherashold)
                    player.targetFace=5;

                if(std::abs(targetingLine.z-(tz+0.5))<=faceJudgeTherashold)
                    player.targetFace=2;

                if(std::abs(targetingLine.z-(tz-0.5))<=faceJudgeTherashold)
                    player.targetFace=3;
                
                //  std::cout<<dist<<" : "<<targetingLine.x<<' '<<targetingLine.y<<' '<<targetingLine.z<<endl;

                break;
            }else{
                player.isTargeting=false;
            }
                
        }
        
    }

    void updateCamPosition(GLfloat deltaTime){
        updateVelocity(deltaTime);
        glm::vec3 sumVelocity = glm::vec3(0.0f);
        //加玩家控制产生的速度
        sumVelocity += player.ObjectiveVelocity;
        //加其他原因产生的速度，例如重力
        sumVelocity += player.OtherVelocity;

        //进一步处理sumVelocity，处理非弹性碰撞

        glm::vec3 playerPosition=player.getBlockLocation();

        for(int x=playerPosition.x-2;x<=playerPosition.x+2;x++)
            for(int y=playerPosition.y-2;y<=playerPosition.y+2;y++)
                for(int z=playerPosition.z-2;z<=playerPosition.z+2;z++){
                        if(x>=chunkSize/2||x<-chunkSize/2||y>=chunkSize/2||y<-chunkSize/2||z>=chunkSize/2||z<-chunkSize/2)continue;
                        int type=world.chunkBlocks[getBlockRenderIndex(x)][getBlockRenderIndex(y)][getBlockRenderIndex(z)];
                        if(type==0)continue;
                        processHit(sumVelocity,glm::vec3(x,y,z));
                }

        // processHit(sumVelocity,glm::vec3(0,2,1));

        //更新玩家位置
        player.rawPosition += sumVelocity;
        

        bool onTheGround=false;
        //处理脚下有方块的情况
        for(int y=playerPosition.y-2;y<=playerPosition.y;y++)
        {
                if(y>=chunkSize/2||y<-chunkSize/2)continue;
                if(playerPosition.x>=chunkSize/2||playerPosition.x<-chunkSize/2||y>=chunkSize/2||y<-chunkSize/2||playerPosition.z>=chunkSize/2||playerPosition.z<-chunkSize/2)continue;
                
                int type=world.chunkBlocks[getBlockRenderIndex(playerPosition.x)][getBlockRenderIndex(y)][getBlockRenderIndex(playerPosition.z)];
                if(type==0)continue;
                // if(player.rawPosition .y/blockLength<=y+0.5){
                //     player.rawPosition.y=(y+0.5)*blockLength;
                //     player.isOnTheGround=true;
                //     onTheGround=true;
                //        //player.rawPosition .y/blockLength是玩家的实际posotion
                //        //(y+0.5)*blockLength是把rawPosition换算成实际Position
                // }

                Location playerLoation=player.getLocation();
                if(playerLoation.y<=y+0.5){
                    playerLoation.y=y+0.5;
                    player.setLocation(playerLoation);
                        player.isOnTheGround=true;
                    onTheGround=true;
                }
                
        }       
        //防止左脚踩右脚原地升天
        if(!onTheGround)player.isOnTheGround=false;
        //碰到地停止飞行
        if(onTheGround)player.isFlying=false;

        //不掉出世界
        // if(player.rawPosition.y<=0.5*blockLength)
        // player.rawPosition.y=0.5*blockLength;

        this->Position = player.rawPosition;
        this->Position.y+=playerHeight;

        //处理非飞行下的潜行
        if(player.isSneaking&&!player.isFlying)this->Position.y+=SNEAKINGY;
        //归零，重新计算玩家控制产生的速度
        player.ObjectiveVelocity=glm::vec3(0.0f);

        processTargeting();
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime,bool pressing,int gamemode,bool isPressingWSADKey)
    { 

        GLfloat MovementSpeed=player.MovementSpeed;
        GLfloat OriginalMovementSpeed=MovementSpeed;
        GLfloat frameVelocity = player.MovementSpeed * deltaTime;


        isPressingWSAD=isPressingWSADKey;
        // std::cout<<isPressingKey<<'\n';

        if(player.isSneaking)
        //非叠加
            MovementSpeed=OriginalMovementSpeed*0.25;

        if(player.isFlying)
        //非叠加
            MovementSpeed=OriginalMovementSpeed*1.1;

        if(player.isSprinting){//叠加
            MovementSpeed*=1.5;
            pressTimeAcc['=']+=deltaTime;
        }

        // std::cout<<pressTimeAcc['=']<<' '<<this->Zoom<<'\n';

        if(player.isSprinting)
            this->Zoom=sprintZoomStartFunction(pressTimeAcc['='],deltaTime);
        else
            this->Zoom=sprintZoomEndFunction(releaseTimeAcc['='],deltaTime);

        // if (direction == FORWARD)
        //     this->Position += this->Front * velocity;
        // if (direction == BACKWARD)
        //     this->Position -= this->Front * velocity;
        // if (direction == LEFT)
        //     this->Position -= this->Right * velocity;
        // if (direction == RIGHT)
        //     this->Position += this->Right * velocity;

        // if (direction == UP){
        //     glm::vec3 upDelta=glm::vec3(0.0f,1.0f,0.0f);
        //     this->Position += upDelta*frameVelocity;
        // }

        // if (direction == DOWN){
        //     glm::vec3 upDelta=glm::vec3(0.0f,-1.0f,0.0f);
        //     this->Position += upDelta*frameVelocity;
        // }


        if (direction == TEST){
            if(pressing){
                if(pressTimeAcc['`']==0.0f){
                    inGUI=!inGUI;

                    // this->addVelocity(glm::vec3(-0.5f,0.0f,0.2f));
                    // world.setBlock(player.getBlockLocation(),1);
                    //   if(player.isTargeting)mapManager.placeBlock(player.target+glm::vec3(0.0f,1.0f,0.0f),1);
                }
                pressTimeAcc['`']+=deltaTime;
                releaseTimeAcc['`']=0.0f;
            }
            else{
                pressTimeAcc['`']=0.0f;
                releaseTimeAcc['`']+=deltaTime;
            }
        }

        if (direction == SPRINT){
            if(pressing){
                 if(!player.isSprinting&&!player.isSneaking){
                    // isSprinting=!isSprinting;
                    player.isSprinting=true;
                    // pressTimeAcc['=']+=deltaTime;
                    releaseTimeAcc['=']=0.0f;
                }
            }
            // else{
            //     pressTimeAcc['=']=0.0f;
            //     releaseTimeAcc['=']+=deltaTime;
            // }
        }

        if (direction == JUMP){
            if(pressing){

                if(pressTimeAcc['.']==0.0f){

                    if(releaseTimeAcc['.']>=0.005f&&releaseTimeAcc['.']<=0.1f){
                        player.isFlying=!player.isFlying;
                        //悬停
                        player.ObjectiveVelocity.y=0.0f;
                        player.OtherVelocity.y=0.0f;
                    }
                        
                    if(!player.isFlying&&player.isOnTheGround){
                        player.OtherVelocity.y=0.0f;
                        player.giveVelocity(Velocity(0.0f,0.07f,0.0f));
                    }

                }

                if(player.isFlying){
                    glm::vec3 upDelta=glm::vec3(0.0f,1.0f,0.0f);
                    player.ObjectiveVelocity += upDelta* flySpeedFunction(pressTimeAcc['.'],deltaTime,MovementSpeed,4);
                }

                pressTimeAcc['.']+=deltaTime;
                releaseTimeAcc['.']=0.0f;
            }
            else{

                if(player.isFlying){
                    glm::vec3 upDelta=glm::vec3(0.0f,1.0f,0.0f);
                    player.ObjectiveVelocity += upDelta* stopFlyingSpeedFunction(releaseTimeAcc['.'],deltaTime,MovementSpeed,4);
                }

                pressTimeAcc['.']=0.0f;
                releaseTimeAcc['.']+=deltaTime;
            }
        }


        if (direction == SNEAK){
            if(pressing){
                player.isSneaking=true;

                 if(player.isFlying){
                    glm::vec3 dnDelta=glm::vec3(0.0f,-1.0f,0.0f);
                    player.ObjectiveVelocity += dnDelta* flySpeedFunction(pressTimeAcc[','],deltaTime,MovementSpeed,5);
                }

                pressTimeAcc[',']+=deltaTime;
                releaseTimeAcc[',']=0.0f;
            }
            else{
               player.isSneaking=false;

                  if(player.isFlying){
                    glm::vec3 dnDelta=glm::vec3(0.0f,-1.0f,0.0f);
                    player.ObjectiveVelocity += dnDelta* stopFlyingSpeedFunction(releaseTimeAcc[','],deltaTime,MovementSpeed,5);
                }

                pressTimeAcc[',']=0.0f;
                releaseTimeAcc[',']+=deltaTime;
            }
        }
            

        if (direction == FORWARD){

            
            
            if(pressing){
                player.ObjectiveVelocity += this->FrontWithoutY * (player.isFlying? flySpeedFunction(pressTimeAcc['w'],deltaTime,MovementSpeed,0):walkSpeedFunction(pressTimeAcc['w'],deltaTime,MovementSpeed));
                
                if(!player.isSprinting&&releaseTimeAcc['w']>=0.005f&&releaseTimeAcc['w']<=0.1f&&!player.isSneaking){
                    // isSprinting=!isSprinting;
                    player.isSprinting=true;
                    // pressTimeAcc['=']+=deltaTime;
                    releaseTimeAcc['=']=0.0f;
                }

                // std::cout<<isSprinting<<'\n';
                
                pressTimeAcc['w']+=deltaTime;
                releaseTimeAcc['w']=0.0f;
            }
            else{
                player.ObjectiveVelocity += this->FrontWithoutY * (player.isFlying? stopFlyingSpeedFunction(releaseTimeAcc['w'],deltaTime,MovementSpeed,0):stopWalkingSpeedFunction(releaseTimeAcc['w'],deltaTime,MovementSpeed));

                player.isSprinting=false;

                pressTimeAcc['w']=0.0f;
                releaseTimeAcc['w']+=deltaTime;

                pressTimeAcc['=']=0.0f;
                releaseTimeAcc['=']+=deltaTime;
            }

        }


		if (direction == BACKWARD){
			    if(pressing){
                player.ObjectiveVelocity -= this->FrontWithoutY * (player.isFlying? flySpeedFunction(pressTimeAcc['s'],deltaTime,MovementSpeed,1):walkSpeedFunction(pressTimeAcc['s'],deltaTime,MovementSpeed));
                pressTimeAcc['s']+=deltaTime;
                releaseTimeAcc['s']=0.0f;
            }
            else{
                player.ObjectiveVelocity -= this->FrontWithoutY * (player.isFlying? stopFlyingSpeedFunction(releaseTimeAcc['s'],deltaTime,MovementSpeed,1):stopWalkingSpeedFunction(releaseTimeAcc['s'],deltaTime,MovementSpeed));
                pressTimeAcc['s']=0.0f;
                releaseTimeAcc['s']+=deltaTime;
            }
        }

		if (direction == LEFT){

            if(pressing){
                player.ObjectiveVelocity -= this->Right * (player.isFlying? flySpeedFunction(pressTimeAcc['a'],deltaTime,MovementSpeed,2):walkSpeedFunction(pressTimeAcc['a'],deltaTime,MovementSpeed));
                pressTimeAcc['a']+=deltaTime;
                releaseTimeAcc['a']=0.0f;
            }
            else{
                player.ObjectiveVelocity -= this->Right * (player.isFlying? stopFlyingSpeedFunction(releaseTimeAcc['a'],deltaTime,MovementSpeed,2):stopWalkingSpeedFunction(releaseTimeAcc['a'],deltaTime,MovementSpeed));
                pressTimeAcc['a']=0.0f;
                releaseTimeAcc['a']+=deltaTime;
            }

			// float lastY=this->Position.y;
            // this->Position -= this->Right * velocity;
            // this->Position.y=lastY;
        }

		if (direction == RIGHT){

            if(pressing){
                player.ObjectiveVelocity += this->Right * (player.isFlying? flySpeedFunction(pressTimeAcc['d'],deltaTime,MovementSpeed,3):walkSpeedFunction(pressTimeAcc['d'],deltaTime,MovementSpeed));
                pressTimeAcc['d']+=deltaTime;
                releaseTimeAcc['d']=0.0f;
            }
            else{
                player.ObjectiveVelocity += this->Right * (player.isFlying? stopFlyingSpeedFunction(releaseTimeAcc['d'],deltaTime,MovementSpeed,3):stopWalkingSpeedFunction(releaseTimeAcc['d'],deltaTime,MovementSpeed));
                pressTimeAcc['d']=0.0f;
                releaseTimeAcc['d']+=deltaTime;
            }

			// float lastY=this->Position.y;
            // this->Position += this->Right * velocity;
            // this->Position.y=lastY;
        }           

        

    }

    void ProcessMouseClick(Camera_Movement direction, GLfloat deltaTime,bool pressing,int gamemode){
        if(direction==ATTACK){
            if(pressing){
                //  std::cout<<"attack\n";
                player.processClick(true);
            }
        }

        if(direction==USE){
            if(pressing){
                player.processClick(false);
            }
        }
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw   += xoffset;
        this->Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        this->updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(GLfloat yoffset)
    {
        if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
            this->Zoom -= yoffset;
        if (this->Zoom <= 1.0f)
            this->Zoom = 1.0f;
        if (this->Zoom >= 45.0f)
            this->Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
        this->FrontWithoutY = glm::normalize(glm::cross(glm::vec3(0,1,0),this->Right));
    }

};