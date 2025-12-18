#pragma once

enum ObjectType{
    INPUT,
    GATE,
    WIRE,
    LIGHT,
    NONE
};

constexpr int DRAG_THRESHOLD = 4;
constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;
constexpr int INPUT_RADIUS = 10;
constexpr int LIGHT_RADIUS = 10;
constexpr int PIN_RADIUS = 7;
