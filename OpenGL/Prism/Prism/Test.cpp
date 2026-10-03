#include "Test.h"
#include "iostream"

Test::Test::Test()
{
    std::cout << "Base Test Constructor" << std::endl;
}

void Test::Test::OnUpdate()
{

}

void Test::Test::OnRender()
{
}

void Test::Test::OnImGuiRender()
{
}

Test::Test::~Test()
{
    std::cout << "Base Test Destructor" << std::endl;
}
