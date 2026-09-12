#include "Renderer.h"
#include "ShaderProgram.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

void Renderer::BlendAlpha() const
{
    glEnable(GL_BLEND);
    // give blend rgba coffecients
    glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA );
    glBlendEquation(GL_ADD);
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const ShaderProgram& program) const
{
    program.Bind();
    vao.Bind();
    ibo.Bind();
   
    glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);

}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
