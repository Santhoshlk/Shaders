#include "Test.h"
#include "iostream"

Test::Test::Test()
{
    std::cout << "Base Test Constructor" << std::endl;
}

Test::Test::~Test()
{
    std::cout << "Base Test Destructor" << std::endl;
}
