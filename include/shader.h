#ifndef SHADER_H
#define SHADER_H

class Shader {
    typedef unsigned int GLuint;
    GLuint m_program;

  public:
    Shader(const char *vertex_file_path, const char *fragment_file_path);
    void use() const;
    void setMat4(int index, const double *const mat) const;
    void setVec(int index, const double *const vec) const;
    void setMat4(int index, const float *const mat) const;

    ~Shader();
};

#endif // SHADER_H
