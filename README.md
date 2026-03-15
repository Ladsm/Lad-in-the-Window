
<img src="./images/LITWLogo.png" style="float: right; padding-left: 10px;"  width="100" height="100" alt="Alt text for the image">

# Lad In The Window

## Demos :

<img src="./images/LITWDemo.png" alt="Demo Image" width="500" height="300">

![Demo Gif](./images/LITWDemo.gif)

Lad In The Window(LITW), is a TUI that looks like a GUI.  

## Core Ideas

### Window

A window, is a object that holds widgets.

### Widgets

An object inside a window.  
The base of interaction in LITW.

## Build

LITW is designed to be completely dependency-free. You only need a standard C++ compiler and CMake to get started.

#### Prerequisites

 - Windows: MSVC (Visual Studio) or MinGW.
 - Linux/macOS: GCC or Clang.
 - Build System: CMake (3.10 or higher recommended).

##### Instructions

```sh
git clone https://github.com/ladsm/Lad-in-the-Window.git
cd Lad-in-the-Window
mkdir build
cd build
cmake ..
cmake --build .
#where binary is stored
#linux
build/LITW
#Windows
build\Debug\LITW.exe
```

> *Note*
>  For the best experience, use a modern terminal like Windows Terminal, Alacritty, or iTerm2 that supports 24-bit color and SGR mouse tracking.

## Controls

| Category  | Key(s) | 	Action |
| ------------- | ------------- | ------------- |
| Windows  | 1 - 9  | Focus window by index |
|    | Escape | Cycle window focus |
| Window Operations  | Q / X | Minimize / Close |
| | E / R |	Move / Resize mode|
| Navigation | WASD / Arrows | Move cursor/selection |
| System  | C | Open Start Menu |




