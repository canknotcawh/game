#ifndef _DEFS__H
#define _DEFS__H


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int PLAYER_WIDTH = 58;
const int PLAYER_HEIGHT = 58;

const char* WINDOW_TITLE = "SicktheDuck";

const char* PLAYER_SPRITE_FILE = "allpics/player/combined_player.png";
const SDL_Rect player_movingsouth_clips[] = {
    {14, 10, 34, 58},
    {18, 80, 28, 58},
    {80, 82, 30, 56},
    {144, 82, 30, 54},
    {208, 80, 28, 58},
    {272, 82, 30, 56},
    {336, 82, 30, 54}
};
const SDL_Rect player_movingwest_clips[] = {
    {80, 12, 28, 56},
    {16, 152, 30, 56},
    {80, 154, 28, 54},
    {144, 154, 28, 54},
    {208, 152, 30, 56},
    {272, 154, 28, 54},
    {336, 154, 28, 54}
};
const SDL_Rect player_movingeast_clips[] = {
    {212, 12, 28, 56},
    {18, 222, 30, 56},
    {84, 224, 28, 54},
    {148, 224, 28, 54},
    {210, 222, 30, 56},
    {276, 224, 28, 54},
    {340, 224, 28, 54}
};
const SDL_Rect player_movingnorth_clips[] = {
    {144, 10, 32, 58},
    {20, 290, 26, 58},
    {82, 292, 26, 54},
    {146, 292, 26, 54},
    {210, 290, 26, 58},
    {274, 292, 26, 54},
    {340, 292, 26, 54}
};

const int PLAYER_FRAMES = sizeof(player_movingsouth_clips) / sizeof(int) / 4;


#endif // _DEFS__H
