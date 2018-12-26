#include "meshmanager.h"
#include "mesh.h"
#include "meshdata.h"

MeshManager::MeshManager() {
    primitives.reserve(10);
    MeshData sphereModel;
    sphereModel.addSphere(10);
    primitives.emplace_back(sphereModel);
}

MeshManager::~MeshManager() {
    //DLOG("Meshes destroyed");
}

const Mesh *MeshManager::Sphere() {
    return &primitives[0];
}
