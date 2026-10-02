#pragma once


namespace Test
{
    class Test
    {
    public:
        Test();

        Test(const Test& t) = delete;
        void operator=(const Test& t) = delete;

        virtual void OnUpdate() = 0;
        virtual void OnRender() = 0;
        virtual void OnImGuiRender() = 0;

        virtual ~Test();

    };
}


