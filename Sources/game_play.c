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
    if (i < 0 || i >= g->b->height || j < 0 || j >= g->b->width)
    {
        return false;
    }
    if (g->b->index_selected_square == -1)
    {
        return false;
    }
    int i_pos_select = g->b->index_selected_square / g->b->width;
    int j_pos_select = g->b->index_selected_square % g->b->width;
    int dist_i = abs(i - i_pos_select);
    int dist_j = abs(j - j_pos_select);
    return ((dist_i == 1 && dist_j == 0) || (dist_i == 0 && dist_j == 1));
}

void game_handle_mouse_click(game *g, int j, int i)
{
    int index = index_board(g->b, i, j);

    if (g->b->index_selected_square == -1)
    {
        if (g->b->content_square[index].object != EMPTY &&
            g->b->content_square[index].object != ROCK)
        {
            g->b->index_selected_square = index;
        }
        game_init_animation(ANIM_IDLE);
        return;
    }

    if (index == g->b->index_selected_square)
    {
        g->b->index_selected_square = -1;
        game_init_animation(ANIM_IDLE);
        return;
    }

    if (!is_adjacent(g, i, j))
    {
        if (g->b->content_square[index].object != EMPTY &&
            g->b->content_square[index].object != ROCK)
        {
            g->b->index_selected_square = index;
        }
        else
        {
            g->b->index_selected_square = -1;
        }
        game_init_animation(ANIM_IDLE);
        return;
    }

    int ind1 = g->b->index_selected_square;
    int ind2 = index_board(g->b, i, j);

    if (g->b->content_square[ind2].object == ROCK ||
        g->b->content_square[ind2].object == EMPTY)
    {
        g->b->index_selected_square = -1;
        game_init_animation(ANIM_IDLE);
        return;
    }

    cell tmp = g->b->content_square[ind1];
    g->b->content_square[ind1] = g->b->content_square[ind2];
    g->b->content_square[ind2] = tmp;

    bool valid_move = false;
    if (g->lines)
    {
        valid_move = is_there_destructible_zone_row(g->b) ||
                     is_there_destructible_zone_col(g->b);
    }
    else
    {
        valid_move = is_there_destrucible_zone_sccs(g->b);
    }

    if (valid_move)
    {
        g->b->index_selected_square = -1;
        game_init_animation(ANIM_MOVE_GOODSWAP);
        game_init_animation(ANIM_KILL);
    }
    else
    {
        tmp = g->b->content_square[ind1];
        g->b->content_square[ind1] = g->b->content_square[ind2];
        g->b->content_square[ind2] = tmp;

        g->b->index_selected_square = -1;
        game_init_animation(ANIM_MOVE_BADSWAP);
    }
}

void game_handle_hint_request(game *)
{
    return;
}

void game_handle_detonation(game *)
{
    return;
}

void game_set_gravity(game *g, direction dir)
{
    /*if (g->gravity_control == 0) {
        return;
    }

    if (g->gravity_control == 1) {
        if (dir == DOWN || dir == RIGHT || dir == UP || dir == LEFT) {
            g->b->direction = dir;
        }
    }

    else if (g->gravity_control == 2) {
        if (dir == DOWN || dir == RIGHT || dir == UP || dir == LEFT) {
            g->b->direction = dir;
        }
    }

    else if (g->gravity_control == 3) {
        if (dir == DOWN || dir == RIGHT) {
            g->b->direction = dir;
        }
    }

    else if (g->gravity_control == 4) {
        if (dir == DOWN || dir == LEFT || dir == UP || dir == RIGHT) {
            g->b->direction = dir;
        }
    }

    game_init_animation(ANIM_IDLE);*/
}

void game_cycle_gravity(game *g)
{
    return;
}

void game_handle_refill(game *g, int)
{
    return;
}

void marquer_gemmes(game *g)
{
    int w = g->b->width;
    int h = g->b->height;
    int size = w * h;

    for (int k = 0; k < size; k++)
    {
        g->b->gems_cible[k] = false;
    }

    if (g->lines)
    {
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                element type = g->b->content_square[i * w + j].object;
                if (type != EMPTY && type != ROCK && type != BOMB)
                {
                    int count = 1;
                    while (j + count < w &&
                           g->b->content_square[i * w + (j + count)].object == type)
                    {
                        count++;
                    }
                    if (count >= 3)
                    {
                        for (int k = 0; k < count; k++)
                        {
                            g->b->gems_cible[i * w + (j + k)] = true;
                        }
                    }
                    j += count - 1;
                }
            }
        }

        for (int j = 0; j < w; j++)
        {
            for (int i = 0; i < h; i++)
            {
                element type = g->b->content_square[i * w + j].object;
                if (type != EMPTY && type != ROCK && type != BOMB)
                {
                    int count = 1;
                    while (i + count < h &&
                           g->b->content_square[(i + count) * w + j].object == type)
                    {
                        count++;
                    }
                    if (count >= 3)
                    {
                        for (int k = 0; k < count; k++)
                        {
                            g->b->gems_cible[(i + k) * w + j] = true;
                        }
                    }
                    i += count - 1;
                }
            }
        }
    }
    else
    {
        bool *visited = calloc(size, sizeof(bool));

        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                int idx = i * w + j;
                element type = g->b->content_square[idx].object;

                if (type != EMPTY && type != ROCK && type != BOMB && !visited[idx])
                {
                    for (int k = 0; k < size; k++)
                        visited[k] = false;

                    int scc_size = find_sccs(g->b, i, j, type, visited);

                    if (scc_size >= 4)
                    {
                        for (int k = 0; k < size; k++)
                        {
                            if (visited[k])
                            {
                                g->b->gems_cible[k] = true;
                            }
                        }
                    }
                }
            }
        }
        free(visited);
    }
}

void gems_gravity(board *b)
{
    printf("gravity gemmes commencees\n");
    int w = b->width;
    int h = b->height;
    int i, j, dest, index, dest_index;

    if (b->direction == DOWN)
    {
        for (j = 0; j < w; j++)
        {
            dest = h - 1;
            for (i = h - 1; i >= 0; i--)
            {
                index = i * w + j;
                if (b->content_square[index].object != EMPTY)
                {
                    if (dest != i)
                    {
                        dest_index = dest * w + j;
                        b->content_square[dest_index] = b->content_square[index];
                        b->content_square[index].object = EMPTY;
                    }
                    dest--;
                }
            }
        }
    }
    else if (b->direction == UP)
    {
        for (j = 0; j < w; j++)
        {
            dest = 0;
            for (i = 0; i < h; i++)
            {
                index = i * w + j;
                if (b->content_square[index].object != EMPTY)
                {
                    if (dest != i)
                    {
                        dest_index = dest * w + j;
                        b->content_square[dest_index] = b->content_square[index];
                        b->content_square[index].object = EMPTY;
                    }
                    dest++;
                }
            }
        }
    }
    else if (b->direction == LEFT)
    {
        for (i = 0; i < h; i++)
        {
            dest = 0;
            for (j = 0; j < w; j++)
            {
                index = i * w + j;
                if (b->content_square[index].object != EMPTY)
                {
                    if (dest != j)
                    {
                        dest_index = i * w + dest;
                        b->content_square[dest_index] = b->content_square[index];
                        b->content_square[index].object = EMPTY;
                    }
                    dest++;
                }
            }
        }
    }
    else if (b->direction == RIGHT)
    {
        for (i = 0; i < h; i++)
        {
            dest = w - 1;
            for (j = w - 1; j >= 0; j--)
            {
                index = i * w + j;
                if (b->content_square[index].object != EMPTY)
                {
                    if (dest != j)
                    {
                        dest_index = i * w + dest;
                        b->content_square[dest_index] = b->content_square[index];
                        b->content_square[index].object = EMPTY;
                    }
                    dest--;
                }
            }
        }
    }
    printf("gravity gemmes terminees\n");
}
void fill_empty_cells(game *g)
{
    if (!g->puzzle)
    {
        int size = g->b->width * g->b->height;
        for (int k = 0; k < size; k++)
        {
            if (g->b->content_square[k].object == EMPTY)
            {
                g->b->content_square[k].object = rand() % 7;
                g->b->content_square[k].power = GEM_BASIC;
                g->b->content_square[k].countdown = 0;
            }
        }
    }
}

void game_end_animation(game *g, anim_type type)
{
    int size = g->b->width * g->b->height;

    if (type == ANIM_IDLE)
    {
        return;
    }

    else if (type == ANIM_MOVE_BADSWAP)
    {
        return;
    }

    else if (type == ANIM_MOVE_GOODSWAP)
    {
        marquer_gemmes(g);
        return;
    }

    else if (type == ANIM_KILL)
    {
        for (int k = 0; k < size; k++)
        {
            if (g->b->gems_cible[k])
            {
                g->b->content_square[k].object = EMPTY;
                g->b->gems_cible[k] = false;
            }
        }
        game_init_animation(ANIM_MOVE_FALL);
        return;
    }

    else if (type == ANIM_MOVE_FALL)
    {
        gems_gravity(g->b);

        bool new_combo = false;
        if (g->lines)
        {
            new_combo = is_there_destructible_zone_row(g->b) ||
                        is_there_destructible_zone_col(g->b);
        }
        else
        {
            new_combo = is_there_destrucible_zone_sccs(g->b);
        }

        if (new_combo)
        {
            marquer_gemmes(g);
            game_init_animation(ANIM_KILL);
            return;
        }

        if (!g->puzzle)
        {
            fill_empty_cells(g);

            bool fill_combo = false;
            if (g->lines)
            {
                fill_combo = is_there_destructible_zone_row(g->b) ||
                             is_there_destructible_zone_col(g->b);
            }
            else
            {
                fill_combo = is_there_destrucible_zone_sccs(g->b);
            }

            if (fill_combo)
            {
                marquer_gemmes(g);
                game_init_animation(ANIM_KILL);
                return;
            }
        }

        game_init_animation(ANIM_IDLE);
        return;
    }
    return;
}

/*************************************************************************************/
/*+ Functions used by the GUI to retrieve information when it performs an animation +*/
/*+ You must implement these functions so that the GUI can display the animations   +*/
/*************************************************************************************/

direction game_get_cell_move(game *g, int j, int i, int *distance)
{
    *distance = 0;

    if (i < 0 || i >= g->b->height || j < 0 || j >= g->b->width)
    {
        return UP;
    }

    int index = i * g->b->width + j;

    if (g->b->content_square[index].object == EMPTY)
    {
        return UP;
    }

    if (g->b->direction == DOWN)
    {
        for (int k = i + 1; k < g->b->height; k++)
        {
            int idx_below = k * g->b->width + j;
            if (g->b->content_square[idx_below].object == EMPTY)
            {
                *distance = k - i;
                return DOWN;
            }
        }
    }
    else if (g->b->direction == UP)
    {
        for (int k = i - 1; k >= 0; k--)
        {
            int idx_above = k * g->b->width + j;
            if (g->b->content_square[idx_above].object == EMPTY)
            {
                *distance = i - k;
                return UP;
            }
        }
    }
    else if (g->b->direction == LEFT)
    {
        for (int k = j - 1; k >= 0; k--)
        {
            int idx_left = i * g->b->width + k;
            if (g->b->content_square[idx_left].object == EMPTY)
            {
                *distance = j - k;
                return LEFT;
            }
        }
    }
    else if (g->b->direction == RIGHT)
    {
        for (int k = j + 1; k < g->b->width; k++)
        {
            int idx_right = i * g->b->width + k;
            if (g->b->content_square[idx_right].object == EMPTY)
            {
                *distance = k - j;
                return RIGHT;
            }
        }
    }

    return UP;
}

gem_kill game_get_cell_kill(game *g, int j, int i)
{
    int index = i * g->b->width + j;

    if (g->b->gems_cible != NULL && g->b->gems_cible[index])
    {
        if (g->b->content_square[index].power == GEM_FLAME)
        {
            return KILL_EXPLODE;
        }
        else if (g->b->content_square[index].power == GEM_STAR)
        {
            return KILL_SHATTER;
        }
        else
        {
            return KILL_SHRINK;
        }
    }
    return KILL_NOKILL;
}

bool game_retrieve_hint(game *g, int *j, int *i)
{
    for (int row = 0; row < g->b->height; row++)
    {
        for (int col = 0; col < g->b->width; col++)
        {
            if (col + 1 < g->b->width)
            {
                int idx1 = row * g->b->width + col;
                int idx2 = row * g->b->width + (col + 1);

                cell tmp = g->b->content_square[idx1];
                g->b->content_square[idx1] = g->b->content_square[idx2];
                g->b->content_square[idx2] = tmp;

                bool valid = false;
                if (g->lines)
                {
                    valid = is_there_destructible_zone_row(g->b) ||
                            is_there_destructible_zone_col(g->b);
                }
                else
                {
                    valid = is_there_destrucible_zone_sccs(g->b);
                }

                tmp = g->b->content_square[idx1];
                g->b->content_square[idx1] = g->b->content_square[idx2];
                g->b->content_square[idx2] = tmp;

                if (valid)
                {
                    *i = row;
                    *j = col;
                    return true;
                }
            }

            if (row + 1 < g->b->height)
            {
                int idx1 = row * g->b->width + col;
                int idx2 = (row + 1) * g->b->width + col;

                cell tmp = g->b->content_square[idx1];
                g->b->content_square[idx1] = g->b->content_square[idx2];
                g->b->content_square[idx2] = tmp;

                bool valid = false;
                if (g->lines)
                {
                    valid = is_there_destructible_zone_row(g->b) ||
                            is_there_destructible_zone_col(g->b);
                }
                else
                {
                    valid = is_there_destrucible_zone_sccs(g->b);
                }

                tmp = g->b->content_square[idx1];
                g->b->content_square[idx1] = g->b->content_square[idx2];
                g->b->content_square[idx2] = tmp;

                if (valid)
                {
                    *i = row;
                    *j = col;
                    return true;
                }
            }
        }
    }

    return false;
}
