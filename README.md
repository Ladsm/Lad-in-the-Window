<div align="center">
	<img src="./images/LITWLogo.png" width="150" height="150" alt="LITW Logo">
	<h1>Lad In The Window (LITW)<h5>Version: 0.9.6</h5></h1>
	<p><strong>A TUI that looks like a GUI.</strong></p>
	<h2>Demo</h2>
	<img src="./images/LITWDemo.png" alt="Demo Image" width="45%">
	<img src="./images/LITWDemo.gif" alt="Demo Gif" width="45%">
</div>

## Core Ideas

*   **Window:** An object that holds widgets.
*   **Widgets:** An object inside a window. The base of interaction in LITW.

LITW is designed to be completely dependency-free. You only need a standard C++ compiler and CMake to get started.

#### Prerequisites
*   **Windows:** MSVC (Visual Studio) or MinGW
*   **Linux/macOS:** GCC or Clang
*   **Build System:** CMake (3.10 or higher recommended)

#### Build Instructions
```sh
git clone https://github.com/ladsm/Lad-in-the-Window.git
cd Lad-in-the-Window
mkdir build && cd build
cmake ..
cmake --build .

#The demo is here:
# Linux/macOS:
./LITWDemo
# Windows:
.\Debug\LITWDemo.exe
```

> *Note*  
>  For the best experience, use a modern terminal like [Windows Terminal](https://github.com/microsoft/terminal), [Alacritty](https://github.com/alacritty/alacritty), or [kitty](https://github.com/kovidgoyal/kitty) that supports 24-bit color, SGR mouse tracking and nerd fonts.

> TTY Only  
> For TTY only, use [KMSCON](https://github.com/kmscon/kmscon) for 24 bit color.

<div align="center">

## Controls

| Category           | Key(s)                         | Action                        |
| ------------------ | ------------------------------ | ----------------------------- |
| Windows            | 1 - 9                          | Focus window by index         |
|                    | Escape                         | Cycle window focus            |
| Window Operations  | Q / X                          | Minimize / Close              |
|                    | Z                              | Maximize / Restore window     |
|                    | E / R                          | Move / Resize mode            |
| Navigation         | WASD / Arrows                  | Move cursor / selection       |
| System             | C                              | Open / Close Start Menu       |
|                    | F12                            | Exit                          |
| Mouse              | Left Click                     | Focus window                  |
|                    | Left Drag                      | Move window                   |
|                    | Left Click on Title Bar        | Drag to move                  |
|                    | Left Click on Window Corners   | Drag to resize                |
| Text Input Widgets | Type while focused             | Add characters                |
|                    | Backspace                      | Remove characters             |
|                    | Enter                          | Confirm input / finish typing |
| Scrollable Widgets | Space                          | Scroll down                   |
|                    | Tab                            | Scroll up                     |

</div>
<div align="center">
	<h5>Links</h5>
	<p>
	<a href="https://ladsm.neocities.org/LITW/">Website</a>,
	<a href="https://github.com/Ladsm/Lad-in-the-Window">GitHub</a>
	</p>
</div>
