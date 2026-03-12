#pragma once
#pragma once
enum class InputType {
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Top0,
    Top1,
    Top2,
    Top3,
    Top4,
    Top5,
    Top6,
    Top7,
    Top8,
    Top9,
    Enter,
    Escape,
    E,
    X,
    Q,
    C,
    R
};
InputType GetPlayerInput();
int readKey();