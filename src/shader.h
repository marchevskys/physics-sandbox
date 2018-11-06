#ifndef SHADER_H
#define SHADER_H
//#include <iostream>
//namespace std{
//class string;
//}
#include <vector>
class Shader {

    static std::vector<Shader *> shaders;
    static Shader *currentShader;
    int program;
    enum Type { GS,
                VS,
                PS };

  public:
    Shader(const char *vPath, const char *fPath, const char *gPath = nullptr);
    int create(const char *source, const Type type);

    void setBool(const char *name, bool value) const;
    void setInt(const char *name, int value) const;
    void setUint(const char *name, unsigned int value) const;
    void setFloat(const char *name, float value) const;
    void setVec2(const char *name, float x, float y) const;
    void setVec3(const char *name, float x, float y, float z) const;
    void setVec4(const char *name, float x, float y, float z, float w) const;
    void setMat4(const char *name, const double *const mat) const;
    void setMat4(int index, const double *const mat) const;
    void setMat4(int index, const float *const mat) const;

    void use() const;
    operator int() const;
    ~Shader();
};

#endif // SHADER_H
