#version 330 core
out vec3 color;

in vec3 fragmentColor;
in vec3 position_worldspace;
in vec3 normal_cameraspace;
in vec3 eyeDirection_cameraspace;
in vec3 lightDirection_cameraspace;

uniform vec3 lightPosition_worldspace;
 
void main(){
	vec3 lightColor = vec3(1,1,1);
	float lightPower = 50.0f;

	vec3 diffColor = fragmentColor;
	vec3 ambColor = vec3(0.5,0.5,0.5) * diffColor;
	vec3 specColor = vec3(0.3,0.3,0.3);

	float distance = length(lightPosition_worldspace - position_worldspace);

	vec3 n = normalize(normal_cameraspace);
	vec3 l = normalize(lightDirection_cameraspace);
	float cosTheta = clamp(dot(n,l),0,1);

	vec3 e = normalize(eyeDirection_cameraspace);
	vec3 r = reflect(-l, n);
	float cosAlpha = clamp(dot(e,r),0,1);

	color = ambColor +
		diffColor * lightColor * lightPower * cosTheta / (distance * distance) +
		specColor * lightColor * lightPower * pow(cosAlpha,5) / (distance * distance);
}
