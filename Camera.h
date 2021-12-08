#pragma once

// Std. Includes
#include <vector>
#include <map>
#include <algorithm>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    //creative
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP,
    SNEAK
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat STANDARDSPEED      =  3.0f;
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
    glm::vec3 Velocity;
    glm::vec3 OtherVelocity;
    std::map<char,float> pressTimeAcc;
    std::map<char,float> releaseTimeAcc;
    bool isSneaking;
    // Eular Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(STANDARDSPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
        initPhysics();
    }
    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(STANDARDSPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
        initPhysics();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
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
        this->Velocity=glm::vec3(0.0f);
        this->OtherVelocity=glm::vec3(0.0f);
    }


    float walkSpeedFunction(float x,GLfloat deltaTime){
        if(x<=0.25f)return 4.0f*x*this->MovementSpeed* deltaTime;
        if(x>0.25f)return this->MovementSpeed* deltaTime;
    }

    float stopWalkingSpeedFunction(float x,GLfloat deltaTime){
        return std::max(0.0f,(1.0f-4.5f*x)*this->MovementSpeed* deltaTime);
    }

    void addVelocity(glm::vec3 velo){
        this->OtherVelocity+=velo;
    }

    void updateVelocity(GLfloat deltaTime){
        GLfloat frameVelocity = this->MovementSpeed * deltaTime;
        //玩家主动操作方面的速度已经在ProcessKeyboard中计算。
        //在game loop中，updateCamPosition在ProcessKeyboard之后调用，
        //所以可以在updateVelocity中作出对速度的最后修改，然后在updateCamPosition最终计算出位置。
        
        //水平方向上，由于阻力，减少速度
        OtherVelocity.z+=this->OtherVelocity.z<0?std::min(-this->OtherVelocity.z,frameVelocity):std::min(this->OtherVelocity.z,frameVelocity);
        OtherVelocity.x+=this->OtherVelocity.x<0?std::min(-this->OtherVelocity.x,frameVelocity):std::min(this->OtherVelocity.x,frameVelocity);

        //垂直方向上，加重力
        OtherVelocity.y-=frameVelocity*0.08;
        //限制最大下落速度
        if(OtherVelocity.y<=-6.4*frameVelocity)OtherVelocity.y=-6.4*frameVelocity;
        // std::cout<<OtherVelocity.y<<'\n';
        
    }

    void updateCamPosition(GLfloat deltaTime){
        updateVelocity(deltaTime);
        glm::vec3 sumVelocity = glm::vec3(0.0f);
        //加玩家控制产生的速度
        sumVelocity += this->Velocity;
        //加其他原因产生的速度，例如重力
        sumVelocity += this->OtherVelocity;
        //合成速度，更新玩家位置
        this->Position += sumVelocity;
        if(this->Position.y<=4.0f*0.5405f)
        this->Position.y=4.0*0.5405f;
        //处理潜行
        if(isSneaking)this->Position.y+=SNEAKINGY;
        //归零，重新计算玩家控制产生的速度
        this->Velocity=glm::vec3(0.0f);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime,bool pressing,int gamemode)
    { 

        GLfloat frameVelocity = this->MovementSpeed * deltaTime;

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

        if (direction == JUMP){
            if(pressing){
                if(pressTimeAcc['.']==0.0f){
                    OtherVelocity.y=0.0f;
                    this->addVelocity(glm::vec3(0.0f,0.07f,0.0f));
                }
                pressTimeAcc['.']+=deltaTime;
                releaseTimeAcc['.']=0.0f;
            }
            else{
                pressTimeAcc['.']=0.0f;
                releaseTimeAcc['.']+=deltaTime;
            }
        }


        if (direction == SNEAK){
            if(pressing){
                isSneaking=true;
                pressTimeAcc[',']+=deltaTime;
                releaseTimeAcc[',']=0.0f;
            }
            else{
                isSneaking=false;
                pressTimeAcc[',']=0.0f;
                releaseTimeAcc[',']+=deltaTime;
            }
        }
            

        if (direction == FORWARD){
            
            if(pressing){
                this->Velocity += this->FrontWithoutY * walkSpeedFunction(pressTimeAcc['w'],deltaTime);
                pressTimeAcc['w']+=deltaTime;
                releaseTimeAcc['w']=0.0f;
            }
            else{
                this->Velocity += this->FrontWithoutY * stopWalkingSpeedFunction(releaseTimeAcc['w'],deltaTime);
                pressTimeAcc['w']=0.0f;
                releaseTimeAcc['w']+=deltaTime;
            }

        }


		if (direction == BACKWARD){
			    if(pressing){
                this->Velocity -= this->FrontWithoutY * walkSpeedFunction(pressTimeAcc['s'],deltaTime);
                pressTimeAcc['s']+=deltaTime;
                releaseTimeAcc['s']=0.0f;
            }
            else{
                this->Velocity -= this->FrontWithoutY * stopWalkingSpeedFunction(releaseTimeAcc['s'],deltaTime);
                pressTimeAcc['s']=0.0f;
                releaseTimeAcc['s']+=deltaTime;
            }
        }

		if (direction == LEFT){

            if(pressing){
                this->Velocity -= this->Right * walkSpeedFunction(pressTimeAcc['a'],deltaTime);
                pressTimeAcc['a']+=deltaTime;
                releaseTimeAcc['a']=0.0f;
            }
            else{
                this->Velocity -= this->Right * stopWalkingSpeedFunction(releaseTimeAcc['a'],deltaTime);
                pressTimeAcc['a']=0.0f;
                releaseTimeAcc['a']+=deltaTime;
            }

			// float lastY=this->Position.y;
            // this->Position -= this->Right * velocity;
            // this->Position.y=lastY;
        }

		if (direction == RIGHT){

            if(pressing){
                this->Velocity += this->Right * walkSpeedFunction(pressTimeAcc['d'],deltaTime);
                pressTimeAcc['d']+=deltaTime;
                releaseTimeAcc['d']=0.0f;
            }
            else{
                this->Velocity += this->Right * stopWalkingSpeedFunction(releaseTimeAcc['d'],deltaTime);
                pressTimeAcc['d']=0.0f;
                releaseTimeAcc['d']+=deltaTime;
            }

			// float lastY=this->Position.y;
            // this->Position += this->Right * velocity;
            // this->Position.y=lastY;
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