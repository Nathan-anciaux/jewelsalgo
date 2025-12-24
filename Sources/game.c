#include "queue.h"
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
// Returns the direction of gravity on the game boardmake
{
    return b->direction;
}

bool board_get_picked(board *b, int *j, int *i)
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

int board_refill_size(board *b)
// Returns the size of the storage queue in the board (used only in puzzle mode).
{
    if (b->q == NULL)
    {
        return -1;
    }
    return getsize_queue(b->q);
}

bool board_read_refill(board *b, int k, cell *c)
// Reads an element from the storage queue (without removing it).
{
    if (k < board_refill_size(b))
    {
        return false;
    }

    for (int i = 0; i < board_refill_size(b) - k; i++)
    {
        enqueue(dequeue(b->q), b->q);
    }
    *c = dequeue(b->q);
    enqueue(*c, b->q);
    for (int j = 0; j < k; j++)
    {
        enqueue(dequeue(b->q), b->q);
    }
    return true;
}

/******************************************************************************/
/*+ Functions used by the GUI to create and update a new game in classic mode */
/******************************************************************************/

game *game_create_classic(int width, int height, bool lines, int target_score, int scale_score, bool nodelay, bool upgrades, bool easy_mode, bool gravity_gems,
                          bool bombs)
// Called by the interface to initialize a classic game with the specified width and height.
{
    // Game part
    game *g;
    g = malloc(sizeof(*g));
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
    g->b = malloc(sizeof(*(g->b))); // ou sizeof(board)
    g->b->width = width;
    g->b->height = height;
    g->b->content_square = malloc(g->b->width * g->b->height * sizeof(cell));
    g->b->direction = DOWN;
    g->b->q = NULL;
    g->b->index_selected_square = -1;

    // Score part
    g->score = calloc(1, sizeof(*(g->score)));
    g->score->target_score = target_score;
    g->score->scale_score = scale_score;

    // fix stable board

    bool stable = false;

    while (!stable)
    {
        for (int i = 0; i < g->b->width * g->b->height; i++)
        {
            g->b->content_square[i].object = rand() % 7;
            g->b->content_square[i].power = GEM_BASIC;
            g->b->content_square[i].countdown = 0;
        }
        while (is_there_destructible_zone_col(g->b) || is_there_destructible_zone_row(g->b))
        {
            remove_destructible_zone_col(g->b);
            remove_destructible_zone_row(g->b);
        }
        if (can_move_be_played(g->b))
        {
            stable = true;
        }
    }
    return g;
}

game *game_create_puzzle(int width, int height, bool lines, bool upgrades, short refill_mode, int ini, bool balanced, short gravity_control)
// Called by the interface to initialize a puzzle game with the specified width and height.
{
    // game part
    game *g = malloc(sizeof(*g));
    g->gravity_control = gravity_control;
    g->lines = lines;
    g->refill_mode = refill_mode;
    g->upgrades = upgrades;
    g->bombs = false;
    g->easy = true;
    g->gravity_gems = false;
    g->nodelay = true;
    g->puzzle = true;

    // board part
    g->b = malloc(sizeof(*(g->b)));
    g->b->width = width;
    g->b->height = height;
    g->b->content_square = malloc(g->b->width * g->b->height * sizeof(cell));
    for (int i = 0; i < g->b->width * g->b->height; i++)
    {
        g->b->content_square[i].object = 10;
        g->b->content_square[i].power = GEM_BASIC;
        g->b->content_square[i].countdown = 0;
    }
    g->b->direction = DOWN;
    g->b->index_selected_square = -1;

    // if balanced
    if (balanced)
    {
        // creating another array with values ​​randomized 3 at a time

        // we need to have a size%3 = 0
        int size_board_tab_rand = (g->b->height * g->b->width) - ((g->b->height * g->b->width) % 3);

        cell board_tab_rand[size_board_tab_rand];

        // tab rand for all the gems values and empty
        int tab[] = {0, 1, 2, 3, 4, 5, 6, 10};
        int tab_size = 8;

        // fill board_tab_rand
        for (int i = 0; i < size_board_tab_rand; i = i + 3)
        {

            int r = rand() % tab_size;
            int result = tab[r];

            board_tab_rand[i].object = result;
            board_tab_rand[i].power = GEM_BASIC;
            board_tab_rand[i].countdown = 0;

            board_tab_rand[i + 1].object = result;
            board_tab_rand[i + 1].power = GEM_BASIC;
            board_tab_rand[i + 1].countdown = 0;

            board_tab_rand[i + 2].object = result;
            board_tab_rand[i + 2].power = GEM_BASIC;
            board_tab_rand[i + 2].countdown = 0;
        }

        // Copy these values randomised in the first board
        int k = size_board_tab_rand - 1;
        while (k > 0)
        {
            // assignement
            int square_in_board_tab_rand = rand() % k;
            int square_in_board = rand() % g->b->width;
            int profondeur = g->b->height - 1;
            while (g->b->content_square[index_board(g->b, profondeur, square_in_board)].object != EMPTY)
            {
                profondeur--;
                if (profondeur < 0)
                {
                    square_in_board = rand() % g->b->width;
                    profondeur = g->b->height - 1;
                }
            }
            g->b->content_square[index_board(g->b, profondeur, square_in_board)] = board_tab_rand[square_in_board_tab_rand];
            if (is_there_destructible_zone_col(g->b) || is_there_destructible_zone_row(g->b))
            {
                g->b->content_square[index_board(g->b, profondeur, square_in_board)].object = EMPTY;
            }
            else
            {
                // switch
                cell tmp = board_tab_rand[square_in_board_tab_rand];
                board_tab_rand[square_in_board_tab_rand] = board_tab_rand[k];
                board_tab_rand[k] = tmp;
                k--;
            }
        }
    }
    else
    {
        bool stable = false;

        while (!stable)
        {
            for (int i = 0; i < g->b->width * g->b->height; i++)
            {
                g->b->content_square[i].object = rand() % 7;
                g->b->content_square[i].power = GEM_BASIC;
                g->b->content_square[i].countdown = 0;
            }
            while (is_there_destructible_zone_col(g->b) || is_there_destructible_zone_row(g->b))
            {
                remove_destructible_zone_col(g->b);
                remove_destructible_zone_row(g->b);
            }
            if (can_move_be_played(g->b))
            {
                stable = true;
            }
        }
    }
    // for the refill mode = 2
    g->b->q = NULL;
    // queue part
    int tab2[] = {8, 9};
    int tab2_size = 2;
    cell tmp;
    if (g->refill_mode == 0)
    {
        g->b->q = create_queue();
    }
    if (g->refill_mode == 1)
    {
        g->b->q = create_queue();
        for (int i = 0; i < ini; i++)
        {
            int r2 = rand() % tab2_size;
            int result2 = tab2[r2];

            tmp.object = result2;
            tmp.countdown = rand() % 15;
            tmp.power = GEM_BASIC;

            enqueue(tmp, g->b->q);
        }
    }

    // score part
    g->score = calloc(1, sizeof(*(g->score)));

    return g;
}

bool remove_destructible_zone_row(board *b)
// remove destructible zones
{
    cell a;
    bool change = false;
    int j;
    for (int i = 0; i < b->height; i++)
    {
        j = 0;
        while (j < b->width - 2)
        {
            if ((b->content_square[index_board(b, i, j)].object == b->content_square[index_board(b, i, j + 1)].object) &&
                (b->content_square[index_board(b, i, j + 1)].object == b->content_square[index_board(b, i, j + 2)].object))
            {
                a.object = b->content_square[index_board(b, i, j + 1)].object;
                b->content_square[index_board(b, i, j + 1)].object = rand() % 7;
                while (b->content_square[index_board(b, i, j + 1)].object == a.object)
                {
                    b->content_square[index_board(b, i, j + 1)].object = rand() % 7;
                }
                change = true;
            }
            j++;
        }
    }
    return change;
}

bool remove_destructible_zone_col(board *b)
// remove destructible zones
{
    cell a;
    bool change = false;
    int i;
    for (int j = 0; j < b->width; j++)
    {
        i = 0;
        while (i < b->height - 2)
        {
            if ((b->content_square[index_board(b, i, j)].object == b->content_square[index_board(b, i + 1, j)].object) &&
                (b->content_square[index_board(b, i + 1, j)].object == b->content_square[index_board(b, i + 2, j)].object))
            {
                a.object = b->content_square[index_board(b, i + 1, j)].object;
                b->content_square[index_board(b, i + 1, j)].object = rand() % 7;
                while (b->content_square[index_board(b, i + 1, j)].object == a.object)
                {
                    b->content_square[index_board(b, i + 1, j)].object = rand() % 7;
                }
                change = true;
            }
            i++;
        }
    }
    return change;
}
bool is_there_destructible_zone_row(board *b)
// check if there are destructible zones rows
{
    int j;
    for (int i = 0; i < b->height; i++)
    {
        j = 0;
        while (j < b->width - 2)
        {
            if (b->content_square[index_board(b, i, j)].object != EMPTY && b->content_square[index_board(b, i, j + 1)].object != EMPTY &&
                b->content_square[index_board(b, i, j + 2)].object != EMPTY)
            {
                if ((b->content_square[index_board(b, i, j)].object == b->content_square[index_board(b, i, j + 1)].object) &&
                    (b->content_square[index_board(b, i, j + 1)].object == b->content_square[index_board(b, i, j + 2)].object))
                {
                    return true;
                }
            }
            j++;
        }
    }
    return false;
}

bool is_there_destructible_zone_col(board *b)
// check if there are destructible zones cols
{
    int i;
    for (int j = 0; j < b->width; j++)
    {
        i = 0;
        while (i < b->height - 2)
        {
            if (b->content_square[index_board(b, i, j)].object != EMPTY && b->content_square[index_board(b, i + 1, j)].object != EMPTY &&
                b->content_square[index_board(b, i + 2, j)].object != EMPTY)
            {
                if ((b->content_square[index_board(b, i, j)].object == b->content_square[index_board(b, i + 1, j)].object) &&
                    (b->content_square[index_board(b, i + 1, j)].object == b->content_square[index_board(b, i + 2, j)].object))
                {
                    return true;
                }
            }
            i++;
        }
    }
    return false;
}

bool can_move_be_played(board *b)
// check if a move can be played
{
    cell tmp;

    // up square
    for (int i = 1; i < b->height; i++)
    {
        for (int j = 0; j < b->width; j++)
        {
            tmp = b->content_square[index_board(b, i, j)];
            b->content_square[index_board(b, i, j)] = b->content_square[index_board(b, i - 1, j)];
            b->content_square[index_board(b, i - 1, j)] = tmp;

            bool move = is_there_destructible_zone_row(b) || is_there_destructible_zone_col(b);

            tmp = b->content_square[index_board(b, i, j)];
            b->content_square[index_board(b, i, j)] = b->content_square[index_board(b, i - 1, j)];
            b->content_square[index_board(b, i - 1, j)] = tmp;
            if (move)
            {
                return true;
            }
        }
    }

    // down square
    for (int i = 0; i < b->height - 1; i++)
    {
        for (int j = 0; j < b->width; j++)
        {
            tmp = b->content_square[index_board(b, i, j)];
            b->content_square[index_board(b, i, j)] = b->content_square[index_board(b, i + 1, j)];
            b->content_square[index_board(b, i + 1, j)] = tmp;

            bool move = is_there_destructible_zone_row(b) || is_there_destructible_zone_col(b);

            tmp = b->content_square[index_board(b, i, j)];
            b->content_square[index_board(b, i, j)] = b->content_square[index_board(b, i + 1, j)];
            b->content_square[index_board(b, i + 1, j)] = tmp;
            if (move)
            {
                return true;
            }
        }
    }

    // left square
    for (int i = 0; i < b->height; i++)
    {
        for (int j = 1; j < b->width; j++)
        {
            tmp = b->content_square[index_board(b, i, j)];
            b->content_square[index_board(b, i, j)] = b->content_square[index_board(b, i, j - 1)];
            b->content_square[index_board(b, i, j - 1)] = tmp;

            bool move = is_there_destructible_zone_row(b) || is_there_destructible_zone_col(b);

            tmp = b->content_square[index_board(b, i, j)];
            b->content_square[index_board(b, i, j)] = b->content_square[index_board(b, i, j - 1)];
            b->content_square[index_board(b, i, j - 1)] = tmp;
            if (move)
            {
                return true;
            }
        }
    }

    // right square
    for (int i = 0; i < b->height; i++)
    {
        for (int j = 0; j < b->width - 1; j++)
        {
            tmp = b->content_square[index_board(b, i, j)];
            b->content_square[index_board(b, i, j)] = b->content_square[index_board(b, i, j + 1)];
            b->content_square[index_board(b, i, j + 1)] = tmp;

            bool move = is_there_destructible_zone_row(b) || is_there_destructible_zone_col(b);

            tmp = b->content_square[index_board(b, i, j)];
            b->content_square[index_board(b, i, j)] = b->content_square[index_board(b, i, j + 1)];
            b->content_square[index_board(b, i, j + 1)] = tmp;
            if (move)
            {
                return true;
            }
        }
    }
    return false;
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
