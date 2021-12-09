#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform int targeted;

void main()
{    
    vec4 result= vec4(texture(texture_diffuse1, TexCoords));
    if(targeted==1) result=mix(result,vec4(1.0,1.0,1.0,1.0),0.35);
    color = result;
}