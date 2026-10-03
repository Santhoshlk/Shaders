#include "TestMenu.h"

#include <imgui.h>

namespace Test
{
    TestMenu::TestMenu(Test*& CurrentTest) : m_Test(CurrentTest)
    {
        m_Test = this;
    }

    void TestMenu::OnImGuiRender()
    {
        for (const auto& test : m_Tests)
        {
            if (ImGui::Button(test.first.c_str()))
                m_Test = test.second();
        }
    }




   TestMenu::~TestMenu()
    {

    }

}

