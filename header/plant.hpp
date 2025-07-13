#pragma once
#include "projectile.hpp"

class Plant
{
public:
    Plant(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos);
    virtual void render(int bg_pos_x, int bg_pos_y) = 0;
    virtual void set_normal_plant_png() = 0;
    virtual void set_cool_downed_plant_png() = 0;
    virtual void update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row, vector<Sun *> &suns,Zombie* _zombie_to_be_collided) = 0;
    void change_pos(int x, int y);
    void update_cooldown();
    Vector2i get_pos() { return pos; }
    Sprite get_plant_sprite() { return plant_sprite; }
    FloatRect get_rect() { return plant_sprite.getGlobalBounds(); }
    void update_health(int damaged){health-=damaged;}
    string get_plant_name() { return plant_name; }
    int get_row() { return row; }
    void start_timer(){plantClock.restart();}
    bool get_is_in_cooldown() { return is_in_cooldown; }
    void change_is_in_cooldown() { is_in_cooldown = true; }
    int get_health(){return health;}
    int get_price(){return price;}
    Clock get_cooldown_clock(){return plantClock;}
    int get_cooldown(){return cooldown;}
protected:
    Texture plant_texture;
    Sprite plant_sprite;
    int health;
    string plant_name;
    string plant_png_path;
    int price;
    int cooldown;
    int hit_rate;
    bool is_in_cooldown;
    int row;
    RenderWindow *window_ptr;
    Clock plantClock;
    Vector2i pos;
};

class Walnut : public Plant
{
public:
    Walnut(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos);
    void set_normal_plant_png() { plant_png_path = WALLNUT_PNG; }
    void set_cool_downed_plant_png() { plant_png_path = WALLNUT_COOLDOWN_PNG; }
    void render(int bg_pos_x, int bg_pos_y);
    void update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row, vector<Sun *> &suns,Zombie* _zombie_to_be_collided);

private:
};

class Sunflower : public Plant
{
public:
    Sunflower(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos);
    void render(int bg_pos_x, int bg_pos_y);
    void set_normal_plant_png() { plant_png_path = SUNFLOWER_PNG; }
    void set_cool_downed_plant_png() { plant_png_path = SUNFLOWER_COOLDOWN_PNG; }
    void update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row, vector<Sun *> &suns,Zombie* _zombie_to_be_collided);

private:
    Clock sunClock;
    Time sunElapsed;
    Vector2i sun_pos;
};

class PeaShooter : public Plant
{
public:
    PeaShooter(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos);
    void render(int bg_pos_x, int bg_pos_y);
    void set_normal_plant_png() { plant_png_path = PEASHOOTER_PNG; }
    void set_cool_downed_plant_png() { plant_png_path = PEASHOOTER_COOLDOWN_PNG; }
    void update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row, vector<Sun *> &suns,Zombie* _zombie_to_be_collided);

private:
};

class SnowPeaShooter : public Plant
{
public:
    SnowPeaShooter(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos);
    void render(int bg_pos_x, int bg_pos_y);
    void set_normal_plant_png() { plant_png_path = SNOW_PEASHOOTER_PNG; }
    void set_cool_downed_plant_png() { plant_png_path = SNOW_PEASHOOTER_COOLDOWN_PNG; }
    void update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row,vector<Sun *> &suns,Zombie* _zombie_to_be_collided);

private:
};

class Watermelon : public Plant
{
public:
    Watermelon(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos);
    void render(int bg_pos_x, int bg_pos_y);
    void set_normal_plant_png() { plant_png_path = WATERMELON_PNG; }
    void set_cool_downed_plant_png() { plant_png_path = WATERMELON_COOLDOWN_PNG; }
    void update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row,vector<Sun *> &suns,Zombie* _zombie_to_be_collided);

private:
};
