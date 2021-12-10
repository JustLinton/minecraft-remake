#pragma once

#include <bits/stdc++.h>
#include "Blocks.h"
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GameProperties.h"
#include "Utils.h"
using namespace std;


class World{
      public:

            vector<vector<vector <int>> > chunkBlocks;                       
                   
            World(){
                  mapIfs=ifstream("maps/chunk1");

                  if (!mapIfs.is_open())
                  {
                        cout << "can not open map file." << endl;
                        exit(0);
                  }

                  for(int i=1;i<=chunkSize;i++){
                        chunkBlocks.push_back(vector<vector<int> >());
                        for(int j=1;j<=chunkSize;j++){
                              chunkBlocks[i-1].push_back(vector<int>());
                                    for(int k=1;k<=chunkSize;k++)
                                          chunkBlocks[i-1][j-1].push_back(0);
                        }
                   }
                 
            }

            void readMapFile(){
                  int x=0,y=0,z=0,type=0;
                  int length;
                  mapIfs>>length;
                  for(int l=1;l<=length;l++){
                        mapIfs>>x>>y>>z>>type;
                        if(x>=chunkSize/2||x<-chunkSize/2|y>=chunkSize/2||y<-chunkSize/2||z>=chunkSize/2||z<-chunkSize/2){
                              printError();
                              continue;
                        }
                        chunkBlocks[getBlockRenderIndex(x)][getBlockRenderIndex(y)][getBlockRenderIndex(z)]=type;
                  }
                  // cout<<x<<y<<z<<type<<endl;
            }

            bool setBlock(glm::vec3 pos,int type){
                  if(pos.x>=chunkSize/2||pos.x<-chunkSize/2|pos.y>=chunkSize/2||pos.y<-chunkSize/2||pos.z>=chunkSize/2||pos.z<-chunkSize/2)
                        return false;
                  chunkBlocks[getBlockRenderIndex(pos.x)][getBlockRenderIndex(pos.y)][getBlockRenderIndex(pos.z)]=type;
                  return true;
            }

            bool saveMapFile(){

                  mapOfs=ofstream("maps/chunk1");

                  if (!mapOfs.is_open())
                  {
                        cout << "can not save map file." << endl;
                        return false;
                  }


                  int length=0;

                  for(int x=-chunkSize/2;x<=chunkSize/2-1;x++)
                        for(int y=-chunkSize/2;y<=chunkSize/2-1;y++)
                              for(int z=-chunkSize/2;z<=chunkSize/2-1;z++){
                                    int type=chunkBlocks[getBlockRenderIndex(x)][getBlockRenderIndex(y)][getBlockRenderIndex(z)];
                                    if(type!=0)length++;
                              }

                  mapOfs<<length<<'\n';

                  for(int x=-chunkSize/2;x<=chunkSize/2-1;x++)
                        for(int y=-chunkSize/2;y<=chunkSize/2-1;y++)
                              for(int z=-chunkSize/2;z<=chunkSize/2-1;z++){
                                          int type=chunkBlocks[getBlockRenderIndex(x)][getBlockRenderIndex(y)][getBlockRenderIndex(z)];
                                          if(type==0)continue;
                                           mapOfs<<x<<' '<<y<<' '<<z<<' '<<type<<'\n';
                              }

                  return true;

            }

            ~World(){
                  mapIfs.close();
                  mapOfs.close();
            }

      private:
            ifstream mapIfs;
            ofstream mapOfs;

            void printError(){
                  cout<<"map read err."<<endl;
            }

           
};

World world;