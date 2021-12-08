#pragma once

#include "Model.h"
#include <map>
#include <GL/glew.h>


// Independent Models
Model pickaxeModel;
Model chestModel;
Model horseModel;
Model torchModel;
Model doorModel;


void initIndependentModels(){
      //independent models
      pickaxeModel=Model("models/mc_diamondpickaxe/Diamond-Pickaxe.obj");
      chestModel=Model("models/mc_chest/chest.obj");
      horseModel=Model("models/mc_horse/source/horse.fbx");
      torchModel=Model("models/mc-torch/Torch.obj");
      doorModel=Model("models/mc_door/Wooden-Door.obj");
}