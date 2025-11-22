#include "game.h"
#include "alloc.h"
#include "gui_gamevars.h"

/*************************************************************************/
/*  Functions called by the GUI to retrieve information on a game board  */
/*************************************************************************/

int index_board(board *b, int i, int j)
// Returns the clue onto the game board relative to these coordinates.
{
    return i * b->width + j;
}

int board_get_width(board *b)
// Returns the width of a game board.
{
    return b->width;
}

int board_get_height(board *b)
// Returns the height of a game board.
{
    return b->height;
}

direction board_get_gravity(board *b)
// Returns the direction of gravity on the game board
{
    return b->direction;
}

bool board_get_picked(board *b, int *i, int *j)
// Retrieves the coordinates of the currently selected square on the board (if there is one).
{
    if (b->index_selected_square != -1)
    {
        *i = b->index_selected_square / b->width;
        *j = b->index_selected_square % b->width;
        return true;
    }
    return false;
}

void board_get_cell(board *b, int j, int i, cell *c)
// Retrieves the contents of a space on the game board.
{
    c->object = b->content_square[index_board(b, i, j)].object;
    if (b->content_square[index_board(b, i, j)].object == BOMB)
    {
        c->countdown = b->content_square[index_board(b, i, j)].countdown;
    }
    else
    {
        c->power = b->content_square[index_board(b, i, j)].power;
    }
    return;
}

int board_refill_size(board *)
// Returns the size of the storage queue in the board (used only in puzzle mode).
{

    return -1;
}

bool board_read_refill(board *, int, cell *)
// Reads an element from the storage queue (without removing it).
{
    return false;
}

/******************************************************************************/
/*+ Functions used by the GUI to create and update a new game in classic mode */
/******************************************************************************/

game *game_create_classic(int width, int height, bool lines, int target_score, int scale_score, bool nodelay, bool upgrades, bool easy_mode, bool gravity_gems,
                          bool bombs)
// Called by the interface to initialize a classic game with the specified width and height.
{
    // Game part
    game *g = malloc(sizeof(*g));
    g->lines = lines;
    g->nodelay = nodelay;
    g->upgrades = upgrades;
    g->easy = easy_mode;
    g->gravity_gems = gravity_gems;
    g->bombs = bombs;
    g->gravity_control = 2;
    g->puzzle = false;
    g->refill_mode = 2;

    // Board part
    g->b = malloc(sizeof(g->b));
    g->b->width = width;
    g->b->height = height;
    g->b->content_square = calloc(g->b->width * g->b->height, sizeof(cell));
    g->b->direction = DOWN;
    g->b->q = NULL;
    g->b->index_selected_square = -1;

    // Score part
    g->score = calloc(1, sizeof(g->score));
    g->score->target_score = target_score;
    g->score->scale_score = scale_score;
    return g;
}

game *game_create_puzzle(int width, int height, bool lines, bool upgrades, short refill_mode, int ini, bool balanced, short gravity_control)
// Called by the interface to initialize a puzzle game with the specified width and height.
{
    return NULL;
}

void game_next_level(game *)
// Called by the interface when a level ends in classic mode to generate a new game board.
{
    return;
}

void game_release(game *)
// Called by the interface to free up memory allocated for a game.
{
    return;
}

game *game_clone(game *)
// Called by the interface to clone a game structure.
{
    return NULL;
}
