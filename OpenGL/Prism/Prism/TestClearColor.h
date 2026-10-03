#pragma once
#include "Test.h"

namespace Test
{
    class TestClearColor :  public Test
      
    {
        float color[4] = {0};
    public:
        TestClearColor();


        void OnUpdate() override;
        void OnRender() override;
        void OnImGuiRender() override;

       virtual ~TestClearColor();


    };

}


