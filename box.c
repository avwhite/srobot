#include"box.h"
#include<GL/glew.h>
#include<stdlib.h>
#include"linmath.h"

Box createBox(float w, float h, float d)
{
	w /= 2;
	h /= 2;
	d /= 2;

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

	GLfloat g_color_buffer_data[12*3*3];
	int v;
	for (v = 0; v < 12*3 ; v++){
		g_color_buffer_data[3*v+0] = 0.0;
		g_color_buffer_data[3*v+1] = 0.5;
		g_color_buffer_data[3*v+2] = 0.0;
	}

	Box box;
	box.x = 0;
	box.y = 0;
	box.z = 0;

	GLuint colorbuffer;
	glGenBuffers(1, &(box.color_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, box.color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &(box.vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, box.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	return box;
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
	 
	glDrawArrays(GL_TRIANGLES, 0, 12*3);
	 
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(3);
}

void getBoxModelMatrix(Box *box, mat4x4 m)
{
	mat4x4_translate(m, box->x, box->y, box->z);
}

void deleteBox(Box* box)
{
	glDeleteBuffers(1, &(box->vertex_buffer));
	glDeleteBuffers(1, &(box->color_buffer));
}

EntityList createEntityList(int initCap)
{
	EntityList e;
	e.entities = malloc(sizeof(Box)*initCap);
	e.length = 0;
	e.cap = initCap;
	return e;
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

void deleteEntityList(EntityList *e)
{
	int i;
	for (i = 0; i < e->length; ++i) {
		deleteBox(&(e->entities[i]));
	}
	free(e->entities);
	e->cap = 0;
	e->length = 0;
}
