#ifndef _DEFS__H
#define _DEFS__H


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 500;

const int PLAYER_WIDTH = 34;
const int PLAYER_HEIGHT = 58;

const int ENEMY_WIDTH = 34;
const int ENEMY_HEIGHT = 58;

const char* WINDOW_TITLE = "SicktheDuck";
const char* BACKGROUND_FILE = "allpics/backgrounds/background2.png";
const char* PLAYER_SPRITE_FILE = "allpics/player/combined_sprites.png";
const char* ENEMY_SPRITE_FILE = "allpics/player/combined_sprites.png";

const SDL_Rect player_movingsouth_clips[] = {
    {18, 80, 28, 58},
    {80, 82, 30, 56},
    {144, 82, 30, 54},
    {208, 80, 28, 58},
    {272, 82, 30, 56},
    {336, 82, 30, 54}
};
const SDL_Rect player_movingwest_clips[] = {
    {16, 152, 30, 56},
    {80, 154, 28, 54},
    {144, 154, 28, 54},
    {208, 152, 30, 56},
    {272, 154, 28, 54},
    {336, 154, 28, 54}
};
const SDL_Rect player_movingeast_clips[] = {
    {18, 222, 30, 56},
    {84, 224, 28, 54},
    {148, 224, 28, 54},
    {210, 222, 30, 56},
    {276, 224, 28, 54},
    {340, 224, 28, 54}
};
const SDL_Rect player_movingnorth_clips[] = {
    {20, 290, 26, 58},
    {82, 292, 28, 54},
    {146, 292, 26, 54},
    {210, 290, 26, 58},
    {274, 292, 28, 54},
    {340, 292, 26, 54}
};
const SDL_Rect player_idle_clips[] = {
    {14, 10, 34, 58},
    {144, 10, 32, 58},
    {80, 12, 28, 56},
    {212, 12, 28, 56}
};
const SDL_Rect player_idlepunch_south_clips[] = {
    {16, 360, 32, 58},
    {80, 360, 30, 58},
    {146, 362, 26, 56},
    {210, 362, 26, 56},
    {272, 360, 30, 58},
    {336, 360, 32, 58}
};
const SDL_Rect player_idlepunch_west_clips[] = {
    {16, 432, 28, 56},
    {80, 432, 28, 56},
    {132, 432, 42, 56},
    {194, 432, 44, 56},
    {272, 432, 28, 56},
    {336, 432, 28, 56}
};
const SDL_Rect player_idlepunch_east_clips[] = {
    {20, 502, 28, 56},
    {84, 502, 28, 56},
    {146, 502, 41, 56},
    {210, 502, 44, 56},
    {276, 502, 28, 56},
    {340, 502, 28, 56}
};
const SDL_Rect player_idlepunch_north_clips[] = {
    {16, 570, 32, 58},
    {82, 570, 30, 58},
    {146, 572, 26, 58},
    {210, 572, 26, 58},
    {274, 570, 30, 58},
    {336, 570, 32, 58}
};
const SDL_Rect player_movingpunch_south_clips[] = {
    {16, 640, 32, 58},
    {80, 640, 30, 58},
    {146, 642, 26, 56},
    {210, 642, 26, 56},
    {272, 640, 30, 58},
    {336, 640, 32, 58}
};
const SDL_Rect player_movingpunch_west_clips[] = {
    {16, 712, 28, 56},
    {80, 712, 28, 56},
    {132, 712, 42, 56},
    {194, 712, 44, 56},
    {272, 712, 28, 56},
    {336, 712, 28, 56}
};
const SDL_Rect player_movingpunch_east_clips[] = {
    {20, 782, 28, 56},
    {84, 782, 28, 56},
    {146, 782, 41, 56},
    {210, 782, 44, 56},
    {276, 782, 28, 56},
    {340, 782, 28, 56}
};
const SDL_Rect player_movingpunch_north_clips[] = {
    {16, 1130, 32, 58},
    {82, 1130, 30, 58},
    {146, 1132, 26, 58},
    {210, 1132, 26, 58},
    {274, 1130, 30, 58},
    {336, 1130, 32, 58}
};

const int PLAYER_FRAMES = sizeof(player_movingsouth_clips) / sizeof(int) / 4;


#endif // _DEFS__H
