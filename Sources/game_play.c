#include "game_play.h"

/***************************************************************************************************************/
/*+ Function that starts an animation (or return to idle mode). Already written: call it but DO NOT MODIFY it +*/
/***************************************************************************************************************/

anim_type game_anim_signal = ANIM_SIZE;

void game_init_animation(anim_type type)
{
    if (type < 0 || type >= ANIM_SIZE)
    {
        fprintf(stderr, "Invalid animation type in game_init_animation.\n");
        exit(EXIT_FAILURE);
    }
    game_anim_signal = type;
}

/**********************************************************************************************************/
/*+ Functions that ends the game. Already written: call it when the player has lost but DO NOT MODIFY it +*/
/**********************************************************************************************************/

end_signal game_end_signal = SIGNAL_SIZE;

void game_classic_score(void)
{
    game_end_signal = SIGNAL_END_LEVEL; // Signals the end of a level in classic mode.
}

void game_classic_end(void)
{
    game_end_signal = SIGNAL_END_CLGAME; // Signals the end of the game in classic mode.
}

void game_puzzle_end(void)
{
    game_end_signal = SIGNAL_END_PUZZLE; // Signals the end of a puzzle game.
}

/**********************************************************/
/*+ Functions called by the GUI to modify the game state +*/
/**********************************************************/

void game_handle_mouse_click(game *, int, int)
{
    return;
}

void game_handle_hint_request(game *)
{
    return;
}

void game_handle_detonation(game *)
{
    return;
}

void game_set_gravity(game *, direction)
{
    return;
}

void game_cycle_gravity(game *)
{
    return;
}

void game_handle_refill(game *, int)
{
    return;
}

void game_end_animation(game *, anim_type)
{
    return;
}

/*************************************************************************************/
/*+ Functions used by the GUI to retrieve information when it performs an animation +*/
/*+ You must implement these functions so that the GUI can display the animations   +*/
/*************************************************************************************/

direction game_get_cell_move(game *, int, int, int *)
{
    return UP;
}

gem_kill game_get_cell_kill(game *, int, int)
{
    return KILL_NOKILL;
}

bool game_retrieve_hint(game *, int *, int *)
{
    return false;
}
