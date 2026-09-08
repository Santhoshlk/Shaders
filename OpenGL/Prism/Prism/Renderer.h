#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexArray;
class IndexBuffer;
class ShaderProgram;

class Renderer
{
public:

    // make functions to abstract all the gl calls basically
    // we have at making clear and draw  blend alpha update class as per ur need

    void BlendAlpha() const;

    // generally we pass in materials instead of shader
    void Draw(const VertexArray& vao, const IndexBuffer& ibo, const ShaderProgram& program) const;

    void Clear() const;

};

