#include "plant.hpp"
Plant::Plant(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos) : window_ptr(_window_ptr)
{
    pos = _pos;

    plant_png_path = _plant_png_path;
    is_in_cooldown = false;
}

Walnut::Walnut(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos)
    : Plant(_window_ptr, _plant_png_path, _pos)
{
    plant_name = WALNUT_NAME;
    vector<int> file_data;
    file_data = read_from_file(2, 5);
    health = file_data[1];
    cooldown = file_data[2];
    hit_rate = file_data[3];
    price = file_data[5];
}

Sunflower::Sunflower(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos)
    : Plant(_window_ptr, _plant_png_path, _pos)
{
    plant_name = SUNFLOWER_NAME;
    vector<int> file_data;
    file_data = read_from_file(2, 4);
    health = file_data[1];
    cooldown = file_data[2];
    hit_rate = file_data[3];
    price = file_data[5];
}

PeaShooter::PeaShooter(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos)
    : Plant(_window_ptr, _plant_png_path, _pos)
{
    plant_name = PEASHOOTER_NAME;
    vector<int> file_data;
    file_data = read_from_file(2, 1);
    health = file_data[1];
    cooldown = file_data[2];
    hit_rate = file_data[3];
    price = file_data[5];
}

SnowPeaShooter::SnowPeaShooter(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos)
    : Plant(_window_ptr, _plant_png_path, _pos)
{
    plant_name = SNOWPEASHOOTER_NAME;
    vector<int> file_data;
    file_data = read_from_file(2, 2);
    health = file_data[1];
    cooldown = file_data[2];
    hit_rate = file_data[3];
    price = file_data[5];
}

Watermelon::Watermelon(RenderWindow *_window_ptr, string _plant_png_path, Vector2i _pos)
    : Plant(_window_ptr, _plant_png_path, _pos)
{
    plant_name = WATERMELON_NAME;
    vector<int> file_data;
    file_data = read_from_file(2, 3);
    health = file_data[1];
    cooldown = file_data[2];
    hit_rate = file_data[3];
    price = file_data[5];
}

void Sunflower::render(int bg_pos_x, int bg_pos_y)
{
    if (!plant_texture.loadFromFile(PICS_PATH + plant_png_path))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    plant_sprite.setTexture(plant_texture);
    float scaleX = static_cast<float>(bg_pos_x) / (plant_texture.getSize().x);
    float scaleY = static_cast<float>(bg_pos_y) / (plant_texture.getSize().y);
    plant_sprite.setScale(scaleX, scaleY);
    plant_sprite.setPosition(pos.x, pos.y);
    window_ptr->draw(plant_sprite);
}

void PeaShooter::render(int bg_pos_x, int bg_pos_y)
{
    if (!plant_texture.loadFromFile(PICS_PATH + plant_png_path))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    plant_sprite.setTexture(plant_texture);
    float scaleX = static_cast<float>(bg_pos_x) / (plant_texture.getSize().x);
    float scaleY = static_cast<float>(bg_pos_y) / (plant_texture.getSize().y);
    plant_sprite.setScale(scaleX, scaleY);
    plant_sprite.setPosition(pos.x, pos.y);
    window_ptr->draw(plant_sprite);
}

void Walnut::render(int bg_pos_x, int bg_pos_y)
{
    if (!plant_texture.loadFromFile(PICS_PATH + plant_png_path))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    plant_sprite.setTexture(plant_texture);
    float scaleX = static_cast<float>(bg_pos_x) / (plant_texture.getSize().x);
    float scaleY = static_cast<float>(bg_pos_y) / (plant_texture.getSize().y);
    plant_sprite.setScale(scaleX, scaleY);
    plant_sprite.setPosition(pos.x, pos.y);
    window_ptr->draw(plant_sprite);
}

void SnowPeaShooter::render(int bg_pos_x, int bg_pos_y)
{
    if (!plant_texture.loadFromFile(PICS_PATH + plant_png_path))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    plant_sprite.setTexture(plant_texture);
    float scaleX = static_cast<float>(bg_pos_x) / (plant_texture.getSize().x);
    float scaleY = static_cast<float>(bg_pos_y) / (plant_texture.getSize().y);
    plant_sprite.setScale(scaleX, scaleY);
    plant_sprite.setPosition(pos.x, pos.y);
    window_ptr->draw(plant_sprite);
}

void Watermelon::render(int bg_pos_x, int bg_pos_y)
{
    if (!plant_texture.loadFromFile(PICS_PATH + plant_png_path))
    {
        cerr << "cant upload image!";
    }
    plant_sprite.setTexture(plant_texture);
    float scaleX = static_cast<float>(bg_pos_x) / (plant_texture.getSize().x);
    float scaleY = static_cast<float>(bg_pos_y) / (plant_texture.getSize().y);
    plant_sprite.setScale(scaleX, scaleY);
    plant_sprite.setPosition(pos.x, pos.y);
    window_ptr->draw(plant_sprite);
}

void PeaShooter::update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row, vector<Sun *> &suns , Zombie* _zombie_to_be_collided)
{
    if (plantClock.getElapsedTime().asSeconds() >= hit_rate)
    {
        plantClock.restart();
        Vector2i projectile_pos;
        projectile_pos.y = pos.y + 20;
        projectile_pos.x = pos.x + 60;

        if (num_zombies_in_row[row - 1] != 0)
        {
            Regular_projectile *regular_projectile = new Regular_projectile(window_ptr, projectile_pos, REGULAR_PROJECTILE_PNG);
            projectiles.push_back(regular_projectile);
        }
    }
}

void SnowPeaShooter::update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row, vector<Sun *> &suns, Zombie* _zombie_to_be_collided)
{
    if (plantClock.getElapsedTime().asSeconds() >= hit_rate)
    {
        plantClock.restart();
        Vector2i projectile_pos;
        projectile_pos.y = pos.y + 20;
        projectile_pos.x = pos.x + 60;
        if (num_zombies_in_row[row - 1] != 0)
        {
            Snow_projectile *snow_projectile = new Snow_projectile(window_ptr, projectile_pos, SNOW_PROJECTILE_PNG);
            projectiles.push_back(snow_projectile);
        }
    }
}

void Walnut::update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row, vector<Sun *> &suns, Zombie* _zombie_to_be_collided)
{
}

void Sunflower::update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row, vector<Sun *> &suns, Zombie* _zombie_to_be_collided)
{
    sunElapsed = sunClock.getElapsedTime();
    bool already_planted_sun = false;
    if (sunElapsed.asSeconds() >= hit_rate)
    {
        sunClock.restart();
        sun_pos.y = pos.y + 55;
        sun_pos.x = pos.x - 15;
        for (int i = 0; i < suns.size(); i++)
        {
            if (suns[i]->get_pos() == sun_pos)
            {
                already_planted_sun = true;
            }
        }
        if (!already_planted_sun)
        {
            Sun *sun = new Sun(window_ptr, sun_pos, false);
            suns.push_back(sun);
        }
    }
}

void Watermelon::update(vector<Projectile *> &projectiles, vector<int> num_zombies_in_row, vector<Sun *> &suns, Zombie* _zombie_to_be_collided)
{
    if (plantClock.getElapsedTime().asSeconds() >= hit_rate)
    {
        plantClock.restart();
        Vector2i projectile_pos;
        projectile_pos.y = pos.y ;
        projectile_pos.x = pos.x ;
        if (num_zombies_in_row[row - 1] != 0)
        {
            Watermelon_projectile *watermelon_projectile = new Watermelon_projectile(window_ptr, projectile_pos, WATERMELON_PROJECTILE_PNG);
            watermelon_projectile->set_zombie_collision(_zombie_to_be_collided);
            double time_in_motion =  (double)(_zombie_to_be_collided->get_pos_x() - projectile_pos.x)/(_zombie_to_be_collided->get_speed()+watermelon_projectile->get_speed()) / 25;
            watermelon_projectile->set_time_for_watermelon_motion(time_in_motion);
            projectiles.push_back(watermelon_projectile);
        }
    }
}

void Plant::change_pos(int x, int y)
{
    pos.x = x;
    pos.y = y;
    row = 0;
    if (FIRST_BLOCK_TOP_POS_Y <= pos.y && pos.y < SECOND_BLOCK_TOP_POS_Y)
        row = 1;
    if (SECOND_BLOCK_TOP_POS_Y <= pos.y && pos.y < THIRD_BLOCK_TOP_POS_Y)
        row = 2;
    if (THIRD_BLOCK_TOP_POS_Y <= pos.y && pos.y < FORTH_BLOCK_TOP_POS_Y)
        row = 3;
    if (FORTH_BLOCK_TOP_POS_Y <= pos.y && pos.y < FIFTH_BLOCK_TOP_POS_Y)
        row = 4;
    if (FIFTH_BLOCK_TOP_POS_Y <= pos.y && pos.y < FIFTH_BLOCK_DOWN_POS_Y)
        row = 5;
}

void Plant::update_cooldown()
{
    if (cooldown <= plantClock.getElapsedTime().asSeconds())
    {
        is_in_cooldown = false;
    }
}