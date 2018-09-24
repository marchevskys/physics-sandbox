#include "gameobject.h"
#include "physbody.h"

GameObject::GameObject() {
}

void GameObject::update() {
    body->getMatrix(matrix.data());
}

GameObject::~GameObject() {
}
