#include <iostream>

#include "Core/Application.h"

using namespace Lux;

int main()
{
    Lux::Application app;
    std::cout << app.get_string() << std::endl;

    //app.loop();
}