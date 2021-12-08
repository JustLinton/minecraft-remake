#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;
out vec2 TexCoords2;
out vec2 TexSlot1Coords;

uniform mat4 projection;
uniform int sel;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
    // TexCoords =  vec2(vertex.z*8.3*1.6-0.9,vertex.w*1.6);
    TexCoords2 =  vec2(vertex.z*8.3-sel*0.914,vertex.w);
    // TexCoords2 =  vec2(vertex.z*8.3-0.9,vertex.w);
    TexSlot1Coords=vec2(vertex.z*8.3*1.6-1.7,vertex.w*1.6-0.27);
}  