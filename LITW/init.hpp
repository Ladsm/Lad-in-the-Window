#pragma once
#include <iostream>
#if defined(_WIN32)
#include <Windows.h>
#endif
#if defined(__linux__)
#include <unistd.h>
#include <termios.h>
#endif

inline void init() {
	// make it look nice
#if defined(_WIN32)
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut != INVALID_HANDLE_VALUE) {
		DWORD mode = 0;
		if (GetConsoleMode(hOut, &mode)) {
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(hOut, mode);
		}
	}
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif
	std::cout << "\033[?25l";
	std::cout << "\033[?1000h";
	std::cout << "\033[?1002h";
	std::cout << "\033[?1006h";
	std::cout.flush();
}