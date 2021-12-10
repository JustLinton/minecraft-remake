#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform int targeted;

void main()
{    
    vec4 result= vec4(texture(texture_diffuse1, TexCoords));
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
}