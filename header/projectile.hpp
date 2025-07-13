#pragma once
#include "zombie.hpp"

class Projectile
{
public:
    Projectile(RenderWindow *_window_ptr, string projectile_type_png);
    void render();
    void update();
    Vector2i get_pos() { return pos; }
    string get_name() { return projectile_name; }
    FloatRect get_rect() { return projectile_sprite.getGlobalBounds(); }
    int get_damage() { return damage; }
    string get_projectile_name() { return projectile_name; }
    void set_zombie_collision(Zombie *_nearest_zombie) { nearest_zombie = _nearest_zombie; }
    void set_time_for_watermelon_motion(double time_for_watermelon_motion) { Time_for_watermelon_motion = time_for_watermelon_motion; }
    int get_speed() { return speed; }
    bool is_end_collision_time() {return Time_for_watermelon_motion < motion_watermelon_clock.getElapsedTime().asSeconds();}
    Zombie* get_nearest_zombie() {return nearest_zombie;}

protected:
    int damage;
    int speed;
    double Time_for_watermelon_motion = 0;
    int initial_pos_x;
    int initial_pos_y;
    vector<int> file_data;
    string projectile_name;
    RenderWindow *window_ptr;
    Texture projectile_texture;
    Sprite projectile_sprite;
    Vector2i pos;
    Clock projectile_clock;
    Clock motion_watermelon_clock;
    Zombie *nearest_zombie;
};

class Regular_projectile : public Projectile
{
public:
    Regular_projectile(RenderWindow *_window_ptr, Vector2i _pos, string projectile_type_png);

private:
};

class Snow_projectile : public Projectile
{
public:
    Snow_projectile(RenderWindow *_window_ptr, Vector2i _pos, string projectile_type_png);

private:
};

class Watermelon_projectile : public Projectile
{
public:
    Watermelon_projectile(RenderWindow *_window_ptr, Vector2i _pos, string projectile_type_png);

private:
};