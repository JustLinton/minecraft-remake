#pragma once

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameProperties.h"

glm::mat4 unitMat(    // 单位矩阵
	glm::vec4(1, 0, 0, 0),
	glm::vec4(0, 1, 0, 0),
	glm::vec4(0, 0, 1, 0),
	glm::vec4(0, 0, 0, 1)
);

int getBlockRenderIndex(int a){
	return a+chunkSize/2;
}

// struct BlockPosition
// {
//       //在我的MC中，世界坐标系统是标准的右手坐标系
//       int x=0,y=0,z=0;
//       BlockPosition(){}
//       BlockPosition(int x_,int y_,int z_):x(x_),y(y_),z(z_){}
// };
