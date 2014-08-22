#include <stdio.h>
#include <stdlib.h>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

char* filetobuf(char *file)
{
    FILE *fptr;
    long length;
    char *buf;
 
    fptr = fopen(file, "rb"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length+1); /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */
 
    return buf; /* Return the buffer */
}

GLuint LoadShaders(char * vertex_file_path, char * fragment_file_path)
{
	GLint Result = GL_FALSE;
	int InfoLogLength;

	 // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Compile vertex shader
	printf("Compiling Vertex Shader\n");
	char* VertexSource = filetobuf(vertex_file_path);
	glShaderSource(VertexShaderID, 1, (const GLchar**)&VertexSource , NULL);
    glCompileShader(VertexShaderID);

	// Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	if(Result == GL_FALSE) {
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		char* VertexShaderErrorMessage = (char *)malloc(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, &InfoLogLength, VertexShaderErrorMessage);
		printf("Vertex Shader complilation error\n");
		printf(VertexShaderErrorMessage);
		free(VertexShaderErrorMessage);
	}

	// Compile fragment shader
	printf("Compiling Fragment Shader\n");
	char* FragmentSource = filetobuf(fragment_file_path);
	glShaderSource(FragmentShaderID, 1, (const GLchar**)&FragmentSource , NULL);
    glCompileShader(FragmentShaderID);

	// Check fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	if(Result == GL_FALSE) {
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		char* FragmentShaderErrorMessage = (char *)malloc(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, &InfoLogLength, FragmentShaderErrorMessage);
		printf("Fragment shader compilation error\n");
		printf(FragmentShaderErrorMessage);
		free(FragmentShaderErrorMessage);
	}

    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	if(Result == GL_FALSE) {
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		char *ProgramErrorMessage = (char *)malloc(InfoLogLength);
		glGetProgramInfoLog(ProgramID, InfoLogLength, &InfoLogLength, ProgramErrorMessage);
		printf("Shader Linking error\n");
		printf(ProgramErrorMessage);
		free(ProgramErrorMessage);
	}
 
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
 
    return ProgramID;
}
