#pragma once

#include <cstring>
#include <iostream>

#include "glew.h"




class Shader {
    enum Type {
        VERTEX,
        FRAGMENT
    };
    
    private:
        GLuint shaderHandle;
        Shader::Type type;

    public:
        Shader(GLchar* source, Type type) {
            this->shaderHandle = glCreateShader(type == VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
            GLint length = (GLint) std::strlen(source);
            glShaderSource(this->shaderHandle, 1, &source, &length);
            glCompileShader(this->shaderHandle);

            // error log
			GLint shaderLogSize;
			glGetShaderiv(this->shaderHandle, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->shaderHandle, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}
        }

        ~Shader() {
            glDeleteShader(this->shaderHandle);
        }

        GLuint getHandle() {
            return this->shaderHandle;    
        }

        Type getType() {
            return this->type;
        }
};