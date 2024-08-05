#version 330										

in vec4 vColor;
in vec2 texCoord;
in vec3 normal;	//phong shading
//flat in vec3 normal;	//create flat shading effect (do not do it like this)
										
out vec4 colour;

struct DirectionalLight {
	vec3 colour;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

uniform	sampler2D theTexture;
uniform DirectionalLight directionalLight;								

void main()
{	
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
	
	//calc dot = dot B = magnitude of A multiplied the magnitude B, multiplied by cosine of angle between vectors
	// A*B = |A|*|B|cos(angle)
	// |A|*|B|cos(angle) = |1|*|1|cos(angle) = cos(angle)
	//angle = 90degree = 0 value; || angle = 0degree = 1 value; 
	//max = if value < 0 then value = 0;

	float diffuseFactor = max(dot(normalize(normal), normalize(directionalLight.direction)), 0.0f); //normalize = convert to a unit vector
	vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor; //diffuse intensity brightness, diffuseFactor = secured allowed angle to appear
 
	//colour = ambientColour * vColor;
	colour = texture(theTexture, texCoord) * (ambientColour + diffuseColour); //* vColor;
								
}