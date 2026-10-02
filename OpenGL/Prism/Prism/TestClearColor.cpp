#include "TestClearColor.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GL/glew.h>


Test::TestClearColor::TestClearColor()
{

}

void Test::TestClearColor::OnUpdate()
{

}

void Test::TestClearColor::OnRender()
{

    glClearColor(color[0],color[1],color[2],color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Test::TestClearColor::OnImGuiRender()
{
    ImGui::Begin("Test Clear Color");

    ImGui::ColorPicker4("Color Picker",color,ImGuiColorEditFlags_AlphaBar);

    ImGui::End();



}

Test::TestClearColor::~TestClearColor()
{

}
