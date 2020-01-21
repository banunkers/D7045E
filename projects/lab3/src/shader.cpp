#include "shader.h"

Shader::Shader(GLchar* source, Type type) {
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

Shader::~Shader() {
    glDeleteShader(this->shaderHandle);
}

GLuint Shader::getHandle() {
    return this->shaderHandle;    
}

Shader::Type Shader::getType() {
    return this->type;
}