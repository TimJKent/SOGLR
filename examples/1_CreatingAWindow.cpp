#include "SOGLR/SOGLR.hpp"

int main()
{
    SOGLR::Window window(800, 600, "1_CreatingAWindow");

    while (!window.ShouldClose())
    {
        window.BeginFrame();
        window.EndFrame();
    }

    return 0;
}