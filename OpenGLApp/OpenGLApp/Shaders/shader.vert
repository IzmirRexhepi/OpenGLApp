#version 330
											
layout(location = 0) in vec3 pos; //position
layout (location = 1) in vec2 tex; //texture
layout (location =2) in vec3 norm; //normals

out vec4 vColor;
out vec2 texCoord; //texture cordinates	
out vec3 normal; //phong shading
//flat out vec3 normal;	//create flat shading effect (do not do it like this)							

uniform mat4 model;									
uniform mat4 projection;
uniform mat4 view;								

void main()
{										
	gl_Position = projection * view * model * vec4(pos, 1.0);
	vColor = 	vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	texCoord = tex;

	//summuraize:
	//Using model to account changes in rotation and scaling
	//making matrix 3x3 because we don't take into account any transform since normals is a direction and not a position
	//using transpose inverse to invert scaling calculation on moddel to account non uniform scaling (if scale in one direction is needed)
	normal = mat3(transpose(inverse(model))) * norm;
	
}
