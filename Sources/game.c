#include "game.h"
#include "alloc.h"
#include "gui_gamevars.h"

/*************************************************************************/
/*  Functions called by the GUI to retrieve information on a game board  */
/*************************************************************************/

int index_board(board *b, int i, int j)
{
    return i * b->width + j;
}

int board_get_width(board *b)
{
    return b->width;
}

int board_get_height(board *b)
{
    return b->height;
}

direction board_get_gravity(board *b)
{
    return b->direction;
}

bool board_get_picked(board *b, int *i, int *j)
{
    if (b->content_square[b->index_selected_square] != NULL)
    {
        i = b->index_selected_square / b->width;
        j = b->index_selected_square % b->width;
        return true;
    }
    return false;
}

void board_get_cell(board *, int, int, cell *)
{

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

game *game_create_classic(int width, int height, bool lines, int target_score, int scale_score, bool nodelay, bool upgrades, bool easy_mode, bool gravity_gems,
                          bool bombs)
{
    game *g = malloc(sizeof(game));
    board *g->b = malloc(sizeof(board));
    g->b = malloc(g->b->width * g->b->height * sizeof(int));
    g->b->width = width;
    g->b->height = height;
    g->lines = lines;
    g->score->target_score = target_score;
    g->score->scale_score = scale_score;
    g->nodelay = nodelay;
    g->upgrades = upgrades;
    g->easy = easy_mode;
    g->gravity_gems = gravity_gems;
    g->bombs = bombs;
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
