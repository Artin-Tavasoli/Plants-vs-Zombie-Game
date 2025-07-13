#pragma once
#include "plant.hpp"

enum State
{
    START,
    IN_GAME,
    WON,
    GAMEOVER,
    EXIT
};

class System
{
public:
    System(State _state);
    void run();
    ~System();
    void make_map();

private:
    RenderWindow window;
    int window_width;
    int window_height;
    int sun_speed;
    int sun_interval;
    int balance;
    vector<Plant *> plants;
    vector<Plant *> item_bar_objects;
    vector<Plant *> cool_downed_objects;
    vector<Projectile *> projectiles;
    vector<Zombie *> zombies;
    vector<Sun *> suns;
    Plant *dragged_object;
    State state;
    Texture menu_texture;
    Texture background_texture;
    Texture item_bar_texture;
    Texture lost_texture;
    Texture win_texture;
    Texture sun_texture;
    Texture sun_icon_bg_texture;
    Sprite menu_sprite;
    Sprite background_sprite;
    Sprite item_bar_sprite;
    Sprite win_sprite;
    Sprite lost_sprite;
    Sprite sun_sprite;
    Sprite sun_icon_bg_sprite;
    vector<vector<pair<Vector2i, bool>>> *Map;
    Time systemElapsed;
    Clock systemClock;
    Clock sun_clock_from_sky;
    Music grass_walk_music;
    Music game_over_music;
    Music victory_music;
    vector<int> attack_data;
    vector<int> sun_data;
    vector<int> time_interval;
    vector<int> num_zombies_per_phase;
    vector<int> zombie_arrival_time;
    vector<int> num_zombies_in_row;
    int current_phase;
    void win_render();
    void gameover_render();
    void update();
    void render();
    void handle_events();
    void handle_cooldown();
    void handle_zombie_projectile_collision();
    void handle_zombie_plant_collision();
    void handle_collision();
    void set_background();
    void adding_item_bar_objects();
    void handle_attack_wave();
    void write_phase_text(string phase_text);
    void delete_projectile_out_of_bounds();
    void add_falling_sky_sun();
    void add_sun_icon();
    void generate_zombie_random_times();
    void generate_zombies();
    void update_plants();
    void update_projectiles();
    void update_zombies();
    void update_suns();
    void select_plant_from_item_bar(Vector2i mousePosition);
    void plant_selected_item_from_item_bar(Vector2i mousePosition);
    void collect_sun(Vector2i mousePosition);
    void write_cooldown_text();
    void write_price_text();
    void handle_events_win_or_lose();
    void open_grass_walk_music();
    void open_game_over_music();
    int find_zombie_index(Zombie *zombie);
    void open_victory_music();
    void start_menu();
    void make_dark_bg();
    pair<Vector2i, bool> get_center_block_position(Vector2i mouse_pos);
    Zombie *find_nearest_zombie(int plant_row, int plant_pos_x);
};