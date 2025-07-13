#ifndef _includes_
#define _defines_


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include<unistd.h>
#include<chrono>
#include<random>
#include <sstream> 
#include <iomanip>
using namespace std;
using namespace sf;

const int WINDOW_LENGTH = 1400;
const int WINDOW_WIDTH = 600;
const int RECT_LENGTH = 80;
const int RECT_WEIDTH = 100;
const int BLOCK_TOP_LEFT_CORNER_X = 260;
const int BLOCK_TOP_LEFT_CORNER_Y = 70;
const int ITEM_BAR_LENGTH = 80;
const int ITEM_BAR_WEIDTH = 500;
const int FIRST_ITEM_BAR_POS_Y = 50;
const int SECOND_ITEM_BAR_POS_Y = 150;
const int THIRD_ITEM_BAR_POS_Y = 250;
const int FORTH_ITEM_BAR_POS_Y = 350;
const int FIFTH_ITEM_BAR_POS_Y = 450;
const int FIRST_BLOCK_TOP_POS_Y = 70;
const int SECOND_BLOCK_TOP_POS_Y = 170;
const int THIRD_BLOCK_TOP_POS_Y = 270;
const int FORTH_BLOCK_TOP_POS_Y = 370;
const int FIFTH_BLOCK_TOP_POS_Y = 470;
const int FIFTH_BLOCK_DOWN_POS_Y = 570;
const int NUM_OF_ITEMS = 5;
const int NUM_OF_VERTICAL_BLOCKS = 6;
const int NUM_OF_HORIZENTAL_BLOCKS = 10;
const int PLAY_GROUND_LENGTH = 980;
const int PLAY_GROUND_WIDTH = 570;
const int NUM_OF_ROWS = 5;
const int ZOMBIE_IN_SNOW_TIME = 5;
const int SUN_PRICE = 25;
const int SUN_WAITING_TIME = 5;
const string PATH_GAME_SETTING = "./GameSetting.csv";
const string PICS_PATH = "./images/";
const string FONTS_PATH = "./fonts/";
const string MUSICS_PATH = "./musics/";
const char TYPE_DELIMETER = ',';
const char INTRO_DELIMETER = ':';
const char DATA_DELIMETER = '-';
const string MENU_SCREEN = "menu_screen.png";
const string SUNFLOWER_PNG = "Sunflower.png";
const string WALLNUT_PNG = "Wallnut_body.png";
const string PEASHOOTER_PNG = "Peashooter.png";
const string SNOW_PEASHOOTER_PNG = "snowpeashooter.png";
const string SUNFLOWER_COOLDOWN_PNG = "Sunflower_cooldown.png";
const string WALLNUT_COOLDOWN_PNG = "Walnut_cooldown.png";
const string PEASHOOTER_COOLDOWN_PNG = "Peashooter_cooldown.png";
const string SNOW_PEASHOOTER_COOLDOWN_PNG = "snowpeashooter_cooldown.png";
const string REGULAR_PROJECTILE_PNG = "regular_projectile.png";
const string SNOW_PROJECTILE_PNG= "snow_projectile.png";
const string REGULAR_PROJECTILE_NAME = "regular_projectile";
const string SNOW_PROJECTILE_NAME = "snow_projectile";
const string REGULAR_ZOMBIE_PNG = "regular_zombie";
const string NORMAL_ZOMBIE_PNG = "normal_zombie.png";
const string GIANT_ZOMBIE_PNG = "giant_zombie.png";
const string FRONTYARD_PNG = "Frontyard.png";
const string ITEM_BAR_PNG = "Item_Bar.png";
const string SUN_PNG = "sun.png";
const string SUN_ICON_BG = "sun_icon_bg.png";
const string PHASE_FONT_TTF = "Jersey25Charted-Regular.ttf";
const string GAME_OVER_FONT = "Creepster-Regular.ttf";
const string WIN_SCREEN_PNG = "Winning_Pic.png";
const string ERROR_MESSAGE = "Can't open the file"; 

const string SUNFLOWER_NAME = "sunflower";
const string PEASHOOTER_NAME = "peashooter";
const string SNOWPEASHOOTER_NAME = "snowpeashooter";
const string WALNUT_NAME = "walnut";

const string WATERMELON_NAME = "watermelon";
const string WATERMELON_PROJECTILE_PNG = "melonpult_melon.png";
const string WATERMELON_COOLDOWN_PNG = "watermelon_cooldown.png";
const string WATERMELON_PNG = "watermelon.png";
const string GRASS_WALK_MUSIC = "grass_walk.ogg";
const string GAME_OVER_MUSIC = "game_over.ogg";
const string VICTORY_MUSIC = "victory_music.ogg";


#endif