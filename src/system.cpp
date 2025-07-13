#include "system.hpp"

System::System(State _state)
{
    sun_data = read_from_file(4, 1);
    sun_speed = sun_data[0];
    sun_interval = sun_data[1];
    dragged_object = NULL;
    state = _state;
    attack_data = read_from_file(3, 1);
    for (int i = 0; i < attack_data[0] / attack_data[1]; i++)
        time_interval.push_back(i * attack_data[1]);
    for (int i = 0; i < attack_data[0] / attack_data[1]; i++)
        num_zombies_per_phase.push_back(attack_data[2] + attack_data[3] * i);
    current_phase = 0;
    set_background();
    balance = 50;
    for (int i = 0; i < NUM_OF_ROWS; i++)
    {
        num_zombies_in_row.push_back(0);
    }
}

System::~System()
{
    for (auto plant : plants)
        delete plant;
    for (auto item_bar_object : item_bar_objects)
        delete item_bar_object;
    for (auto cool_downed_object : cool_downed_objects)
        delete cool_downed_object;
    for (auto projectile : projectiles)
        delete projectile;
    for (auto zombie : zombies)
        delete zombie;
}

void System::handle_cooldown()
{
    for (auto item_bar_object : item_bar_objects)
    {
        if (item_bar_object->get_is_in_cooldown())
            item_bar_object->update_cooldown();
    }
    for (auto item_bar_object : item_bar_objects)
    {
        if (item_bar_object->get_is_in_cooldown())
        {
            item_bar_object->set_cool_downed_plant_png();
        }
        else
        {
            item_bar_object->set_normal_plant_png();
        }
    }
}

void System::generate_zombie_random_times()
{
    if(current_phase == attack_data[0] / attack_data[1])
        return;
    if (systemClock.getElapsedTime().asSeconds() >= attack_data[1])
    {
        systemClock.restart();
        current_phase++;
        zombie_arrival_time.clear();
        for (int i = 0; i < num_zombies_per_phase[current_phase - 1]; i++)
        {
            if (attack_data[1] == 1)
                zombie_arrival_time.push_back(0);
            else
            {
                float random_time = get_random_number_between_a_limit(attack_data[1]);
                zombie_arrival_time.push_back(random_time);
            }
        }
        sort(zombie_arrival_time.begin(), zombie_arrival_time.end());
    }
}

void System::generate_zombies()
{
    for (int i = 0; i < zombie_arrival_time.size(); i++)
    {
        if (zombie_arrival_time[i] <= systemClock.getElapsedTime().asSeconds() && zombie_arrival_time[i] != -1)
        {
            int which_random_zombie = get_random_number_between_a_limit(3);
            zombie_arrival_time[i] = -1;
            int block_row[5] = {20, 120, 220, 320, 420};
            int which_random_row_zombie = get_random_number_between_a_limit(6);
            Vector2i zombie_pos;
            zombie_pos.y = block_row[which_random_row_zombie - 1];
            num_zombies_in_row[which_random_row_zombie - 1]++;
            zombie_pos.x = PLAY_GROUND_LENGTH;
            if (which_random_zombie == 1)
            {
                NormalZombie *normalzombie = new NormalZombie(&window, zombie_pos, which_random_row_zombie);
                zombies.push_back(normalzombie);
            }
            else
            {
                GiantZombie *giantzombie = new GiantZombie(&window, zombie_pos, which_random_row_zombie);
                zombies.push_back(giantzombie);
            }
        }
    }
}

void System::handle_attack_wave()
{
    generate_zombie_random_times();
    generate_zombies();
}

int System::find_zombie_index(Zombie *zombie)
{
    for (int i = 0; i < zombies.size(); i++)
    {
        if (zombies[i] == zombie)
        {
            return i;
        }
    }
    return 0;
}

void System::handle_zombie_projectile_collision()
{
    if (zombies.empty() || projectiles.empty())
        return;
    for (int i = 0; i < projectiles.size(); i++)
    {
        if (projectiles[i]->get_name() == WATERMELON_NAME && projectiles[i]->is_end_collision_time())
        {
            auto save_p = projectiles[i];
            Zombie *nearest_zombie = projectiles[i]->get_nearest_zombie();
            nearest_zombie->update_health(projectiles[i]->get_damage());
            projectiles.erase(projectiles.begin() + i);
            if (nearest_zombie->get_health() <= 0)
            {
                num_zombies_in_row[nearest_zombie->get_row() - 1]--;
                auto save_z = nearest_zombie;
                zombies.erase(zombies.begin() + find_zombie_index(nearest_zombie));
                delete save_z;
            }
            delete save_p;
            i--;
        }
    }
    for (int i = 0; i < zombies.size(); i++)
    {
        for (int j = 0; j < projectiles.size(); j++)
        {
            FloatRect p_rect = projectiles[j]->get_rect();
            FloatRect z_rect = zombies[i]->get_rect();
            if (z_rect.intersects(p_rect))
            {
                if (projectiles[j]->get_name() == SNOW_PROJECTILE_NAME && !zombies[i]->get_in_snow_mode())
                {
                    zombies[i]->change_is_in_snow_mode();
                }
                zombies[i]->update_health(projectiles[j]->get_damage());
                auto save_p = projectiles[j];
                projectiles.erase(projectiles.begin() + j);
                delete save_p;
                j--;
                if (zombies[i]->get_health() <= 0)
                {
                    num_zombies_in_row[zombies[i]->get_row() - 1]--;
                    auto save_z = zombies[i];
                    zombies.erase(zombies.begin() + i);
                    delete save_z;
                    i--;
                }
                break;
            }
        }
    }
}

void System::delete_projectile_out_of_bounds()
{
    for (int i = 0; i < projectiles.size(); i++)
    {
        if (projectiles[i]->get_pos().x > PLAY_GROUND_LENGTH)
        {
            auto save_p = projectiles[i];
            projectiles.erase(projectiles.begin() + i);
            delete save_p;
            i--;
        }
    }
}

void System::handle_zombie_plant_collision()

{
    if (zombies.empty() || plants.empty())
        return;
    for (int j = 0; j < plants.size(); j++)
    {
        FloatRect p_rect = plants[j]->get_rect();
        for (int i = 0; i < zombies.size(); i++)
        {
            FloatRect z_rect = zombies[i]->get_rect();
            if (z_rect.intersects(p_rect) && zombies[i]->get_row() == plants[j]->get_row())
            {
                zombies[i]->make_stable();
                if (zombies[i]->is_in_hit_moment())
                {
                    plants[j]->update_health(zombies[i]->get_damage());
                }
            }
        }
        if (plants[j]->get_health() <= 0)
        {
            for (auto zombie : zombies)
            {
                if (zombie->get_row() == plants[j]->get_row())
                {
                    zombie->set_in_motion();
                }
            }
            auto save_p = plants[j];
            plants.erase(plants.begin() + j);
            delete save_p;
            j--;
        }
    }
}

void System::handle_collision()
{
    handle_zombie_projectile_collision();
    handle_zombie_plant_collision();
}

void System::gameover_render()
{
    Font font;
    if (!font.loadFromFile(FONTS_PATH + GAME_OVER_FONT))
    {
        cerr << ERROR_MESSAGE << endl;
    }

    Text text1;
    text1.setFont(font);
    text1.setString("ZOMBIES");
    text1.setFillColor(Color::Green);
    text1.setCharacterSize(80);
    text1.setPosition(500, 100);

    Text text2;
    text2.setFont(font);
    text2.setString("ATE YOUR");
    text2.setFillColor(Color::Green);
    text2.setCharacterSize(80);
    text2.setPosition(500, 250);

    Text text3;
    text3.setFont(font);
    text3.setString("BRAIN!");
    text3.setFillColor(Color::Green);
    text3.setCharacterSize(80);
    text3.setPosition(520, 400);
    window.draw(text1);
    window.display();
    sleep(1);
    window.draw(text2);
    window.display();
    sleep(1);
    window.draw(text3);
    window.display();
}

void System::handle_events_win_or_lose()
{
    Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case (Event::KeyPressed):
            if (event.key.code == Keyboard::Escape)
            {
                window.close();
            }
            break;
        case (Event::Closed):
            window.close();
            break;
        }
    }
}

void System::win_render()
{
    if (!win_texture.loadFromFile(PICS_PATH + WIN_SCREEN_PNG))
        cerr << ERROR_MESSAGE << endl;
    win_sprite.setTexture(win_texture);
    win_sprite.setScale(0.3, 0.3);
    window.draw(win_sprite);
    window.display();
}

void System::start_menu()
{
    if (!menu_texture.loadFromFile(PICS_PATH + MENU_SCREEN))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    menu_sprite.setTexture(menu_texture);
    float scaleX = static_cast<float>(WINDOW_LENGTH) / (menu_texture.getSize().x);
    float scaleY = static_cast<float>(WINDOW_WIDTH) / (menu_texture.getSize().y);
    menu_sprite.setScale(scaleX, scaleY);
    window.draw(menu_sprite);
    window.display();
    Event event;
    while (window.pollEvent(event))
    {
        bool pressed = false;
        switch (event.type)
        {
        case (Event::KeyPressed):
            if (event.key.code == Keyboard::Escape)
            {
                window.close();
            }
            else if (event.key.code == Keyboard::Space)
            {
                state = IN_GAME;
            }
            break;
        case (Event::Closed):
            window.close();
            break;
        }
    }
}

void System::make_dark_bg()
{
    RectangleShape darkOverlay;
    darkOverlay.setSize(Vector2f(window.getSize().x, window.getSize().y));
    darkOverlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(darkOverlay); 
}

void System::run()
{
    bool first_in_game = true;
    int count_game_over_entering = 0;
    bool first_won_entering = true;
    while (window.isOpen() && state != EXIT)
    {
        switch (state)
        {
        case START:
            start_menu();
            break;
        case IN_GAME:
            if(current_phase != attack_data[0] / attack_data[1] +1)
                handle_attack_wave();
            if (first_in_game)
            {
                open_grass_walk_music();
            }
            update();
            handle_events();
            render();
            window.display();
            first_in_game = false;
            break;
        case GAMEOVER:
            grass_walk_music.pause();
            count_game_over_entering++;
            if(count_game_over_entering ==1)
            {
                make_dark_bg();
            }
            if (count_game_over_entering <= 3)
            {
                open_game_over_music();
            }
            gameover_render();
            handle_events_win_or_lose();
            break;
        case WON:
            grass_walk_music.pause();
            if (first_won_entering)
            {
                open_victory_music();
            }
            win_render();
            handle_events_win_or_lose();
            first_won_entering = false;
            break;
        }
    }
    window.close();
}

void System::add_falling_sky_sun()
{
    if (sun_clock_from_sky.getElapsedTime().asSeconds() >= sun_interval)
    {
        sun_clock_from_sky.restart();
        int random_sun_pos_x = get_random_number_between_a_limit(720);
        Sun *sun = new Sun(&window, Vector2i(random_sun_pos_x + BLOCK_TOP_LEFT_CORNER_X, 0), true);
        suns.push_back(sun);
    }
}

void System::update_suns()
{
    for (auto sun : suns)
    {
        sun->update(sun_speed);
    }

    for (int i = 0; i < suns.size(); i++)
    {
        if (!suns[i]->get_in_motion())
        {
            if (suns[i]->finished_waiting())
            {
                auto save_s = suns[i];
                suns.erase(suns.begin() + i);
                delete save_s;
                i--;
            }
        }
    }
}

Zombie *System::find_nearest_zombie(int plant_row, int plant_pos_x)
{
    vector<int> zombies_pos_x;
    for (auto zombie : zombies)
    {
        if (zombie->get_row() == plant_row && zombie->get_pos_x() >= plant_pos_x)
        {
            zombies_pos_x.push_back(zombie->get_pos_x());
        }
    }
    if (zombies_pos_x.empty())
        return NULL;
    sort(zombies_pos_x.begin(), zombies_pos_x.end());
    for (auto zombie : zombies)
    {
        if (zombie->get_row() == plant_row && zombie->get_pos_x() >= plant_pos_x)
        {
            if(zombies_pos_x[0] == zombie->get_pos_x())
            {
                return zombie;
            }
           
        }
    }
    return NULL;
}

void System::update_plants()
{
    for (auto plant : plants)
    {
        int plant_row = plant->get_row();
        int plant_pos_x = plant->get_pos().x;
        plant->update(projectiles, num_zombies_in_row, suns, find_nearest_zombie(plant_row, plant_pos_x));
    }
}

void System::update_zombies()
{
    for (auto zombie : zombies)
    {
        zombie->update();
        if (zombie->get_pos_x() <= BLOCK_TOP_LEFT_CORNER_X)
        {
            state = GAMEOVER;
        }
    }
}

void System::update_projectiles()
{
    for (auto projectile : projectiles)
    {
        projectile->update();
    }
}

void System::update()
{
    handle_cooldown();
    handle_collision();
    delete_projectile_out_of_bounds();
    add_falling_sky_sun();
    update_plants();
    update_projectiles();
    update_zombies();
    update_suns();

    if (current_phase == attack_data[0] / attack_data[1] && zombies.empty())
    {
        state = WON;
    }
}

void System::select_plant_from_item_bar(Vector2i mousePosition)
{
    if (mousePosition.x > 0 && mousePosition.x < ITEM_BAR_LENGTH)
    {
        if (mousePosition.y > FIRST_ITEM_BAR_POS_Y && mousePosition.y < SECOND_ITEM_BAR_POS_Y)
        {
            dragged_object = new Sunflower(&window, SUNFLOWER_PNG, Vector2i(0, FIRST_ITEM_BAR_POS_Y));
        }
        else if (mousePosition.y > SECOND_ITEM_BAR_POS_Y && mousePosition.y < THIRD_ITEM_BAR_POS_Y)
        {
            dragged_object = new Walnut(&window, WALLNUT_PNG, Vector2i(0, SECOND_ITEM_BAR_POS_Y));
        }
        else if (mousePosition.y > THIRD_ITEM_BAR_POS_Y && mousePosition.y < FORTH_ITEM_BAR_POS_Y)
        {
            dragged_object = new PeaShooter(&window, PEASHOOTER_PNG, Vector2i(0, THIRD_ITEM_BAR_POS_Y));
        }
        else if (mousePosition.y > FORTH_ITEM_BAR_POS_Y && mousePosition.y < FIFTH_ITEM_BAR_POS_Y)
        {
            dragged_object = new SnowPeaShooter(&window, SNOW_PEASHOOTER_PNG, Vector2i(0, FORTH_ITEM_BAR_POS_Y));
        }
        else if (mousePosition.y > FIFTH_ITEM_BAR_POS_Y && mousePosition.y < ITEM_BAR_WEIDTH + FIRST_ITEM_BAR_POS_Y)
        {
            dragged_object = new Watermelon(&window, WATERMELON_PNG, Vector2i(0, FORTH_ITEM_BAR_POS_Y));
        }
        if (dragged_object->get_price() <= balance)
        {
            balance -= dragged_object->get_price();
        }
        else
        {
            dragged_object = NULL;
        }
    }
}

void System::plant_selected_item_from_item_bar(Vector2i mousePosition)
{
    pair<Vector2i, bool> center_pos_and_if_in_board;
    center_pos_and_if_in_board = get_center_block_position(mousePosition);
    if (center_pos_and_if_in_board.second)
    {
        dragged_object->change_pos(center_pos_and_if_in_board.first.x, center_pos_and_if_in_board.first.y);
        for (auto item_bar_object : item_bar_objects)
        {
            if (item_bar_object->get_plant_name() == dragged_object->get_plant_name() && !item_bar_object->get_is_in_cooldown())
            {
                item_bar_object->start_timer();
                item_bar_object->change_is_in_cooldown();
                plants.push_back(dragged_object);
            }
        }
        dragged_object = NULL;
    }
}

void System::collect_sun(Vector2i mousePosition)
{
    for (int i = 0; i < suns.size(); i++)
    {
        if (suns[i]->get_sprite().getGlobalBounds().contains(mousePosition.x, mousePosition.y))
        {
            balance += SUN_PRICE;
            auto save_s = suns[i];
            suns.erase(suns.begin() + i);
            delete save_s;
            i--;
        }
    }
}

void System::handle_events()
{
    Event event;
    Vector2i initial_pos;
    Vector2i final_pos;
    while (window.pollEvent(event))
    {
        bool pressed = false;
        switch (event.type)
        {
        case (Event::KeyPressed):
            if (event.key.code == Keyboard::Escape)
            {
                window.close();
            }
            break;
        case (Event::Closed):
            window.close();
        case (Event::MouseButtonPressed):
            if (event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePosition = Mouse::getPosition(window);
                select_plant_from_item_bar(mousePosition);
                collect_sun(mousePosition);
            }
            break;

        case (Event::MouseButtonReleased):
            if (event.mouseButton.button == Mouse::Left && dragged_object != NULL)
            {
                Vector2i mousePosition = Mouse::getPosition(window);
                plant_selected_item_from_item_bar(mousePosition);
            }
            break;
        }
    }
}

void System ::set_background()
{
    window.create(VideoMode(WINDOW_LENGTH, WINDOW_WIDTH), "PVZ");
    if (!background_texture.loadFromFile(PICS_PATH + FRONTYARD_PNG))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    background_sprite.setTexture(background_texture);
    if (!item_bar_texture.loadFromFile(PICS_PATH + ITEM_BAR_PNG))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    item_bar_sprite.setTexture(item_bar_texture);
    float scaleX = static_cast<float>(ITEM_BAR_LENGTH) / (item_bar_texture.getSize().x);
    float scaleY = static_cast<float>(ITEM_BAR_WEIDTH) / (item_bar_texture.getSize().y);
    item_bar_sprite.setScale(scaleX, scaleY);
    item_bar_sprite.setPosition(0, 50);
    adding_item_bar_objects();
}

void System::adding_item_bar_objects()
{
    PeaShooter *peashooter = new PeaShooter(&window, PEASHOOTER_PNG, Vector2i(0, THIRD_ITEM_BAR_POS_Y));
    SnowPeaShooter *snowpeashooter = new SnowPeaShooter(&window, SNOW_PEASHOOTER_PNG, Vector2i(0, FORTH_ITEM_BAR_POS_Y));
    Sunflower *sunflower = new Sunflower(&window, SUNFLOWER_PNG, Vector2i(0, FIRST_ITEM_BAR_POS_Y));
    Walnut *walnut = new Walnut(&window, WALLNUT_PNG, Vector2i(0, SECOND_ITEM_BAR_POS_Y));
    Watermelon *watermelon = new Watermelon(&window, WATERMELON_PNG, Vector2i(0, FIFTH_ITEM_BAR_POS_Y));
    item_bar_objects.push_back(peashooter);
    item_bar_objects.push_back(snowpeashooter);
    item_bar_objects.push_back(sunflower);
    item_bar_objects.push_back(walnut);
    item_bar_objects.push_back(watermelon);
}

void System::add_sun_icon()
{
    if (!sun_icon_bg_texture.loadFromFile(PICS_PATH + SUN_ICON_BG))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    sun_icon_bg_sprite.setTexture(sun_icon_bg_texture);
    sun_icon_bg_sprite.setScale(0.15, 0.2);
    sun_icon_bg_sprite.setPosition(1320, 0);
    if (!sun_texture.loadFromFile(PICS_PATH + SUN_PNG))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    sun_sprite.setTexture(sun_texture);
    sun_sprite.setScale(0.6, 0.6);
    sun_sprite.setPosition(1330, 0);
    window.draw(sun_icon_bg_sprite);
    window.draw(sun_sprite);
    Font font;
    if (!font.loadFromFile(FONTS_PATH + PHASE_FONT_TTF))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    Text text;
    text.setFont(font);
    text.setString(to_string(balance));
    text.setCharacterSize(20);
    text.setFillColor(Color::Yellow);
    text.setPosition(1350, 65);
    window.draw(text);
}

void System::render()
{
    window.clear();
    window.draw(background_sprite);
    window.draw(item_bar_sprite);
    add_sun_icon();
    for (auto plant : plants)
    {
        plant->render(RECT_LENGTH, RECT_WEIDTH);
    }
    for (auto item_bar_object : item_bar_objects)
    {
        item_bar_object->render(ITEM_BAR_LENGTH, ITEM_BAR_WEIDTH / NUM_OF_ITEMS);
    }
    for (auto projectile : projectiles)
    {
        projectile->render();
    }
    for (auto zombie : zombies)
    {
        zombie->render();
    }
    for (auto sun : suns)
    {
        sun->render();
    }
    write_phase_text("PHASE: " + to_string(current_phase));
    write_cooldown_text();
    write_price_text();
}

void System::open_grass_walk_music()
{
    if (!grass_walk_music.openFromFile(MUSICS_PATH + GRASS_WALK_MUSIC))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    grass_walk_music.setLoop(true);
    grass_walk_music.play();
}

void System::open_game_over_music()
{
    if (!game_over_music.openFromFile(MUSICS_PATH + GAME_OVER_MUSIC))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    game_over_music.play();
}

void System::open_victory_music()
{
    if (!victory_music.openFromFile(MUSICS_PATH + VICTORY_MUSIC))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    victory_music.play();
}

void System::write_cooldown_text()
{
    Font font;
    if (!font.loadFromFile(FONTS_PATH + PHASE_FONT_TTF))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    Text text;
    text.setFont(font);
    for (auto item_bar_object : item_bar_objects)
    {
        if (item_bar_object->get_is_in_cooldown())
        {
            double remaining_cooldown_time;
            remaining_cooldown_time = item_bar_object->get_cooldown() - item_bar_object->get_cooldown_clock().getElapsedTime().asSeconds();
            ostringstream stream;
            stream << fixed << setprecision(1) << remaining_cooldown_time;
            text.setString(stream.str());
            text.setCharacterSize(20);
            text.setFillColor(Color::Red);
            text.setPosition(item_bar_object->get_pos().x + 30, item_bar_object->get_pos().y + 30);
            window.draw(text);
        }
    }
}

void System::write_price_text()
{

    Font font;
    if (!font.loadFromFile(FONTS_PATH + PHASE_FONT_TTF))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    Text text;
    text.setFont(font);
    RectangleShape background;
    background.setFillColor(Color(139, 69, 19));
    for (auto item_bar_object : item_bar_objects)
    {
        ostringstream stream;
        stream << fixed << setprecision(1) << item_bar_object->get_price();
        ;
        text.setString(stream.str());
        text.setCharacterSize(30);
        text.setFillColor(Color::Black);
        background.setSize(Vector2f(text.getGlobalBounds().width + 13, text.getGlobalBounds().height - 8));
        background.setPosition(item_bar_object->get_pos().x, item_bar_object->get_pos().y + 75);
        text.setPosition(item_bar_object->get_pos().x, item_bar_object->get_pos().y + 67);
        window.draw(background);
        window.draw(text);
    }
}

void System::write_phase_text(string phase_text)
{
    Font font;
    if (!font.loadFromFile(FONTS_PATH + PHASE_FONT_TTF))
    {
        cerr << ERROR_MESSAGE << endl;
    }
    Text text;
    RectangleShape background;
    background.setFillColor(Color(139, 69, 19));
    text.setFont(font);
    text.setString(phase_text);
    text.setCharacterSize(30);
    background.setSize(Vector2f(text.getGlobalBounds().width + 15, text.getGlobalBounds().height + 12));
    background.setPosition(0, 0);
    text.setFillColor(Color::Black);
    text.setPosition(0, 0);
    window.draw(background);
    window.draw(text);
}

void System::make_map()
{
    vector<vector<pair<Vector2i, bool>>> tempMap(NUM_OF_VERTICAL_BLOCKS);
    vector<pair<Vector2i, bool>> temp_horizental(NUM_OF_HORIZENTAL_BLOCKS);
    for (int i = 0; i < tempMap.size(); i++)
    {
        for (int j = 0; j < temp_horizental.size(); j++)
        {
            temp_horizental[j].first.x = BLOCK_TOP_LEFT_CORNER_X + j * RECT_LENGTH;
            temp_horizental[j].first.y = BLOCK_TOP_LEFT_CORNER_Y + i * RECT_WEIDTH;
            temp_horizental[j].second = false;
        }
        tempMap[i] = temp_horizental;
    }
    Map = new vector<vector<pair<Vector2i, bool>>>(tempMap);
}

pair<Vector2i, bool> System::get_center_block_position(Vector2i mouse_pos)
{
    Vector2i center_pos;
    bool can_plant_object = false;
    pair<Vector2i, bool> center_pos_and_if_in_board;
    for (int i = 0; i < Map->size(); i++)
    {
        for (int j = 0; j < (*Map)[i].size(); j++)
        {
            if (mouse_pos.x < (*Map)[i][j].first.x + RECT_LENGTH && mouse_pos.x > (*Map)[i][j].first.x && mouse_pos.y < (*Map)[i][j].first.y + RECT_WEIDTH && mouse_pos.y > (*Map)[i][j].first.y)
            {
                if (mouse_pos.x <= PLAY_GROUND_LENGTH && mouse_pos.y <= PLAY_GROUND_WIDTH && (*Map)[i][j].second == false)
                {
                    center_pos.x = (*Map)[i][j].first.x;
                    center_pos.y = (*Map)[i][j].first.y;
                    (*Map)[i][j].second = true;
                    can_plant_object = true;
                }
            }
        }
    }
    center_pos_and_if_in_board.first = center_pos;
    center_pos_and_if_in_board.second = can_plant_object;
    return center_pos_and_if_in_board;
}
