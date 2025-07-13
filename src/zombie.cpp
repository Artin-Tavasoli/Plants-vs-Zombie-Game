#include "zombie.hpp"
Zombie::Zombie(RenderWindow *_window_ptr) : window_ptr(_window_ptr)
{
    is_moving = true;
    is_in_snow_mode = false;
}

NormalZombie::NormalZombie(RenderWindow *_window_ptr, Vector2i _pos, int _row)
    : Zombie(_window_ptr)
{
    IntRect rect;
    zombie_data = read_from_file(1, 1);
    damage = zombie_data[0];
    health = zombie_data[1];
    hit_rate = zombie_data[2];
    speed = zombie_data[3];
    row = _row;
    zombie_pos.x = _pos.x;
    zombie_pos.y = _pos.y;
    zombie_sprite.setPosition(_pos.x, _pos.y);
    if (!zombie_texture.loadFromFile(PICS_PATH + NORMAL_ZOMBIE_PNG))
    {
        cerr << "cant upload image!";
    }
    zombie_sprite.setTexture(zombie_texture);
    zombie_sprite.setScale(1, 1);
    rect.width = 80;
    rect.height = 120;
    zombie_sprite.setTextureRect(rect);
    walking_zombie_animation_poses[0] = 80;
    for (int i = 1; i < 46; i++)
    {
        walking_zombie_animation_poses[i] = walking_zombie_animation_poses[i - 1] + 226;
    }
}

GiantZombie::GiantZombie(RenderWindow *_window_ptr, Vector2i _pos, int _row)
    : Zombie(_window_ptr)
{
    zombie_data = read_from_file(1, 2);
    damage = zombie_data[0];
    health = zombie_data[1];
    hit_rate = zombie_data[2];
    speed = zombie_data[3];
    row = _row;
    zombie_pos.x = _pos.x;
    zombie_pos.y = _pos.y;
    zombie_sprite.setPosition(_pos.x, _pos.y);
    if (!zombie_texture.loadFromFile(PICS_PATH + GIANT_ZOMBIE_PNG))
    {
        cerr << ERROR_MESSAGE<<endl;
    }
    zombie_sprite.setTexture(zombie_texture);
    zombie_sprite.setScale(0.6, 0.6);
    IntRect rect;
    rect.width = 250;
    rect.height = 250;
    zombie_sprite.setTextureRect(rect);
    walking_zombie_animation_poses[0] = 80;
    for (int i = 1; i < 33; i++)
    {
        walking_zombie_animation_poses[i] = walking_zombie_animation_poses[i - 1] + 315;
    }
}

void Zombie::render()
{
    window_ptr->draw(zombie_sprite);
}

void Zombie::update_health(int projectile_damage)
{
    health -= projectile_damage;
}

void NormalZombie::update()
{
    Time zombie_animation_elapsed = animation_clock.getElapsedTime();
    IntRect rect;
    rect.width = 80;
    rect.height = 120;
    if (zombie_animation_elapsed.asMilliseconds() >= 50)
    {
        animation_clock.restart();
        current_animation_rect_number = (current_animation_rect_number + 1) % 46;
        rect.left = walking_zombie_animation_poses[current_animation_rect_number];
        zombie_sprite.setTextureRect(rect);
    }
    if (is_moving)
    {
        if (zombie_clock.getElapsedTime().asMilliseconds() > 20)
        {
            zombie_clock.restart();
            zombie_pos.x -= speed;
            zombie_sprite.setPosition(zombie_pos.x, zombie_pos.y);
        }
    }
    if (is_in_snow_mode)
    {
        update_texture(NORMAL_ZOMBIE_PNG, 1, 1, 80, 120);
        if (in_snow_clock.getElapsedTime().asSeconds() >= ZOMBIE_IN_SNOW_TIME)
        {
            speed = zombie_data[3];
            is_in_snow_mode = false;
        }
    }
}

void Zombie::update_texture(string zombie_png, float scale_x, float scale_y, int width, int height)
{
    if (!is_in_snow_mode)
    {
        if (!zombie_texture.loadFromFile(PICS_PATH + zombie_png))
        {
            cerr << "cant upload image!";
        }
    }
    else
    {
        if (!zombie_texture.loadFromFile(PICS_PATH + zombie_png))
        {
            cerr << "cant upload image!";
        }
        Image image = zombie_texture.copyToImage();
        for (unsigned int y = 0; y < image.getSize().y; ++y)
        {
            for (unsigned int x = 0; x < image.getSize().x; ++x)
            {
                Color pixel = image.getPixel(x, y);
                pixel.r /= 2;
                pixel.g /= 2;
                image.setPixel(x, y, pixel);
            }
        }
        zombie_texture.update(image);
    }

    IntRect rect;
    rect.width = width;
    rect.height = height;
    zombie_sprite.setTexture(zombie_texture);
    zombie_sprite.setScale(scale_x, scale_y);
}

void GiantZombie::update()
{
    Time zombie_animation_elapsed = animation_clock.getElapsedTime();
    IntRect rect;
    rect.width = 250;
    rect.height = 250;
    if (zombie_animation_elapsed.asMilliseconds() >= 100)
    {
        animation_clock.restart();
        current_animation_rect_number = (current_animation_rect_number + 1) % 33;
        rect.left = walking_zombie_animation_poses[current_animation_rect_number];
        zombie_sprite.setTextureRect(rect);
    }
    if (is_moving)
    {
        if (giant_zombie_clock.getElapsedTime().asMilliseconds() > 20)
        {
            giant_zombie_clock.restart();
            zombie_pos.x -= speed;
            zombie_sprite.setPosition(zombie_pos.x, zombie_pos.y);
        }
    }
    if (is_in_snow_mode)
    {
        update_texture(GIANT_ZOMBIE_PNG, 0.6, 0.6, 250, 250);
        if (in_snow_clock.getElapsedTime().asSeconds() >= ZOMBIE_IN_SNOW_TIME)
        {
            speed = zombie_data[3];
            is_in_snow_mode = false;
        }
    }
}

bool Zombie::is_in_hit_moment()
{
    if (hit_rate_clock.getElapsedTime().asSeconds() >= hit_rate)
    {
        hit_rate_clock.restart();
        return true;
    }
    return false;
}

void Zombie::change_is_in_snow_mode()
{
    is_in_snow_mode = true;
    speed = zombie_data[3] / 2;
    in_snow_clock.restart();
}