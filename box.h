#ifndef BOX_H
#define BOX_H

#include<GL/glew.h>
#include<ode/ode.h>
#include"linmath.h"

typedef struct {
	GLuint vertex_buffer, color_buffer;
	
	int numVerticies;

	dBodyID body;
	dGeomID geom;
} Box;

typedef struct {
	Box *entities;
	int length, cap;

	dWorldID physics;
	dSpaceID collision;
} EntityList;

Box *createBox(EntityList *e, float w, float h, float d, float x, float y, float z);
Box *createGround(EntityList *e);
void renderBox(Box* box);
void deleteBox(Box* box);
void getBoxModelMatrix(Box *box, mat4x4 m);

EntityList createEntityList(int initCap);
void insertEntity(EntityList *e, Box *b);
Box *getEntity(EntityList *e, int index);
void deleteEntityList(EntityList *e);

void updateBoxes(EntityList *e, float deltaTime);

#endif
