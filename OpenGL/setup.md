u are writing them all in same line

Ah my bad bro 🥰 The multi-line box in VS (dropdown → <Edit...>) takes one per line — paste like this:

1. Additional Include Directories

D:\ExternalLibraries\GLEW\include
D:\ExternalLibraries\GLFW\include
D:\ExternalLibraries\GLMlibrary
2. Additional Library Directories

D:\ExternalLibraries\GLEW\lib\Release\x64
D:\ExternalLibraries\GLFW\lib-vc2022
3. Additional Dependencies

glew32.lib
glfw3.lib
opengl32.lib
4. DLL to copy into Prism's x64\Debug

D:\OpenGL\OpenGLLearning\x64\Debug\glew32.dll
5. main.cpp include order

cpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// the locations
1. C/C++ → General → Additional Include Directories
2. Linker → General → Additional Library Directories
3. Linker → Input → Additional Dependencies
4. Copy this file into Prism's x64\Debug folder