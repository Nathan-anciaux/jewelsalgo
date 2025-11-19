#include "game.h"
#include "alloc.h"
#include "gui_gamevars.h"

/*************************************************************************/
/*  Functions called by the GUI to retrieve information on a game board  */
/*************************************************************************/

int board_get_width(board *)
{
    return 0;
}

int board_get_height(board *)
{
    return 0;
}

direction board_get_gravity(board *)
{
    return UP;
}

bool board_get_picked(board *, int *, int *)
{
    return false;
}

void board_get_cell(board *b, int i, int j, cell *c)
{
    c->object = b->content_square->object;
    c->power = b->content_square->power;
    c->countdown = b->content_square->countdown;
    return;
}

int board_refill_size(board *)
{
    return -1;
}

bool board_read_refill(board *, int, cell *)
{
    return false;
}

/******************************************************************************/
/*+ Functions used by the GUI to create and update a new game in classic mode */
/******************************************************************************/

game *game_create_classic(int, int, bool, int, int, bool, bool, bool, bool,
                          bool)
{
    return NULL;
}

game *game_create_puzzle(int, int, bool, bool, short, int, bool, short)
{
    return NULL;
}

void game_next_level(game *)
{
    return;
}

void game_release(game *)
{
    return;
}

game *game_clone(game *)
{
    return NULL;
}
