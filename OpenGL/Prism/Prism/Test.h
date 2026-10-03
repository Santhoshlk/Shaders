#pragma once


namespace Test
{
    class Test
    {
    public:
        Test();

        Test(const Test& t) = delete;
        void operator=(const Test& t) = delete;

        virtual void OnUpdate();
        virtual void OnRender();
        virtual void OnImGuiRender();

        virtual ~Test();

    };
}


