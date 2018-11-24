//1. The prototype/interface header for this implementation (ie, the .h/.hh file that corresponds to this .cpp/.cc file).
//2. Other headers from the same project, as needed.
//3. Headers from other non-standard, non-system libraries (for example, Qt, Eigen, etc).
//4. Headers from other "almost-standard" libraries (for example, Boost)
//5. Standard C++ headers (for example, iostream, functional, etc.)
//6. Standard C headers (for example, cstdint, dirent.h, etc.)

#include "shader.h"

#include <GL/glew.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace Graphics {
void errorLog(const uint program, const char *programType) {
    char infoLog[512];
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    //glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &params);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        printf("ERROR::SHADER::%s::LINKING_FAILED\n", programType);
        std::cout << infoLog << std::endl;
    }
}

Shader::Shader(const char *vertexShaderSource, const char *pixelShaderSource, const char *geometryShaderSource) {
    std::cout << "SHADER CREATED" << std::endl;

    uint vertexShader = create(vertexShaderSource, Type::VS);
    uint fragmentShader = create(pixelShaderSource, Type::PS);
    uint geometryShader = 0;

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if (geometryShaderSource) {
        geometryShader = create(geometryShaderSource, Type::GS);
        glAttachShader(program, geometryShader);
    }
    glLinkProgram(program);
    //check for linking errors
    //printf("%d %d %d %d", vertexShader, fragmentShader, geometryShader, program);
    errorLog(program, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);
}

std::string pathToText(std::string path) {
    using namespace std;
    string result;
    string line;
    ifstream myfile(path);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            if (line.find("#include") != string::npos) {
                size_t left = line.find_first_of('"', 8);
                size_t right = line.find_first_of('"', left + 1);
                string fileName = line.substr(left + 1, right - left - 1);
                size_t lastAfterDiv = path.find_last_of('/');
                string fullPath = path.substr(0, lastAfterDiv + 1) + fileName;
                line = pathToText(fullPath);
            }
            result += line + '\n';
        }
        myfile.close();
    } else
        cout << "Could not open file " << path << endl;

    return result;
}
uint Shader::create(const char *sourcePath, const Type type) {
    using namespace std;

    string shaderCode = pathToText(string(sourcePath));

    unsigned int shader;
    string shaderTypeName;
    switch (type) {
    case Type::VS:
        shader = glCreateShader(GL_VERTEX_SHADER);
        shaderTypeName = "VERTEX";
        break;
    case Type::PS:
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        shaderTypeName = "FRAGMENT";
        break;
    case Type::GS:
        shader = glCreateShader(GL_GEOMETRY_SHADER);
        shaderTypeName = "GEOMETRY";
        break;
    }
    const char *ch = shaderCode.c_str();
    glShaderSource(shader, 1, &ch, nullptr);
    glCompileShader(shader);
    errorLog(shader, shaderTypeName.c_str());

    return shader;
}

void Shader::setBool(const char *name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name), static_cast<int>(value));
}

void Shader::setInt(const char *name, int value) const {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::setUint(const char *name, unsigned int value) const {
    glUniform1ui(glGetUniformLocation(program, name), value);
}

void Shader::setFloat(const char *name, float value) const {
    glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::setVec2(const char *name, float x, float y) const {
    glUniform2f(glGetUniformLocation(program, name), x, y);
}

void Shader::setVec3(const char *name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(program, name), x, y, z);
}

void Shader::setVec4(const char *name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(program, name), x, y, z, w);
}

void Shader::setMat4(const char *name, const double *const mat) const {
    float fMat[16];
    for (int i = 0; i < 16; i++)
        fMat[i] = static_cast<float>(mat[i]);
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, fMat);
}

void Shader::setMat4(int index, const double *const mat) const {

    float fMat[16];
    for (int i = 0; i < 16; i++)
        fMat[i] = static_cast<float>(mat[i]);
    glUniformMatrix4fv(index, 1, GL_FALSE, fMat);
}

void Shader::setMat4(int index, const float *const mat) const {
    glUniformMatrix4fv(index, 1, GL_FALSE, mat);
}

void Shader::use() const {
    //glUniform1f(glGetUniformLocation(program, "time"), (float)globalTime);

    glUseProgram(program);
}

Shader::operator uint() const {
    return program;
}

Shader::~Shader() {
    std::cout << "SHADER DESTROYED" << std::endl;
    glDeleteShader(program); // WHY IT WORKS
}
} // namespace Graphics
