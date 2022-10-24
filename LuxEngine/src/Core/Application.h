#pragma once

#include <string>

#include "Window.h"

namespace Lux
{
class Application
{
public:
    std::string get_string();

    void loop();

    void terminate();

private:

    Window m_Window;


    bool m_Running = true;

};

}