#include <GL/glew.h>
#include <GL/freeglut.h>
#include <chrono>
#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

// TAKEN FROM LABS

// Used to print shader info log
void print_shader_info_log(unsigned int shader_obj) {
	int len = 0;
	glGetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &len);
	if (len>1) {
		char* log = new char[len];
		int s_len = 0;
		glGetShaderInfoLog(shader_obj, len, &s_len, log);
		fprintf(stderr, "%s", log);
		delete[] log;
	}
}

// Used to read from a shader file named filename
char* read_shader_source(const char* filename) {
	char* buffer = NULL;

	if (filename==NULL)
		return buffer;

	FILE* fp = fopen(filename, "r");
	if (fp!=NULL) {
		struct stat status_buf;
		stat(filename, &status_buf); // find out how big it is

		buffer = new char[status_buf.st_size+1];
		// read in the file
		fread(buffer, 1, status_buf.st_size, fp); 
		buffer[status_buf.st_size] = '\0'; // null terminate it

		fclose(fp);
	}
	else {
		fprintf(stderr, "Failed to open shader file %s for reading\n", filename);
		exit(1);
	}

	return buffer;
}