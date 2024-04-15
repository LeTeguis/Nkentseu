#include <iostream>
#include <Unitest/Unitest.h>
#include "Logger/Log.h"
#include "Vector2Test.h"

#include "Nkentseu/Nkentseu.h"

void MyTest(const std::string& name);

#define test_macros(x) #x

nkentseu::int32 Main(const nkentseu::ARGV& argv) {
    //nkentseu::Unitest::Instance().PrintPassedDetails(false);
    //UNITEST_REGISTRY(UnitestRegisterFunction(MyTest, "teste testing"));
    nkentseu::Vector2Test vector2Test;
    return nkentseu::RunTest();
}

void MyTest(const std::string& name)
{
    UNITEST_TRUE(5 == 2, "");
}
