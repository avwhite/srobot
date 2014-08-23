#ifndef CONTROLS_H
#define CONTROLS_H

#include<GLFW/glfw3.h>
#include"linmath.h"

void getViewMatrix(mat4x4 m);
void getProjectionMatrix(mat4x4 m);
void updateControlMatrices(GLFWwindow* window, float deltaTime, mat4x4 projMatrix, mat4x4 viewMatrix);

#endif
