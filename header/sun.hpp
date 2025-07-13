#pragma once
#include "utilities.hpp"

class Sun
{
public:
    Sun(RenderWindow *_window_ptr, Vector2i _pos,bool _in_motion);
    void render();
    void update(int speed);
    void generate_final_pos_y();
    Vector2i get_pos(){return pos;}
    bool finished_waiting();
    bool get_in_motion(){return in_motion;}
    Sprite get_sprite(){return sprite;}
private:
    int final_pos_y;
    Texture texture;
    Sprite sprite;
    Vector2i pos;
    RenderWindow *window_ptr;
    bool in_motion;
    Clock sun_clock;
    Clock wait_clock;
};