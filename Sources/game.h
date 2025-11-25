#ifndef GAME_FULL_H_
#define GAME_FULL_H_

#include "project_tools.h"
#include "queue.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

/***************************/
/*+ Définition du plateau +*/
/***************************/

typedef enum
{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    SIZE_DIRECTION
} direction;

typedef enum
{
    AMBER,
    AMETHYST,
    DIAMOND,
    EMERALD,
    RUBY,
    SAPPHIRE,
    TOPAZ,
    HYPERCUBE,
    ROCK,
    BOMB,
    EMPTY,
    ELEM_SIZE
} element;

typedef enum
{
    GEM_BASIC,
    GEM_GRAVITY,
    GEM_FLAME,
    GEM_STAR,
    GEM_SIZE
} gem_power;

typedef struct
{
    element object;
    union
    {
        gem_power power;
        short countdown;
    };
} cell;

typedef struct
{
    int width;
    int height;
    int direction;             // gravité
    int index_selected_square; // -1 si aucune cell selec
    cell *content_square;
    queue *q;
} board;

/***************************************************************************/
/*+ Fonctions appelées par l'interface pour récupérer des informations    +*/
/*+ sur un plateau de jeu                                                 +*/
/***************************************************************************/

int index_board(board *b, int i, int j);

int board_get_width(board *b);

int board_get_height(board *b);

direction board_get_gravity(board *b);

bool board_get_picked(board *b, int *i, int *j);

void board_get_cell(board *b, int i, int j, cell *c);

int board_refill_size(board *b);

bool board_read_refill(board *b, int k, cell *c);

/*************************/
/*+ Définition d'un jeu +*/
/*************************/

typedef enum
{
    SC_GLOBAL,
    SC_LEVEL,
    SC_SWAPS,
    SC_CONSUMED,
    SC_FLAMED,
    SC_STARRED,
    SC_HYPERCUBES,
    SC_GRAVITY,
    SC_TIME,
    SC_SIZE,
} score_type;

typedef struct
{
    int level;
    int level_score;
    int target_score;
    int scale_score;
    int scores[SC_SIZE];
    int refills;
} score_block;

typedef struct
{
    board *b;
    bool puzzle;
    bool lines;
    bool upgrades;
    short gravity_control;
    bool nodelay;
    bool easy;
    bool gravity_gems;
    bool bombs;
    short refill_mode;
    score_block *score;
} game;

/**********************************************************************************/
/* Fonctions utilisées par l'interface pour créer et mettre à jour un nouveau jeu */
/**********************************************************************************/

game *game_create_classic(int width, int height, bool lines, int target_score,
                          int scale_score, bool nodelay, bool upgrades,
                          bool easy_mode, bool gravity_gems, bool bombs);

game *game_create_puzzle(int width, int height, bool lines, bool upgrades,
                         short refill_mode, int ini, bool balanced,
                         short gravity_control);

void game_next_level(game *g);

void game_release(game *g);

game *game_clone(game *g);

bool is_there_destructible_zone_row(board *b);

bool is_there_destructible_zone_col(board *b);

bool can_move_be_played(board *b);

#endif // GAME_FULL_H_
