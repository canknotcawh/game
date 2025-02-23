#ifndef _DEFS__H
#define _DEFS__H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "SicktheDuck";

const char* PLAYER_SPRITE_FILE = "characters/movingsouth.png";
const int clips[][4] = {
    {  0, 0, 64, 64},
    { 64, 0, 64, 64},
    {128, 0, 64, 64},
    {192, 0, 64, 64}
};

const int PLAYER_FRAMES = sizeof(clips) / sizeof(int) / 4;


#endif
