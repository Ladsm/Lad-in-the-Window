
#include <LITW.hpp>
#include <memory>

int main() {
    WindowManager wm;
    //The window manager, stores windows.
    auto win = std::make_shared<Window>("Hello World", 30, 10);
    //creates a window, title "Hello World", with width 30 and height 10.
    win->AddWidget(std::make_unique<Label>(2, 2, "Hello World"));
    //adds a label widget at width 2 and height 2.
    wm.AddWindow(win); //adds window to window vector.
    wm.Run(); // runs the window manager loop(main loop).
    wm.exit(1);
    /* no natral exit, returns 1 due there being no exit state.
       use wm.exit because it reverts console state from the LITW state to the old state. */
    return 0;
}

