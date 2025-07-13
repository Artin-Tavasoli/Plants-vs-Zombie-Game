#include "sun.hpp"

Sun::Sun(RenderWindow *_window_ptr, Vector2i _pos, bool _in_motion) : window_ptr(_window_ptr)
{
    if (!texture.loadFromFile(PICS_PATH + SUN_PNG))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    sprite.setTexture(texture);
    sprite.setScale(0.5, 0.5);
    pos = _pos;
    sprite.setPosition(pos.x, pos.y);
    in_motion = _in_motion;
    generate_final_pos_y();
}

void Sun::render()
{
    window_ptr->draw(sprite);
}
void Sun::generate_final_pos_y()
{
    final_pos_y = get_random_number_between_a_limit(500) + BLOCK_TOP_LEFT_CORNER_Y;
}
void Sun::update(int speed)
{

    if (in_motion)
    {
        sprite.setOrigin(sprite.getLocalBounds().width/2,sprite.getLocalBounds().height/2);
        sprite.rotate(5);
        if (pos.y <= final_pos_y)
        {
            if (sun_clock.getElapsedTime().asMilliseconds() > 200)
            {
                sun_clock.restart();
                pos.y += speed;
                sprite.setPosition(pos.x, pos.y);
            }
        }
        else
        {
            wait_clock.restart();
            in_motion = false;
        }
    }
}

bool Sun::finished_waiting()
{
    if (wait_clock.getElapsedTime().asSeconds() >= SUN_WAITING_TIME)
    {
        wait_clock.restart();

        return true;
    }
    return false;
}
