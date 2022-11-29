#pragma once

namespace Lux
{

#define Verify(__expr__) { if(!(__expr__)) __debugbreak(); }

#define TODO() __debugbreak()

#define CHECK(__expr__, __message__) { if(!(__expr__)){ Console::Fatal(__message__); exit(1); }}

}