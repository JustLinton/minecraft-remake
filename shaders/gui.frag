#version 330 core
in vec2 TexCoords;
in vec2 TexCoords2;
in vec2 TexSlot1Coords;
out vec4 color;

uniform sampler2D text;
uniform sampler2D text2;
uniform sampler2D textSlot0;
uniform sampler2D textSlot1;
// uniform vec3 textColor;

void main()
{    
    // vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    vec4 sampled = vec4(texture(text, TexCoords).r, texture(text, TexCoords).g, texture(text, TexCoords).b, 0.75);
    vec4 sampled2 = vec4(texture(text2, TexCoords2).r, texture(text2, TexCoords2).g, texture(text2, TexCoords2).b, 0.5);

    vec4 sampledSlot1 = vec4(texture(textSlot1, TexSlot1Coords).r, texture(textSlot1, TexSlot1Coords).g, texture(textSlot1, TexSlot1Coords).b, 1.0);

    if(sampledSlot1.x<=0.1&&sampledSlot1.y<=0.1&&sampledSlot1.z<=0.1){
        //扣图，这样一会mix，白色就被扣掉了
        sampledSlot1.x=1.0;
        sampledSlot1.y=1.0;
        sampledSlot1.z=1.0;
    }

    //建立上下关系
    if(sampled2.x!=1.0)
        sampled.x=1.0;
    if(sampled2.y!=1.0)
        sampled.y=1.0;
    if(sampled2.z!=1.0)
        sampled.z=1.0;

    vec4 slotMixed=sampled*sampled2;

    //建立上下关系
    if(sampledSlot1.x!=1.0)
        slotMixed.x=1.0;
    if(sampledSlot1.y!=1.0)
        slotMixed.y=1.0;
    if(sampledSlot1.z!=1.0)
        slotMixed.z=1.0;

    vec4 mixed=slotMixed*sampledSlot1;
    
    // vec4 sampled = mix(texture(text, TexCoords), texture(text2, TexCoords2), 0.2);
    // color = vec4(textColor, 1.0) * sampled;
    color = vec4(mixed.x,mixed.y,mixed.z,0.87);

}  