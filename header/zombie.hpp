#pragma once
#include "sun.hpp"

class Zombie
{
public:
    Zombie(RenderWindow *_window_ptr);

    void render();
    virtual void update() = 0;
    void update_health(int projectile_damage);
    void change_is_in_snow_mode();
    void update_texture(string zombie_png, float scale_x, float scale_y, int width, int height);
    bool is_in_hit_moment();
    int get_pos_x() { return zombie_pos.x; }
    FloatRect get_rect() { return zombie_sprite.getGlobalBounds(); }
    int get_health() { return health; }
    bool get_in_snow_mode() { return is_in_snow_mode; }
    int get_row() { return row; }
    int get_damage() { return damage; }
    void make_stable() { is_moving = false; }
    bool get_is_moving() { return is_moving; }
    void set_in_motion() { is_moving = true; }
    int get_speed(){return speed;}
protected:
    Texture zombie_texture;
    Sprite zombie_sprite;
    RenderWindow *window_ptr;
    Vector2i zombie_pos;
    vector<int> zombie_data;
    Clock in_snow_clock;
    Clock hit_rate_clock;
    int health;
    int damage;
    int speed;
    int hit_rate;
    int row;
    bool is_in_snow_mode;
    bool is_moving;
};

class NormalZombie : public Zombie
{
public:
    NormalZombie(RenderWindow *_window_ptr, Vector2i _pos, int _row);
    
    void update();
private:
    int walking_zombie_animation_poses[46];
    int current_animation_rect_number = 0;
    Clock animation_clock;
    Clock zombie_clock;
};

class GiantZombie : public Zombie
{
public:
    GiantZombie(RenderWindow *_window_ptr, Vector2i _pos, int _row);
    
    void update();
private:
    int walking_zombie_animation_poses[33];
    int current_animation_rect_number = 0;
    Clock animation_clock;
    Clock giant_zombie_clock;
};