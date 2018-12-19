# physics-sandbox
Physics sandbox, based on Newton Game Dynamics, EntityX and OpenGL

How does it work?
Using Entity Component System (based on EntityX) you can create “entities” with components “Transform”, “PhysBody”, “Model”, which are responsible for:
- Transform (position and orientation of the object in space);
- The physical model based on the physics engine NewtonGameDynamics;
- A visual model, which consists of mesh and material (shader with color);

What you can do there?
Create physical objects. By default, gravity 9.81 m/s2 acts on all physical objects. An object with zero mass is interpreted as static. Make a static surface (Earth), put a ball on it, apply force to it (PhysBody :: applyForce) - the ball will roll on the surface ❤️
You can explode balls like this https://www.youtube.com/watch?v=JwVJS8qGg00 .
There are no input manager, it responds only to Esc - exit and F - switching fullscreen/windowed mode.

In general about classes:
- Transform - 4x4 matrix wrapper, identity by default. Controlled by the PhysBody component, if it exists in this entity.
- Mesh - 3D object stored on the GPU;
- MeshData - a class that generates arrays for the mesh, right now it generates a ball, an icosahedron, soon it will be possible to import .obj meshes;
- Window - window;
- Model - a visual model that includes a 3D model and material.
- Material - the material of Model.
- Shader - compiler and switcher of shaders.
- ShaderManager - singleton, that compiles all shaders.
- Camera - the camera, it stores the View and Projection matrix.
- PhysBody - physical body, wrapper on Newton Game Dynamics.
- Game - a class with game logic.

Linked libraries from Ubuntu PPA:
GLM - vector math;
GLEW - OpenGL wrapper;
GLFW - window ad input;
STL library.

Additional libraries:
- EntityX https://github.com/alecthomas/entityx
- NewtonGameDynamics (physics engine) https://github.com/MADEAPPS/newton-dynamics/

Feel free to use it and contact me.
