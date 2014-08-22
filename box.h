#ifndef BOX_H
#define BOX_H

#include<GL/glew.h>
#include"linmath.h"

typedef struct {
	GLuint vertex_buffer, color_buffer;
	float x,y,z;
} Box;

typedef struct {
	Box *entities;
	int length, cap;
} EntityList;

Box createBox(float w, float h, float d);
void renderBox(Box* box);
void deleteBox(Box* box);
void getBoxModelMatrix(Box *box, mat4x4 m);

EntityList createEntityList(int initCap);
void insertEntity(EntityList *e, Box *b);
Box *getEntity(EntityList *e, int index);
void deleteEntityList(EntityList *e);

#endif
