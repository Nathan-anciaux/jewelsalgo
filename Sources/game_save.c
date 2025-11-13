#include "game_save.h"
#include "game_scores.h"

const char *save_keyword_strings[SK_SIZE] = {
    "width",
    "height",
    "board",
    "gravity",
    "refill_queue",
    "puzzle",
    "lines",
    "upgrades",
    "nodelay",
    "easy",
    "wrap",
    "bombs",
    "refill",
    "gravity_control",
    "level",
    "level_score",
    "target_score",
    "scale_score",
    "score_array",
    "refill_count"};

bool save_game(game *, const char *)
{
    return false;
}

game *load_game(const char *)
{
    return NULL;
}
