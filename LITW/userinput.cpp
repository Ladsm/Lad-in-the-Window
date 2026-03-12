#include <iostream>
#include "userinput.hpp"
#if defined(_WIN32)
#include <conio.h>
#endif
#if defined(__linux__)
#include <unistd.h>
#include <termios.h>
int getchThred() {
    struct termios old_settings, new_settings;
    int ch;
    tcgetattr(STDIN_FILENO, &old_settings);
    new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
    return ch;
}
#endif
/*
+-------+   0   1   2   3   4   5   6   7   8   9
|   2   |  10  11  12  13  14  15  16  17  18  19
|-1   -2|
|   1   | Diagram of GetPlayerInput() for future me
+-------+
*/
InputType GetPlayerInput() {
    int ch =
#ifdef _WIN32
        _getch();
#else
        getchThred();
#endif
#ifdef _WIN32
    if (ch == 13) return InputType::Enter;
    if (ch == 27) return InputType::Escape;
    if (ch == 0 || ch == 0xE0) {
        switch (_getch()) {
        case 72: return InputType::MoveUp;
        case 80: return InputType::MoveDown;
        case 75: return InputType::MoveLeft;
        case 77: return InputType::MoveRight;
        }
        return InputType::None;
    }
#else
    if (ch == 10 || ch == 13) return InputType::Enter;
    if (ch == 27) {
        int next = getchThred();
        if (next == 91) {
            switch (getchThred()) {
            case 'A': return InputType::MoveUp;
            case 'B': return InputType::MoveDown;
            case 'C': return InputType::MoveRight;
            case 'D': return InputType::MoveLeft;
            }
            return InputType::None;
        }
        return InputType::Escape;
    }
#endif
    if (ch >= '0' && ch <= '9') {
        return static_cast<InputType>(
            static_cast<int>(InputType::Top0) + (ch - '0')
            );
    }
    switch (ch) {
    case 'w': case 'W': return InputType::MoveUp;
    case 's': case 'S': return InputType::MoveDown;
    case 'a': case 'A': return InputType::MoveLeft;
    case 'd': case 'D': return InputType::MoveRight;
    case 'e': case 'E': return InputType::E;
    case 'x': case 'X': return InputType::X;
    case 'q': case 'Q': return InputType::Q;
    }
    return InputType::None;
}
//simpler key reader, not made for menuing
#if defined(_WIN32)
int readKey() {
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        int ch2 = _getch();
        return 1000 + ch2;
    }
    return ch;
}
#else
int readKey() {
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int ch = getchar();
    if (ch == 27) {
        if (getchar() == '[') {
            ch = 1000 + getchar();
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif