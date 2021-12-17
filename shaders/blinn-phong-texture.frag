#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  

//my
in vec2 TexCoords;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform int targeted;
uniform vec3 nearBlocksPos[125];
uniform int nearBlocksMaxIndex;
uniform float blockLength;

//my
uniform sampler2D texture_diffuse1;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient;
    ambient=mix(ambient,vec4(texture(texture_diffuse1, TexCoords)).xyz,0.2);
    // vec3 ambient = light.ambient * vec4(texture(texture_diffuse1, TexCoords)).xyz;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = light.diffuse * (diff * material.diffuse);
    vec3 diffuse = light.diffuse * (diff *  vec4(texture(texture_diffuse1, TexCoords)).xyz);
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    vec3 phongResult = ambient + diffuse + specular;
    vec4 result = vec4(phongResult, 1.0f);

    // vec4 result= vec4(texture(texture_diffuse1, TexCoords));



    vec4 originRes=result;
  

    //RTX ON!
    // for(int i=0;i<=nearBlocksMaxIndex;i++){
    //     vec3 coverBlockPos=nearBlocksPos[i];

    //     vec3 reSize= vec3(blockLength);
    //     vec3 lightPos=(light.position)/reSize;
    //     vec3 fgPos=FragPos/reSize;
    //     // float step=0.2;
    //     float step=0.5;
    //     float shadowConsiderLength=2.5;
    //     //1.732/2
    //     // float threas=0.868;
    //     float threas=0.8682;

    //     vec3 itePos=fgPos;
    //     vec3 stepV=normalize(vec3(lightPos.x-fgPos.x,lightPos.y-fgPos.y,lightPos.z-fgPos.z));
    //     // stepV=stepV*step;

    //     bool covered=false;

    //     // stepV = ()*(step);
    //     for(float s=0.58;s<=shadowConsiderLength;s+=step){
    //         itePos=itePos+stepV*s;
    //         vec3 tmpV=coverBlockPos-itePos;
    //         if(tmpV.x>=-threas&&tmpV.x<=threas)
    //             if(tmpV.y>=-threas&&tmpV.y<=threas)
    //                 if(tmpV.z>=-threas&&tmpV.z<=threas){
    //                     // result=vec4(1.0f,1.0f,1.0f,1.0f);
    //                     result=mix(originRes,vec4(0.0f,0.0f,0.0f,1.0f),0.6);
    //                     covered=true;
    //                     break;
    //                 }      
    //     }

    //     if(covered)break;

    // }


    for(int i=0;i<=nearBlocksMaxIndex;i++){
        
        vec3 reSize= vec3(blockLength);
        vec3 coverBlockPos=nearBlocksPos[i];
        coverBlockPos=coverBlockPos*reSize;

        vec3 lightPos=light.position;
        vec3 fgPos=FragPos;

        // float step=0.2;
        float step=0.5*blockLength;
        float shadowConsiderLength=3.0*blockLength;
        //1.732/2
        // float threas=0.868;
        float threas=0.8682*blockLength;

        //修复上半方块没有阴影的问题
        // fgPos.y=fgPos.y-0.16;

        vec3 itePos=fgPos;
        vec3 stepV=normalize(vec3(lightPos.x-fgPos.x,lightPos.y-fgPos.y,lightPos.z-fgPos.z));
        // stepV=stepV*step;

        bool covered=false;

        // stepV = ()*(step);
        for(float s=0.30;s<=shadowConsiderLength;s+=step){
            itePos=itePos+stepV*s;
            vec3 tmpV=coverBlockPos-itePos;
            if(tmpV.x>=-threas&&tmpV.x<=threas)
                if(tmpV.y>=-threas&&tmpV.y<=threas)
                    if(tmpV.z>=-threas&&tmpV.z<=threas){
                        // result=vec4(1.0f,1.0f,1.0f,1.0f);
                        result=mix(originRes,vec4(0.0f,0.0f,0.0f,1.0f),0.6);
                        covered=true;
                        break;
                    }      
        }

        if(covered)break;

    }


    if(targeted==1&&(

        (TexCoords.x>=0.0&&TexCoords.x<=0.006)
        ||(TexCoords.x>=0.32&&TexCoords.x<=0.33)
        ||(TexCoords.x>=0.33&&TexCoords.x<=0.34)
        ||(TexCoords.x>=0.66&&TexCoords.x<=0.67)
        ||(TexCoords.x>=0.67&&TexCoords.x<=0.68)
        ||(TexCoords.x>=0.994&&TexCoords.x<=1.0)

        ||(TexCoords.y>=0.0&&TexCoords.y<=0.006)
        ||(TexCoords.y>=0.244&&TexCoords.y<=0.25)
        ||(TexCoords.y>=0.25&&TexCoords.y<=0.256)
        ||(TexCoords.y>=0.494&&TexCoords.y<=0.5)
        ||(TexCoords.y>=0.5&&TexCoords.y<=0.506)
        ||(TexCoords.y>=0.744&&TexCoords.y<=0.75)
        ||(TexCoords.y>=0.75&&TexCoords.y<=0.756)
        ||(TexCoords.y>=0.994&&TexCoords.y<=1.0)

    )) result=mix(result,vec4(1.0,1.0,1.0,1.0),0.45);

    color = result;
    // color=vec4(nearBlocksPos[0],1.0f);
} 