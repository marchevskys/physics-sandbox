#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <vector>

class Shader;
class ShaderManager {
    std::vector<Shader> shaders;
    ShaderManager();
    ~ShaderManager();

  public:
    static ShaderManager *get() {
        static ShaderManager inst;
        return &inst;
    }
    const Shader *flatShader() const;
    const std::vector<Shader> *getAll3DShaders() { return &shaders; }
    void setViewMatricesForAllShaders(const double *mat);
    void setProjectionMatricesForAllShaders(const float *mat);
    void setViewPos(const double *dir);
};
#endif // SHADERMANAGER_H
