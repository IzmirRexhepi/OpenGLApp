#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 lightMatrices[6];

out vec4 FragPos;

void main (){
	for(int orientation = 0; orientation < 6; orientation++){
		gl_Layer = orientation;
		for(int i = 0; i < 3; i++){
			FragPos = gl_in[i].gl_Position;
			gl_Position = lightMatrices[orientation] * FragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}