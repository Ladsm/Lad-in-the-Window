#pragma once
#include <string>

//ansi color picker https://ladsm.neocities.org/colorpicker
struct WindowPalette {
    std::string Resize = "\033[95;45m";
    std::string Move = "\033[93;43m";
    std::string Focused = "\033[97;44m";
    std::string Unfocused = "\033[37;100m";
    std::string Body = "\033[38;2;0;0;0;48;2;192;192;192m";
    std::string Shadow = "\033[48;2;45;45;45m";
    WindowPalette() = default;
    WindowPalette(std::string x, std::string y, std::string z, std::string a, std::string b, std::string c) :
        Resize(x), Move(y), Focused(z), Unfocused(a), Body(b), Shadow(c) {}
};

struct WindowManagerPalette {
    std::string Wallpaper = "\033[48;5;30m";
    std::string BottomBar = "\033[97;104m";
    std::string BottomBarFocusedWindow = "\033[97;44m";
    WindowManagerPalette() = default;
    WindowManagerPalette(std::string x, std::string y, std::string z) :
        Wallpaper(x), BottomBar(y), BottomBarFocusedWindow(z) {}
};

