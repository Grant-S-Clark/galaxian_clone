//Name: Grant Clark
//Date: November 26th, 2021
//File: Game.h

#ifndef GAME_CLASS_H
#define GAME_CLASS_H

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <sstream>

//Graphic Library Files
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

//My Objects
#include "Constants.h"
#include "Background.h"
#include "Entity.h"
#include "Ship.h"
#include "Enemy.h"
#include "Laser.h"
#include "Explosion.h"

class Game {
  public:
    Game();
    ~Game();

    //Main game loop.
    void run();
    
  private:

    //////////// FUNCTIONS ////////////

    void make_text();
    void read_high_score();
    void save_high_score();
    void make_formation();
    void make_showcase();
    void capture_input();
    void update_all();
    void update_text();
    bool intersects(const Rect&, const Rect&);
    void draw_all();
    
    inline
    void make_explosion(const Entity &e,
                        const int rad_modifier = 0)
    {
        explosions.push_back(Explosion(e.x + e.w / 2,
                                       e.y + e.h / 2,
                                       e.w / 2 + rad_modifier));
        if (!muted)
            explode_s.play();
        
        return;
    }

    void update_minions(const Enemy*);
    void reset_all();

    //////////// OBJECTS ////////////

    bool game_running;
    
    const int MAX_LASERS = 3;
    
    Surface* surface;
    Event event;
    KeyPressed key;

    Background* background;
    Ship* ship;

    std::vector<Ship> lives;
    const int EXTRA_LIFE_SCORE;
    int extra_life_counter;
    const int RESPAWN_TIME;
    int respawn_counter;
    const int GAME_OVER_TIME;
    int game_over_counter;

    const int E_RESPAWN_TIME;
    int e_respawn_counter;

    std::vector<Explosion> explosions;
    //Player lasers
    std::vector<Laser> lasers;
    //Enemy lasers
    std::vector<Laser> e_lasers;
    std::vector<Enemy*> enemies;
    std::vector<Image*> e_points;
    int enemy_dive_counter;
    int rand_enemy_dive;
    int e_offset_min, e_offset_max;
    int e_move_counter;
    
    bool close_window;
    bool space_pressed;

    int score, high_score;
    Font score_f, middle_screen_f, title_f;
    Image* e_respawn_text, *game_over_text, *game_start_text;
    Image* user_score_text, *high_score_text;
    Image* user_score_num, *high_score_num;
    Image* title, *name;
    Rect score_line, high_line;

    Sound laser_s, explode_s;
    Music game_music;
    bool muted, m_key_pressed;
    bool paused, p_key_pressed;

    int round;
    Image* round_text;
};

#endif
