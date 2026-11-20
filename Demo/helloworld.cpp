#include <LITW/LITW.hpp>

int main() {
    litw::WindowManager wm;
    //The window manager, stores windows.
    auto win = litw::makeSharedWindow<litw::Window>("Hello World", 30, 10);
    //creates a window, title "Hello World", with width 30 and height 10.
    win->Add<litw::Label>(2, 2, "Hello World");
    //adds a label widget at width 2 and height 2.
    wm.AddWindow(win); //adds window to window vector.
    wm.Run(); // runs the window manager loop(main loop).
    wm.exit(1);
    /*  exit 1 because it would not normaly exit like this. 
        It would only exits with F12, that just exits, not break the run loop. */
    return 0;
}

