#ifndef _DEFS__H
#define _DEFS__H


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int PLAYER_WIDTH = 38;
const int PLAYER_HEIGHT = 53;

const char* WINDOW_TITLE = "SicktheDuck";

const char* PLAYER_SPRITE_FILE = "characters/allsprites.jpg";
const SDL_Rect player_movingsouth_clips[] = {
    {  0, 0, 64, 64},
    { 64, 0, 64, 64},
    {128, 0, 64, 64},
    {192, 0, 64, 64}
};
const SDL_Rect player_movingwest_clips[] = {
    {  0, 64, 64, 64},
    { 64, 64, 64, 64},
    {128, 64, 64, 64},
    {192, 64, 64, 64}
};
const SDL_Rect player_movingeast_clips[] = {
    {  0, 128, 64, 64},
    { 64, 128, 64, 64},
    {128, 128, 64, 64},
    {192, 128, 64, 64}
};
const SDL_Rect player_movingnorth_clips[] = {
    {  0, 192, 64, 64},
    { 64, 192, 64, 64},
    {128, 192, 64, 64},
    {192, 192, 64, 64}
};

const int PLAYER_FRAMES = sizeof(player_movingsouth_clips) / sizeof(int) / 4;


#endif
