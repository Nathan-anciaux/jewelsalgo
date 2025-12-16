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
bool is_adjacent(game *g, int i, int j)
{
    if (i < 0 || i >= g->b->width || j < 0 || j >= g->b->height)
    {
        return EXIT_FAILURE;
    }
    int i_pos_select = g->b->index_selected_square / g->b->width;
    int j_pos_select = g->b->index_selected_square % g->b->width;
    if (((j == j_pos_select) && (i == i_pos_select + g->b->width || i == i_pos_select - g->b->width)) ||
        ((i == i_pos_select) && (j == j_pos_select + 1 || j == j_pos_select - 1)))
    {
        return true;
    }
    return false;
}

void game_handle_mouse_click(game *g, int i, int j)
{
    if (!board_get_picked(g->b, &i, &j))
    {
        return;
    }
    int index = index_board(g->b, i, j);
    if (g->b->index_selected_square == -1)
    {
        g->b->index_selected_square = index;
        return;
    }
    if (!is_adjacent(g, i, j))
    {
        g->b->index_selected_square = index;
        return;
    }
    return;
    /*
    // swap check
    g->b->index_selected_square = index;
    cell tmp = g->b->content_square[g->b->index_selected_square];
    g->b->content_square[g->b->index_selected_square] = g->b->content_square[index_board(g->b, i, j)];
    g->b->content_square[index_board(g->b, i, j)] = tmp;
    if (is_there_destructible_zone_col(g->b) || is_there_destructible_zone_row(g->b))
    {
        game_init_animation(ANIM_MOVE_GOODSWAP);
        return;
    }
    else
    {
        cell tmp1 = g->b->content_square[g->b->index_selected_square];
        g->b->content_square[g->b->index_selected_square] = g->b->content_square[index_board(g->b, i, j)];
        g->b->content_square[index_board(g->b, i, j)] = tmp1;
        game_init_animation(ANIM_MOVE_BADSWAP);
        return;
    }
    return;*/
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
