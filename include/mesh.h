#ifndef MESH_H
#define MESH_H

class MeshData;
class Mesh {
    typedef unsigned int GLuint;
    typedef int GLsizei;
    GLuint m_VAO = 0, m_VBO = 0, m_EBO = 0;
    GLsizei m_IndexCount = 0;

  public:
    Mesh(const MeshData &data);
    Mesh(MeshData &&data) noexcept;
    void setMeshData(const MeshData &data);
    void render() const;
    ~Mesh();
    Mesh(Mesh &&rhc);

    // define mesh as no-copyable
    Mesh(const Mesh &rhc) = delete;
};

#endif // MESH_H
