#include "Core/Application.h"

std::string Lux::Application::get_string() {
    return "Application is starting";
}

void Lux::Application::loop() {

    while(m_Running)
    {

    }
}

void Lux::Application::terminate() {
    m_Running = false;
}
