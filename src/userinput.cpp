#include <iostream>
#include "userinput.hpp"
#if defined(_WIN32)
#include <conio.h>
#include <windows.h>
#else
#include <unistd.h>
#include <termios.h>
#endif
#ifndef _WIN32
int getch() {
    return getchar();
}
#endif
int g_mouseX = 0;
int g_mouseY = 0;
int getMouseX() { return g_mouseX; }
int getMouseY() { return g_mouseY; }
InputType GetInput() {
#if defined(_WIN32)
    static bool consoleInitialized = false;
    static DWORD originalMode = 0;
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    if (!consoleInitialized) {
        if (GetConsoleMode(hIn, &originalMode)) {
            DWORD mode = originalMode;
            mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
            mode |= ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
            SetConsoleMode(hIn, mode);
        }
        consoleInitialized = true;
    }
    INPUT_RECORD ir;
    DWORD read = 0;
    static DWORD prevMouseButtons = 0;
    while (true) {
        if (!ReadConsoleInput(hIn, &ir, 1, &read)) {
            int ch = _getch();
            if (ch == 9) return InputType::Tab;
            if (ch == 13) return InputType::Enter;
            if (ch == ' ') return InputType::Space;
            if (ch == 27) return InputType::Escape;
            if (ch == 0 || ch == 224) {
                int ex = _getch();
                if (ex >= 59 && ex <= 68) return static_cast<InputType>((int)InputType::F1 + (ex - 59));
                if (ex == 133) return InputType::F11;
                if (ex == 134) return InputType::F12;
            }
            if (ch >= '0' && ch <= '9') return static_cast<InputType>(static_cast<int>(InputType::Top0) + (ch - '0'));
            switch (ch) {
            case 'q': case 'Q': return InputType::Q;
            case 'w': case 'W': return InputType::W;
            case 'e': case 'E': return InputType::E;
            case 'r': case 'R': return InputType::R;
            case 't': case 'T': return InputType::T;
            case 'y': case 'Y': return InputType::Y;
            case 'u': case 'U': return InputType::U;
            case 'i': case 'I': return InputType::I;
            case 'o': case 'O': return InputType::O;
            case 'p': case 'P': return InputType::P;
            case 'a': case 'A': return InputType::A;
            case 's': case 'S': return InputType::S;
            case 'd': case 'D': return InputType::D;
            case 'f': case 'F': return InputType::F;
            case 'g': case 'G': return InputType::G;
            case 'h': case 'H': return InputType::H;
            case 'j': case 'J': return InputType::J;
            case 'k': case 'K': return InputType::K;
            case 'l': case 'L': return InputType::L;
            case 'z': case 'Z': return InputType::Z;
            case 'x': case 'X': return InputType::X;
            case 'c': case 'C': return InputType::C;
            case 'v': case 'V': return InputType::V;
            case 'b': case 'B': return InputType::B;
            case 'n': case 'N': return InputType::N;
            case 'm': case 'M': return InputType::M;
            default: return InputType::None;
            }
        }
        if (ir.EventType == KEY_EVENT) {
            auto& ke = ir.Event.KeyEvent;
            if (!ke.bKeyDown) continue;
            if (ke.wVirtualKeyCode == VK_TAB) return InputType::Tab;
            if (ke.wVirtualKeyCode >= VK_F1 && ke.wVirtualKeyCode <= VK_F12) {
                return static_cast<InputType>((int)InputType::F1 + (ke.wVirtualKeyCode - VK_F1));
            }
            char ch = (char)ke.uChar.AsciiChar;
            if (ch == 13) return InputType::Enter;
            if (ch == ' ') return InputType::Space;
            if (ch == 27) return InputType::Escape;
            switch (ke.wVirtualKeyCode) {
            case VK_UP:    return InputType::MoveUp;
            case VK_DOWN:  return InputType::MoveDown;
            case VK_LEFT:  return InputType::MoveLeft;
            case VK_RIGHT: return InputType::MoveRight;
            }
            if (ch >= '0' && ch <= '9') return static_cast<InputType>(static_cast<int>(InputType::Top0) + (ch - '0'));
            switch (ch) {
            case 'q': case 'Q': return InputType::Q;
            case 'w': case 'W': return InputType::W;
            case 'e': case 'E': return InputType::E;
            case 'r': case 'R': return InputType::R;
            case 't': case 'T': return InputType::T;
            case 'y': case 'Y': return InputType::Y;
            case 'u': case 'U': return InputType::U;
            case 'i': case 'I': return InputType::I;
            case 'o': case 'O': return InputType::O;
            case 'p': case 'P': return InputType::P;
            case 'a': case 'A': return InputType::A;
            case 's': case 'S': return InputType::S;
            case 'd': case 'D': return InputType::D;
            case 'f': case 'F': return InputType::F;
            case 'g': case 'G': return InputType::G;
            case 'h': case 'H': return InputType::H;
            case 'j': case 'J': return InputType::J;
            case 'k': case 'K': return InputType::K;
            case 'l': case 'L': return InputType::L;
            case 'z': case 'Z': return InputType::Z;
            case 'x': case 'X': return InputType::X;
            case 'c': case 'C': return InputType::C;
            case 'v': case 'V': return InputType::V;
            case 'b': case 'B': return InputType::B;
            case 'n': case 'N': return InputType::N;
            case 'm': case 'M': return InputType::M;
            default: return InputType::None;
            }
        }
        else if (ir.EventType == MOUSE_EVENT) {
            auto& me = ir.Event.MouseEvent;
            g_mouseX = me.dwMousePosition.X;
            g_mouseY = me.dwMousePosition.Y;
            DWORD btns = me.dwButtonState;
            if (me.dwEventFlags == MOUSE_MOVED) {
                prevMouseButtons = btns;
                return InputType::MouseMove;
            }
            DWORD leftMask = FROM_LEFT_1ST_BUTTON_PRESSED;
            bool prevLeft = (prevMouseButtons & leftMask) != 0;
            bool curLeft = (btns & leftMask) != 0;
            prevMouseButtons = btns;
            if (!prevLeft && curLeft) return InputType::MouseLeftDown;
            if (prevLeft && !curLeft) return InputType::MouseLeftUp;
            return InputType::MouseMove;
        }
    }
#else
    static struct termios originalTermios;
    static bool consoleInitialized = false;
    if (!consoleInitialized) {
        if (tcgetattr(STDIN_FILENO, &originalTermios) == 0) {
            consoleInitialized = true;
        }
        struct termios raw = originalTermios;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_iflag &= ~(IXON | ICRNL);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        std::cout << "\033[?1002h\033[?1006h" << std::flush;
        consoleInitialized = true;
    }
    int ch = getch();
    if (ch == 9) return InputType::Tab;
    if (ch == 27) {
        int n1 = getch();
        if (n1 == '[') {
            int n2 = getch();
            if (n2 == '<') {
                int cb = 0, cx = 0, cy = 0;
                int c;
                while ((c = getch()) >= '0' && c <= '9') { cb = cb * 10 + (c - '0'); }
                if (c == ';') {
                    while ((c = getch()) >= '0' && c <= '9') { cx = cx * 10 + (c - '0'); }
                    if (c == ';') {
                        while ((c = getch()) >= '0' && c <= '9') { cy = cy * 10 + (c - '0'); }
                        if (c == 'M' || c == 'm') {
                            g_mouseX = cx > 0 ? cx - 1 : 0;
                            g_mouseY = cy > 0 ? cy - 1 : 0;
                            bool isMotion = (cb & 32) != 0;
                            int button = cb & 0b11;
                            if (isMotion) return InputType::MouseMove;
                            if (c == 'M') {
                                if (button == 0) return InputType::MouseLeftDown;
                                return InputType::MouseMove;
                            }
                            else {
                                if (button == 0 || button == 3) return InputType::MouseLeftUp;
                                return InputType::MouseMove;
                            }
                        }
                    }
                }
            }
            else {
                if (n2 == 'A') return InputType::MoveUp;
                if (n2 == 'B') return InputType::MoveDown;
                if (n2 == 'C') return InputType::MoveRight;
                if (n2 == 'D') return InputType::MoveLeft;
                if (n2 >= '0' && n2 <= '9') {
                    int num = n2 - '0';
                    int next;
                    while ((next = getch()) >= '0' && next <= '9') num = num * 10 + (next - '0');
                    if (num == 15) return InputType::F5;
                    if (num == 17) return InputType::F6;
                    if (num == 18) return InputType::F7;
                    if (num == 19) return InputType::F8;
                    if (num == 20) return InputType::F9;
                    if (num == 21) return InputType::F10;
                    if (num == 23) return InputType::F11;
                    if (num == 24) return InputType::F12;
                }
            }
        }
        else if (n1 == 'O') {
            int n2 = getch();
            if (n2 == 'P') return InputType::F1;
            if (n2 == 'Q') return InputType::F2;
            if (n2 == 'R') return InputType::F3;
            if (n2 == 'S') return InputType::F4;
        }
        return InputType::Escape;
    }
    if (ch == 10 || ch == 13) return InputType::Enter;
    if (ch == ' ') return InputType::Space;
    if (ch >= '0' && ch <= '9') return static_cast<InputType>(static_cast<int>(InputType::Top0) + (ch - '0'));
    switch (ch) {
    case 'q': case 'Q': return InputType::Q;
    case 'w': case 'W': return InputType::W;
    case 'e': case 'E': return InputType::E;
    case 'r': case 'R': return InputType::R;
    case 't': case 'T': return InputType::T;
    case 'y': case 'Y': return InputType::Y;
    case 'u': case 'U': return InputType::U;
    case 'i': case 'I': return InputType::I;
    case 'o': case 'O': return InputType::O;
    case 'p': case 'P': return InputType::P;
    case 'a': case 'A': return InputType::A;
    case 's': case 'S': return InputType::S;
    case 'd': case 'D': return InputType::D;
    case 'f': case 'F': return InputType::F;
    case 'g': case 'G': return InputType::G;
    case 'h': case 'H': return InputType::H;
    case 'j': case 'J': return InputType::J;
    case 'k': case 'K': return InputType::K;
    case 'l': case 'L': return InputType::L;
    case 'z': case 'Z': return InputType::Z;
    case 'x': case 'X': return InputType::X;
    case 'c': case 'C': return InputType::C;
    case 'v': case 'V': return InputType::V;
    case 'b': case 'B': return InputType::B;
    case 'n': case 'N': return InputType::N;
    case 'm': case 'M': return InputType::M;
    }
    return InputType::None;
#endif
}
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
