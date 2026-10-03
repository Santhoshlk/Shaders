#include "TestClearColor.h"

#include <imgui.h>
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

    ImGui::ColorPicker4("Color Picker",color,ImGuiColorEditFlags_AlphaBar);

}

Test::TestClearColor::~TestClearColor()
{
    for (auto& i : color)
    {
        i = 0.f;
    }
    glClearColor(0.f,0.f,0.f,0.f);
    glClear(GL_COLOR_BUFFER_BIT);
}
