#include"box.h"
#include<GL/glew.h>
#include<stdlib.h>
#include"linmath.h"

Box *createBox(EntityList *e, float wi, float he, float de, float x, float y, float z)
{
	Box box;
	dMass m;
	float w, h, d;

	w = wi/2;
	h = he/2;
	d = de/2;

	box.numVerticies = 36;

	GLfloat g_vertex_buffer_data[] = {
		w, h, d, 1, 0, 0,  //right face
		w, h, -d, 1, 0, 0,
		w, -h, d, 1, 0, 0,
		w, h, -d, 1, 0, 0,
		w, -h, d, 1, 0, 0,
		w, -h, -d, 1, 0, 0,

		-w, h, d, -1, 0, 0,//left face
		-w, h, -d, -1, 0, 0,
		-w, -h, d, -1, 0, 0,
		-w, h, -d, -1, 0, 0,
		-w, -h, d, -1, 0, 0,
		-w, -h, -d, -1, 0, 0,

		w, h, d, 0, 1, 0, //up face
		w, h, -d, 0, 1, 0,
		-w, h, d, 0, 1, 0,
		w, h, -d, 0, 1, 0,
		-w, h, d, 0, 1, 0,
		-w, h, -d, 0, 1, 0,

		w, -h, d, 0, -1, 0, //down face
		w, -h, -d, 0, -1, 0,
		-w, -h, d, 0, -1, 0,
		w, -h, -d, 0, -1, 0,
		-w, -h, d, 0, -1, 0,
		-w, -h, -d, 0, -1, 0,

		w,h,d, 0, 0, 1, //front face
		w,-h,d, 0, 0, 1,
		-w,h,d, 0, 0, 1,
		w,-h,d, 0, 0, 1,
		-w,h,d, 0, 0, 1,
		-w,-h,d, 0, 0, 1,

		w,h,-d, 0, 0, -1,//back face
		w,-h,-d, 0, 0, -1,
		-w,h,-d, 0, 0, -1,
		w,-h,-d, 0, 0, -1,
		-w,h,-d, 0, 0, -1,
		-w,-h,-d, 0, 0, -1
	};

	GLfloat g_color_buffer_data[box.numVerticies*3];
	int v;
	for (v = 0; v < box.numVerticies ; v++){
		g_color_buffer_data[3*v+0] = 0.0;
		g_color_buffer_data[3*v+1] = 0.5;
		g_color_buffer_data[3*v+2] = 0.0;
	}

	GLuint colorbuffer;
	glGenBuffers(1, &(box.color_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, box.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &(box.vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, box.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	box.body = dBodyCreate(e->physics);
	dMassSetZero(&m);
	dMassSetBoxTotal(&m,1.0,wi,he,de);
	dBodySetPosition(box.body, x,  y, z);

	box.geom = dCreateBox(e->collision, wi, he, de);

	dGeomSetBody(box.geom,box.body);

	if (e->length == e->cap) {
		e->cap *= 2;
		e->entities = realloc(e->entities, sizeof(Box) * e->cap);
	}
	e->entities[e->length] = box;
	e->length += 1;

	return &e->entities[e->length-1];
}

Box *createGround(EntityList *e) {
	float w = 200;
	float h = 0.1;
	float d = 200;

	Box box;

	box.numVerticies = 6;

	GLfloat g_vertex_buffer_data[] = {
		w, h, d, 0, 1, 0, //up face
		w, h, -d, 0, 1, 0,
		-w, h, d, 0, 1, 0,
		w, h, -d, 0, 1, 0,
		-w, h, d, 0, 1, 0,
		-w, h, -d, 0, 1, 0,
	};

	GLfloat g_color_buffer_data[box.numVerticies*3];
	int v;
	for (v = 0; v < box.numVerticies ; v++){
		g_color_buffer_data[3*v+0] = 0.5;
		g_color_buffer_data[3*v+1] = 0.5;
		g_color_buffer_data[3*v+2] = 0.5;
	}

	GLuint colorbuffer;
	glGenBuffers(1, &(box.color_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, box.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &(box.vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, box.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//box.body = dBodyCreate(e->physics);
	//dBodySetPosition(box.body, x,  y, z);
	//dBodySetKinematic(box.body);

	box.geom = dCreatePlane(e->collision,0,1,0,0);

	if (e->length == e->cap) {
		e->cap *= 2;
		e->entities = realloc(e->entities, sizeof(Box) * e->cap);
	}
	e->entities[e->length] = box;
	e->length += 1;

	return &e->entities[e->length-1];
}

void renderBox(Box* box)
{	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, box->vertex_buffer);
	glVertexAttribPointer(
	   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   sizeof(GLfloat)*6,                  // stride
	   (void*)0            // array buffer offset
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, box->vertex_buffer);
	glVertexAttribPointer(
	   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   sizeof(GLfloat)*6,                  // stride
	   (void *)(sizeof(GLfloat)*3)            // array buffer offset
	);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, box->color_buffer);
	glVertexAttribPointer(
	   2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	   3,                  // size
	   GL_FLOAT,           // type
	   GL_FALSE,           // normalized?
	   0,                  // stride
	   (void*)0            // array buffer offset
	);
	 
	glDrawArrays(GL_TRIANGLES, 0, box->numVerticies);
	 
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(3);
}

void getBoxModelMatrix(Box *box, mat4x4 m)
{
	const dReal *pos;
	if(dGeomGetClass(box->geom) == 4) {
		//We are dealing with a plane, which has no position
		mat4x4_identity(m);
	} else {
		pos = dGeomGetPosition(box->geom);
		mat4x4_translate(m, pos[0], pos[1], pos[2]);
	}
}

void deleteBox(Box* box)
{
	glDeleteBuffers(1, &(box->vertex_buffer));
	glDeleteBuffers(1, &(box->color_buffer));
}

void insertEntity(EntityList *e, Box *b)
{
	if (e->length == e->cap) {
		e->cap *= 2;
		e->entities = realloc(e->entities, sizeof(Box) * e->cap);
	}
	e->entities[e->length] = *b;
	e->length += 1;
}

Box *getEntity(EntityList *e, int index)
{
	if (index >= 0 && index <= e->length) {
		return &(e->entities[index]);
	} else {
		return NULL;
	}
}

EntityList createEntityList(int initCap)
{
	EntityList e;
	e.entities = malloc(sizeof(Box)*initCap);
	e.length = 0;
	e.cap = initCap;
	e.physics = dWorldCreate();
	e.collision = dHashSpaceCreate(0);
	e.collisionJoints = dJointGroupCreate(0);
	dWorldSetGravity(e.physics, 0, -0.01, 0);
	return e;
}


void deleteEntityList(EntityList *e)
{
	int i;
	for (i = 0; i < e->length; ++i) {
		deleteBox(&(e->entities[i]));
	}
	dWorldDestroy(e->physics);
	dSpaceDestroy(e->collision);
	dJointGroupDestroy(e->collisionJoints);
	free(e->entities);
	e->cap = 0;
	e->length = 0;
}

void updateBoxes(EntityList *e, float deltaTime)
{
	dSpaceCollide(e->collision, e, &nearCallback);
	dWorldStep(e->physics,  0.05);
	dJointGroupEmpty(e->collisionJoints);
}

void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	EntityList *e;
	dBodyID b1;
	dBodyID b2;
	int numc;
	dContact contact;  
	int i;
	
	e = data;

	b1 = dGeomGetBody(o1);
	b2 = dGeomGetBody(o2);

    contact.surface.mode = dContactBounce/* | dContactSoftCFM*/;
    // friction parameter
    contact.surface.mu = dInfinity;
    // bounce is the amount of "bouncyness".
    contact.surface.bounce = 0.9;
    // bounce_vel is the minimum incoming velocity to cause a bounce
    contact.surface.bounce_vel = 0.1;
    // constraint force mixing parameter
    //contact.surface.soft_cfm = 0.001;  

	if (numc = dCollide (o1,o2,1,&contact.geom,sizeof(dContact))) {
		dJointID c = dJointCreateContact (e->physics,e->collisionJoints,&contact);
		dJointAttach (c,b1,b2);
	}
}
