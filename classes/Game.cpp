//Name: Grant Clark
//Date: November 26th, 2021
//File: Game.cpp

#include "Game.h"

Game::Game() : surface(new Surface(SCREEN_W, SCREEN_H, "Galaxian")),
               background(new Background()),
               ship(new Ship()),
               game_running(false),
               MAX_LASERS(3),
               EXTRA_LIFE_SCORE(12500),
               RESPAWN_TIME(800),
               respawn_counter(0),
               E_RESPAWN_TIME(500),
               GAME_OVER_TIME(300),
               close_window(false),
               space_pressed(false),
               score(0),
               score_f(Font("fonts/font.otf", 15)),
               middle_screen_f(Font("fonts/font.otf", 20)),
               title_f(Font("fonts/font.otf", 35)),
               e_respawn_text(new Image(middle_screen_f.render("Reinforcements Incoming!", RED))),
               game_over_text(new Image(middle_screen_f.render("Game Over", RED))),
               game_start_text(new Image(middle_screen_f.render("Press Enter To Start", GREEN))),
               user_score_text(new Image(score_f.render("Score", WHITE))),
               high_score_text(new Image(score_f.render("High", WHITE))),
               name(new Image(score_f.render("Grant Clark", WHITE))),
               title(new Image(title_f.render("Galaxian", BLUE))),
               user_score_num(nullptr),
               high_score_num(nullptr),
               score_line(Rect(10, 30, 50, 3)),
               high_line(Rect(SCREEN_W - 60, 30, 50, 3)),
               laser_s(Sound("sounds/laser.wav")),
               explode_s(Sound("sounds/explosion.wav")),
               game_music(Music("sounds/GameLoop.ogg")),
               muted(false),
               m_key_pressed(false),
               paused(false),
               p_key_pressed(false),
               round(0),
               round_text(nullptr)
{
    srand(time(NULL));

    Entity::set_surface(surface);
    Explosion::set_surface(surface);
    Background::set_surface(surface);

    make_text();
    read_high_score();
    make_showcase();
    
    return;
}


Game::~Game()
{
    for (int i = 0; i < enemies.size(); i++)
        if (enemies.at(i) != nullptr)
            delete enemies.at(i);
    
    for (int i = 0; i < e_points.size(); ++i)
        if (e_points.at(i) != nullptr)
            delete e_points.at(i);
    
    if (background != nullptr)
        delete background;
    
    if (ship != nullptr)
        delete ship;
    
    if (surface != nullptr)
        delete surface;

    if (user_score_text != nullptr)
        delete user_score_text;

    if (high_score_text != nullptr)
        delete high_score_text;

    if (user_score_num != nullptr)
        delete user_score_num;

    if (high_score_num != nullptr)
        delete high_score_num;

    if (e_respawn_text != nullptr)
        delete e_respawn_text;

    if (game_over_text != nullptr)
        delete game_over_text;

    if (game_start_text != nullptr)
        delete game_start_text;

    if (name != nullptr)
        delete name;

    if (title != nullptr)
        delete title;

    if (round_text != nullptr)
        delete round_text;
    
    return;
}


void Game::run()
{
    const int RATE = 10; //100 fps
    int start_time, end_time, delay_time;
    
    //Infinite game loop.
    while (true)
    {
        start_time = getTicks();
        
        if ((event.poll() && event.type() == QUIT) ||
            close_window)
            break;

        //Capture all keyboard input.
        capture_input();

        //If closing the window, do not
        //perform any actions.
        if (!close_window && !paused)
        {
            update_all();

            draw_all();

            end_time = getTicks();
            delay_time = RATE - end_time + start_time;
            //delay by 8 milliseconds.
            if (delay_time > 0)
                delay(delay_time);
        }
    }

    if (score > high_score ||
        (score == high_score && high_score == 0))
    {
        high_score = score;
        save_high_score();
    }

    return;
}


void Game::make_text()
{
    //Set positions of score texts.
    user_score_text->rect().x = score_line.x + (score_line.w / 2)
        - (user_score_text->getRect().w / 2);
    user_score_text->rect().y = 12;

    high_score_text->rect().x = high_line.x + (high_line.w / 2)
        - (high_score_text->getRect().w / 2);
    high_score_text->rect().y = 12;

    //Set position of enemy respawn text.
    e_respawn_text->rect().x = (SCREEN_W / 2) - (e_respawn_text->getRect().w / 2);
    e_respawn_text->rect().y = (SCREEN_H / 2) - (e_respawn_text->getRect().h / 2);

    game_over_text->rect().x = (SCREEN_W / 2) - (game_over_text->getRect().w / 2);
    game_over_text->rect().y = (SCREEN_H / 2) - (game_over_text->getRect().h / 2);

    game_start_text->rect().x = (SCREEN_W / 2) - (game_start_text->getRect().w / 2);
    game_start_text->rect().y = (SCREEN_H / 2) - (game_start_text->getRect().h / 2);

    name->rect().x = (SCREEN_W / 2) - (name->getRect().w / 2);
    name->rect().y = SCREEN_H - 20;

    title->rect().x = (SCREEN_W / 2) - (title->getRect().w / 2);
    title->rect().y = 10;

    //Create showcase score text.
    for (int i = 0; i <= int(Flagship); ++i)
    {
        e_points.push_back(new Image(score_f.render(std::to_string((i + 1) * 100).c_str(), WHITE)));
        e_points.back()->rect().y = SCREEN_H / 2 - 115;
    }
    
    return;
}


void Game::read_high_score()
{
    std::ifstream f;
    std::string line;
    
    f.open("high.txt", std::ios::in);
    if (f.is_open())
    {
        f >> line;
        f.close();

        for (int i = 0; i < line.size(); ++i)
        {
            if (!isdigit(line.at(i)))
            {
                high_score = 0;
                break;
            }

            if (i == line.size() - 1)
            {
                high_score = atoi(line.c_str());
                break;
            }
        }
    }

    //If the file is not found.
    else
    {
        line = "0";
        high_score = 0;
    }
    
    if (high_score_num != nullptr)
        delete high_score_num;
    high_score_num = new Image(score_f.render(line.c_str(), WHITE));
    high_score_num->rect().x = high_line.x + (high_line.w / 2)
        - (high_score_num->getRect().w / 2);
    high_score_num->rect().y = 30;

    return;
}


void Game::save_high_score()
{
    std::ofstream f;
    f.open("high.txt", std::ios::out | std::ios::trunc);
    f << high_score;
    f.close();

    return;
}


//Creates the default alien formation.
void Game::make_formation()
{
    /*
      F = Alien Flagship
      R = Red Alien
      P = Purple Alien
      B = Blue Alien
      
            F     F
          R R R R R R
        P P P P P P P P 
      B B B B B B B B B B
      B B B B B B B B B B
      B B B B B B B B B B

      Space between each enemy is 3 pixels.
      This space is defined by the offset integer.
      Space between top of formation and Flagships
      is 50 pixels.
    */

    //Even if offset is zero, the sprites will still seem distanced
    //from each other due to whitespace in the images.
    int offset = 0; 
    int from_top = 50;
    //32 is the width of an enemy sprite.
    int from_left = (SCREEN_W - ((32 + offset) * 10)) / 2;

    //Capture offset min and max for formation moving later in
    //the update_all() function.
    e_offset_min = ((from_left / 2) + (from_left / 2)) * -1;
    e_offset_max = (from_left) / 2 + (from_left / 2);
    e_move_counter = 0;
        
    //Clear the vector.
    for (int i = 0; i < enemies.size(); i++)
        delete enemies.at(i);
    enemies.clear();
    
    //Flagships.
    for (int i = 0; i < 2; i++)
    {
        enemies.push_back(new Enemy(Flagship));
        enemies.back()->set_pos( from_left +
            (enemies.back()->w * 3) + (offset * 3) +
            (((enemies.back()->w * 3) + (offset * 3)) * i), //x
            from_top); //y
        enemies.back()->update();
    }
    
    //Red Aliens.
    for (int i = 0; i < 6; i++)
    {
        enemies.push_back(new Enemy(Red));
        enemies.back()->set_pos( from_left +
            (enemies.back()->w * 2) + (offset * 2) +
            ((enemies.back()->w + offset) * i), //x
            from_top + (enemies.back()->h + offset)); //y
        enemies.back()->update();
        //Set flagship minions.
        enemies.at(i / 3)->minions()[i % 3] = enemies.at(2 + i);
    }

    //Purple Aliens.
    for (int i = 0; i < 8; i++)
    {
        enemies.push_back(new Enemy(Purple));
        enemies.back()->set_pos( from_left +
            (enemies.back()->w + offset) +
            ((enemies.back()->w + offset) * i), //x
            from_top + ((enemies.back()->h + offset) * 2)); //y
        enemies.back()->update();
    }

    //Blue Aliens.
    for (int i = 0; i < 30; i++)
    {
        enemies.push_back(new Enemy(Blue));
        enemies.back()->set_pos(from_left +
            ((enemies.back()->w + offset) * (i % 10)), //x
            from_top + ((enemies.back()->h + offset) * (3 + (i / 10)))); //y
        enemies.back()->update();
    }

    //Set static variables to default values.
    Enemy::reset_statics();

    //Reset enemy movement counter and respawn counter.
    e_move_counter = 0;
    e_respawn_counter = 0;
    
    //Reset enemy spawning variables.
    rand_enemy_dive = rand() % (enemies.size() * 5) + 100;
    enemy_dive_counter = 0;
    
    return;
}


void Game::make_showcase()
{
    //Clear the vector.
    for (int i = 0; i < enemies.size(); ++i)
        delete enemies.at(i);
    enemies.clear();

    for (int i = 0; i <= int(Flagship); ++i)
    {
        enemies.push_back(new Enemy(E_Type(i)));
        enemies.back()->set_pos(SCREEN_W + enemies.back()->w + (i * enemies.back()->w * 3),
                                SCREEN_H / 2 - 100);
    }

    return;
}

void Game::capture_input()
{
    key = get_keypressed();

    //Force close the game.
    if (key[SDLK_ESCAPE])
    {
        close_window = true;
        return;
    }

    if (game_running)
    {
        //Player related input.
        if (ship != nullptr)
        {
            //Ship move left / right
            if (key[RIGHTARROW] && key[LEFTARROW])
                ship->dx() = 0;
            else if (key[RIGHTARROW] && ship->x + ship->w < SCREEN_W)
                ship->dx() = 1;
            else if (key[LEFTARROW] && ship->x > 0)
                ship->dx() = -1;
            else
                ship->dx() = 0;

            //Player Lasers
            if (!space_pressed && key[SPACE])
            {
                space_pressed = true;
                if (lasers.size() < MAX_LASERS)
                {
                    if (!muted)
                        laser_s.play();
                    lasers.push_back(Laser(true));
                    lasers.at(lasers.size() - 1).set_pos(
                        ship->x + (ship->w / 2) -
                        lasers.at(lasers.size() - 1).w / 2,
                        ship->y);
                }
            }
            //No holding down space to fire player lasers.
            else if (space_pressed && !key[SPACE])
                space_pressed = false;
        }

        //Muting audio
        if (!m_key_pressed && key[SDLK_m])
        {
            m_key_pressed = true;
            muted = !muted;

            if (muted)
                game_music.stop();

            if (!muted)
                game_music.play();
        }

        else if (m_key_pressed && !key[SDLK_m])
            m_key_pressed = false;

        //Pausing The Game
        if (!p_key_pressed && key[SDLK_p])
        {
            p_key_pressed = true;
            paused = !paused;
        }

        else if (p_key_pressed && !key[SDLK_p])
            p_key_pressed = false;
    }

    else
    {
        if (key[SDLK_RETURN])
        {
            reset_all();
            game_running = true;
            if (!muted)
                game_music.play();
            paused = false;
        }
    }
    
    return;
}


void Game::update_all()
{
    //Update star positions.
    background->update();

    if (game_running)
    {
        //update explosions.
        for (int i = 0; i < explosions.size(); i++)
            if (explosions.at(i).finished())
            {
                explosions.erase(explosions.begin() + i);
                i--;
            }

        //Enemy Movement
        if (!enemies.empty())
        {
            if (e_move_counter == 2)
            {
                if (Enemy::offset() == e_offset_min ||
                    Enemy::offset() == e_offset_max)
                {
                    Enemy::change_direction();
                }
                for (int i = enemies.size() - 1; i >= 0; --i)
                    enemies.at(i)->update();
                for (int i = 0; i < enemies.size(); ++i)
                {
                    if (enemies.at(i)->firing())
                    {
                        e_lasers.push_back(Laser(false));
                        e_lasers.at(e_lasers.size() - 1).set_pos(
                            enemies.at(i)->x + (enemies.at(i)->w / 2) -
                            e_lasers.at(e_lasers.size() - 1).w / 2,
                            enemies.at(i)->y);
                    }
                    enemies.at(i)->move();
                }
                
                Enemy::update_offset();
            }

            //Update the enemy movement counter.
            if (e_move_counter == 2)
                e_move_counter = 0;
            else
                e_move_counter++;

            //Enemy diving.
            if (ship != nullptr)
            {
                if (enemy_dive_counter == rand_enemy_dive)
                {
                    int rand_index = rand() % enemies.size();
                    int i = rand_index;
                    do
                    {
                        if (!enemies.at(i)->diving())
                        {
                            enemies.at(i)->diving() = true;
                            if (enemies.at(i)->t() == Flagship)
                                for (int j = 0; j < 3; j++)
                                    if (enemies.at(i)->minions()[j] != nullptr &&
                                        !enemies.at(i)->minions()[j]->diving())
                                    {
                                        enemies.at(i)->minions()[j]->diving() = true;
                                        enemies.at(i)->minions()[j]->in_group() = true;
                                    }

                            break;
                        }
                
                        ++i;
                        if (i == enemies.size())
                            i = 0;
                    } while (i != rand_index);

                    enemy_dive_counter = 0;
                    rand_enemy_dive = rand() % (enemies.size() * 5) + 100;
                }
                else
                    ++enemy_dive_counter;
            }
        }

        //Enemy formation respawn.
        else
        {
            //Update round number and text.
            if (e_respawn_counter == 0)
            {
                round++;
                if (round_text != nullptr)
                    delete round_text;
                round_text = new Image(score_f.render(std::string("Round - " +
                                                                      std::to_string(round)).c_str(), WHITE));
                round_text->rect().x = 5;
                round_text->rect().y = SCREEN_H - round_text->getRect().h - 5;
            }
            
            if (e_respawn_counter == E_RESPAWN_TIME)
            {
                make_formation();
                e_respawn_counter = 0;
            }

            else
                ++e_respawn_counter;
        }
    

        //Update e_lasers
        for (int i = 0; i < e_lasers.size(); i++)
        {
            e_lasers.at(i).update();
            if (e_lasers.at(i).y > SCREEN_H)
            {
                e_lasers.erase(e_lasers.begin() + i);
                i--;
            }
        }

        //update lasers
        for (int i = 0; i < lasers.size(); i++)
        {
            lasers.at(i).update();
            if (lasers.at(i).y + lasers.at(i).h < 0)
            {
                lasers.erase(lasers.begin() + i);
                i--;
            }
        }
    
        //Update ship.
        if (ship != nullptr)
            ship->update();

        if (ship != nullptr)
        {
            //Check for ship/enemy collisions.
            for (int i = 0; i < enemies.size(); i++)
            {
                if (intersects(ship->rect(), enemies.at(i)->rect()))
                {
                    make_explosion(*ship, 5);
                    make_explosion(*enemies.at(i));
                    delete ship;
                    ship = nullptr;
                    if (enemies.at(i)->t() == Red)
                        update_minions(enemies.at(i));
                    delete enemies.at(i);
                    enemies.erase(enemies.begin() + i);
                    //Break, there is only one ship.
                    break;
                }
            }
        }
    
        if (ship != nullptr)
        {
            //Check for ship/enemy laser collisions.
            for (int i = 0; i < e_lasers.size(); i++)
            {
                if (intersects(ship->rect(), e_lasers.at(i)))
                {
                    make_explosion(*ship, 5);
                    delete ship;
                    ship = nullptr;
                    e_lasers.erase(e_lasers.begin() + i);
                    //Break out because only one ship, if it dies,
                    //there is no reason to keep checking.
                    break;
                }
            }
        }

        //Check for enemy/laser collisions
        for (int i = 0; i < enemies.size(); i++)
        {
            for (int j = 0; j < lasers.size(); j++)
            {
                if (intersects(enemies.at(i)->rect(), lasers.at(j)))
                {
                    make_explosion(*enemies.at(i));
                    if (enemies.at(i)->t() == Red)
                        update_minions(enemies.at(i));
                
                    //increment score.
                    score += (int(enemies.at(i)->t()) + 1) * 100;
                    extra_life_counter += (int(enemies.at(i)->t()) + 1) * 100;
                
                    delete enemies.at(i);
                    enemies.erase(enemies.begin() + i);
                    i--;
                    lasers.erase(lasers.begin() + j);
                    j--;

                    //Out of bounds correcting.
                    if (i == -1)
                    {
                        if (enemies.size() == 0)
                            break;
                        else
                            i++;
                    }
                }
            }
        }

        //Check to see if another life needs to be given.
        if (extra_life_counter / EXTRA_LIFE_SCORE == 1)
        {
            extra_life_counter -= EXTRA_LIFE_SCORE;
            lives.push_back(Ship(SHIP_S));
            lives.back().set_pos(SCREEN_W - lives.back().w - 10 - ((lives.size() - 1) * (lives.back().w + 5)),
                                 SCREEN_H - lives.back().h - 10);
        }

        //Respawn timer
        if (ship == nullptr && !lives.empty())
        {
            if (respawn_counter == RESPAWN_TIME)
            {
                ship = new Ship();
                respawn_counter = 0;
                lives.pop_back();
            }
            else
                ++respawn_counter;
        }

        //Game Over timer
        if (lives.empty() && ship == nullptr)
        {
            if (game_over_counter == GAME_OVER_TIME)
            {
                game_running = false;
                make_showcase();
                game_music.stop();
                if (score > high_score)
                {
                    high_score = score;
                    save_high_score();
                    read_high_score();
                }
            }

            else
                ++game_over_counter;
        }
    }

    //Game not running.
    else
    {
        if (enemies.front()->x > 40)
        {
            for (int i = 0; i < enemies.size(); ++i)
            {
                enemies.at(i)->x -= 1;
                e_points.at(i)->rect().x = enemies.at(i)->x + (enemies.at(i)->w / 2)
                    - (e_points.at(i)->getRect().w / 2);
            }
        }
    }

    update_text();
    
    return;
}


void Game::update_text()
{
    //string updating
    std::string num_str = std::to_string(score);
    if (user_score_num != nullptr)
        delete user_score_num;
    user_score_num = new Image(score_f.render(num_str.c_str(), WHITE));
    user_score_num->rect().x = score_line.x + (score_line.w / 2)
        - (user_score_num->getRect().w / 2);
    user_score_num->rect().y = 30;

    

    if (score > high_score)
    {
        if (high_score_num != nullptr)
            delete high_score_num;
        high_score_num = new Image(score_f.render(num_str.c_str(), RED));
        high_score_num->rect().x = high_line.x + (high_line.w / 2)
            - (high_score_num->getRect().w / 2);
        high_score_num->rect().y = 30;
    }
    
    return;
}


bool Game::intersects(const Rect& r0, const Rect& r1)
{
    if ((r0.x + r0.w < r1.x) || (r0.x > r1.x + r1.w) ||
        (r0.y + r0.h < r1.y) || (r0.y > r1.y + r1.h))
    {
        return false;
    }
    
    return true;
}


void Game::draw_all()
{
    //Lock and clear surface.
    surface->lock();
    surface->fill(BLACK);

    //Draw objects to the surface.
    background->draw();

    if (game_running)
    {
        for (int i = 0; i < explosions.size(); ++i)
            explosions.at(i).draw();
    
        if (ship != nullptr)
            ship->draw();

        //Draw all lasers to the surface.
        for (int i = 0; i < lasers.size(); ++i)
            lasers.at(i).draw();
        for (int i = 0; i < e_lasers.size(); ++i)
            e_lasers.at(i).draw();

        //Draw all lives to the surface.
        for (int i = 0; i < lives.size(); ++i)
            lives.at(i).draw();

        if (lives.empty() && ship == nullptr)
            surface->put_image(*game_over_text, game_over_text->rect());
        
        else if (enemies.empty())
            surface->put_image(*e_respawn_text, e_respawn_text->rect());

        surface->put_image(*round_text, round_text->rect());
    }

    //Game not running
    else
    {
        for (int i = 0; i < e_points.size(); ++i)
            surface->put_image(*(e_points.at(i)), e_points.at(i)->rect());
        surface->put_image(*game_start_text, game_start_text->rect());
        surface->put_image(*name, name->rect());
        surface->put_image(*title, title->rect());
    }

    //Draw all enemimes to the surface.
    for (int i = 0; i < enemies.size(); ++i)
    {
        enemies.at(i)->draw();
    }
    
    surface->put_image(*user_score_text, user_score_text->rect());
    surface->put_image(*high_score_text, high_score_text->rect());
    surface->put_image(*user_score_num, user_score_num->rect());
    surface->put_image(*high_score_num, high_score_num->rect());
    surface->put_rect(score_line, {255, 255, 255});
    surface->put_rect(high_line, {255, 255, 255});

    //Unlock surface.
    surface->unlock();
    surface->flip();
    
    return;
}

/////// Private Functions ///////

void Game::update_minions(const Enemy* e_ptr)
{
    if (!enemies.empty())
    {
        if (enemies.front()->t() == Flagship)
        {
            for (int i = 0; i < 3; ++i)
                if (e_ptr == enemies.front()->minions()[i])
                {
                    enemies.front()->minions()[i] = nullptr;
                    return;
                }
        }
    }

    if (!enemies.size() > 1)
    {
        if (enemies.at(1)->t() == Flagship)
        {
            for (int i = 0; i < 3; ++i)
                if (e_ptr == enemies.at(1)->minions()[i])
                {
                    enemies.at(1)->minions()[i] = nullptr;
                    return;
                }
        }
    }
    
    return;
}


void Game::reset_all()
{
    //Make a new ship, delete if necessary.
    if (ship != nullptr)
        delete ship;
    ship = new Ship();

    //Set up enemy formation.
    make_formation();

    //Reset laser vectors.
    lasers.clear();
    e_lasers.clear();
    
    //Reset lives to 2.
    lives.clear();
    for (int i = 0; i < 2; i++)
    {
        //Uses SHIP_S constant to denote a small ship.
        lives.push_back(Ship(SHIP_S));
        lives.back().set_pos(SCREEN_W - lives.back().w - 10 - (i * (lives.back().w + 5)),
                             SCREEN_H - lives.back().h - 10);
    }

    score = 0;
    extra_life_counter = 0;

    game_over_counter = 0;

    //Reset round count
    round = 1;

    //Update round value.
    if (round_text != nullptr)
        delete round_text;
    round_text = new Image(score_f.render(std::string("Round - " +
                                                          std::to_string(round)).c_str(), WHITE));
    round_text->rect().x = 5;
    round_text->rect().y = SCREEN_H - round_text->getRect().h - 5;
    
    return;
}
