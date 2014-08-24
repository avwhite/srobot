#include"controls.h"
#include<stdio.h>

vec3 position = {0, 1, 5};
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float initialFov = 0.78f;

float speed = 3.0f;
float mouseSpeed = 0.005f;

void updateControlMatrices(GLFWwindow* window, float deltaTime, mat4x4 viewMatrix, mat4x4 projMatrix) {

	double xpos, ypos;
	float fov;

	vec3 temp;
	vec3 direction, right, up, lookAt;

	//glfwGetCursorPos(window, &xpos, &ypos);
	//glfwSetCursorPos(window, 1024/2, 768/2);

	//horizontalAngle += mouseSpeed * (float)(1024/2 - xpos);
	//verticalAngle   += mouseSpeed * (float)( 768/2 - ypos);

	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		horizontalAngle -= (speed/3) * deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		horizontalAngle += (speed/3) * deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		verticalAngle += (speed/3) * deltaTime;
	}
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		verticalAngle -= (speed/3) * deltaTime;
	}

	direction[0] = cosf(verticalAngle) * sinf(horizontalAngle);
	direction[1] = sinf(verticalAngle);
	direction[2] = cosf(verticalAngle) * cosf(horizontalAngle);

	right[0] = sinf(horizontalAngle - 3.14f/2.0f);
	right[1] = 0;
	right[2] = cosf(horizontalAngle - 3.14f/2.0f);

	vec3_mul_cross(up, right, direction);

	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		vec3_scale(temp, direction, deltaTime * speed);
		vec3_add(position, position, temp);
	}
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		vec3_scale(temp, direction, deltaTime * speed);
		vec3_sub(position, position, temp);
	}
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		vec3_scale(temp, right, deltaTime * speed);
		vec3_add(position, position, temp);
	}
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		vec3_scale(temp, right, deltaTime * speed);
		vec3_sub(position, position, temp);
	}

	fov = initialFov;

	mat4x4_perspective(projMatrix,
		fov,
		4.0f / 3.0f,
		0.1f,
		100.0f
	);
	
	vec3_add(lookAt, position, direction);
	
	mat4x4_look_at(viewMatrix,
		position,
		lookAt,
		up
	);
}
