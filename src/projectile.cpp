#include "projectile.hpp"

Projectile::Projectile(RenderWindow *_window_ptr, string projectile_type_png) : window_ptr(_window_ptr)
{
    if (!projectile_texture.loadFromFile(PICS_PATH + projectile_type_png))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    projectile_sprite.setTexture(projectile_texture);
}

void Projectile::render()
{
    projectile_sprite.setTexture(projectile_texture);
    window_ptr->draw(projectile_sprite);
}

void Projectile::update()
{
    if (projectile_clock.getElapsedTime().asMilliseconds() > 20)
    {
        projectile_clock.restart();
        if (projectile_name != WATERMELON_NAME)
        {
            pos.x += speed;
        }
        else
        {
            double time = motion_watermelon_clock.getElapsedTime().asSeconds();
            pos.x = initial_pos_x + speed * time * 50;
            pos.y = initial_pos_y + time * (time - Time_for_watermelon_motion) * 30;
        }
        projectile_sprite.setPosition(pos.x, pos.y);
    }
}

Regular_projectile::Regular_projectile(RenderWindow *_window_ptr, Vector2i _pos, string projectile_type_png)
    : Projectile(_window_ptr, projectile_type_png)
{
    projectile_name = REGULAR_PROJECTILE_NAME;
    pos = _pos;
    projectile_sprite.setPosition(_pos.x, _pos.y);
    file_data = read_from_file(2, 1);
    projectile_sprite.setScale(0.013, 0.013);
    damage = file_data[0];
    speed = file_data[4];
}

Snow_projectile::Snow_projectile(RenderWindow *_window_ptr, Vector2i _pos, string projectile_type_png)
    : Projectile(_window_ptr, projectile_type_png)
{
    projectile_name = SNOW_PROJECTILE_NAME;
    pos = _pos;
    projectile_sprite.setPosition(_pos.x, _pos.y);
    file_data = read_from_file(2, 2);
    projectile_sprite.setScale(0.013, 0.013);
    damage = file_data[0];
    speed = file_data[4];
}

Watermelon_projectile::Watermelon_projectile(RenderWindow *_window_ptr, Vector2i _pos, string projectile_type_png)
    : Projectile(_window_ptr, projectile_type_png)
{
    projectile_name = WATERMELON_NAME;
    pos = _pos;
    initial_pos_x = pos.x;
    initial_pos_y = pos.y;
    projectile_sprite.setPosition(_pos.x, _pos.y);
    file_data = read_from_file(2, 3);
    projectile_sprite.setScale(0.8, 0.8);
    damage = file_data[0];
    speed = file_data[4];
    motion_watermelon_clock.restart();
}