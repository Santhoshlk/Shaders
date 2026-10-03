#pragma once
#include "Test.h"
#include <vector>
#include <string>
// declared in functional
#include <functional>


namespace Test
{

    class TestMenu :public Test
    {
        Test*& m_Test;
        std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;


    public:
        TestMenu(Test*& CurrentTest);

        // no copy constructors
        TestMenu(const TestMenu& menu) = delete;
        void  operator=(const TestMenu& menu) = delete;

        void OnImGuiRender() override;

        template<typename T>
        void RegisterTests(const std::string& button_name);

      virtual  ~TestMenu();
        

    };

    template <typename T>
    void  TestMenu::RegisterTests(const std::string& button_name)
    {

        m_Tests.push_back(std::make_pair(button_name, []() { return new T(); }));
    }
}
