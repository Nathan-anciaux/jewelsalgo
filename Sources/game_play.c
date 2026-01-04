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
    int dist;
    game_init_animation(ANIM_IDLE);

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
        game_init_animation(ANIM_MOVE_BADSWAP);
    }
}

void game_handle_hint_request(game *g)
{
    int hint_i, hint_j;
    if (game_retrieve_hint(g, &hint_j, &hint_i))
    {
        game_init_animation(ANIM_HINT);
    }
}

void game_handle_detonation(game *g)
{
    if (g->nodelay)
    {
        return;
    }
    bool destructible = false;
    if (g->lines)
    {
        destructible = is_there_destructible_zone_row(g->b) ||
                       is_there_destructible_zone_col(g->b);
    }
    else
    {
        destructible = is_there_destrucible_zone_sccs(g->b);
    }

    if (destructible)
    {
        marquer_gemmes(g);
        game_init_animation(ANIM_KILL);
    }
}

void game_set_gravity(game *g, direction dir)
{

    if (g->gravity_control == 0)
    {
        return;
    }

    if (g->gravity_control == 1)
    {
        if (dir == DOWN || dir == RIGHT || dir == UP || dir == LEFT)
        {
            g->b->direction = dir;
        }
    }

    else if (g->gravity_control == 2)
    {
        if (dir == DOWN || dir == RIGHT || dir == UP || dir == LEFT)
        {
            g->b->direction = dir;
        }
    }

    else if (g->gravity_control == 3)
    {
        if (dir == DOWN || dir == RIGHT)
        {
            g->b->direction = dir;
        }
    }

    else if (g->gravity_control == 4)
    {
        if (dir == DOWN || dir == LEFT || dir == UP || dir == RIGHT)
        {
            g->b->direction = dir;
        }
    }

    game_init_animation(ANIM_IDLE);
}

void game_cycle_gravity(game *g)
{
    if (g->gravity_control == 0)
    {
        return;
    }
    direction new_dir = g->b->direction;
    if (g->gravity_control == 1 || g->gravity_control == 2)
    {
        if (g->b->direction == DOWN)
            new_dir = RIGHT;
        else if (g->b->direction == RIGHT)
            new_dir = UP;
        else if (g->b->direction == UP)
            new_dir = LEFT;
        else
            new_dir = DOWN;
    }
    else if (g->gravity_control == 3)
    {
        if (g->b->direction == DOWN)
            new_dir = RIGHT;
        else
            new_dir = DOWN;
    }
    else if (g->gravity_control == 4)
    {
        if (g->b->direction == DOWN)
            new_dir = LEFT;
        else if (g->b->direction == LEFT)
            new_dir = UP;
        else if (g->b->direction == UP)
            new_dir = RIGHT;
        else
            new_dir = DOWN;
    }
    g->b->direction = new_dir;
    bool tomber = false;
    if (g->b->direction == DOWN)
    {
        for (int j = 0; j < g->b->width; j++)
        {
            for (int i = g->b->height - 2; i >= 0; i--)
            {
                int current = i * g->b->width + j;
                int bas = (i + 1) * g->b->width + j;
                if (g->b->content_square[current].object != EMPTY &&
                    g->b->content_square[bas].object == EMPTY)
                {
                    tomber = true;
                }
            }
        }
    }
    else if (g->b->direction == UP)
    {
        for (int j = 0; j < g->b->width; j++)
        {
            for (int i = 1; i < g->b->height; i++)
            {
                int current = i * g->b->width + j;
                int haut = (i - 1) * g->b->width + j;
                if (g->b->content_square[current].object != EMPTY &&
                    g->b->content_square[haut].object == EMPTY)
                {
                    tomber = true;
                }
            }
        }
    }
    else if (g->b->direction == LEFT)
    {
        for (int i = 0; i < g->b->height; i++)
        {
            for (int j = 1; j < g->b->width; j++)
            {
                int current = i * g->b->width + j;
                int gauche = i * g->b->width + (j - 1);
                if (g->b->content_square[current].object != EMPTY &&
                    g->b->content_square[gauche].object == EMPTY)
                {
                    tomber = true;
                }
            }
        }
    }
    else if (g->b->direction == RIGHT)
    {
        for (int i = 0; i < g->b->height; i++)
        {
            for (int j = g->b->width - 2; j >= 0; j--)
            {
                int current = i * g->b->width + j;
                int droite = i * g->b->width + (j + 1);
                if (g->b->content_square[current].object != EMPTY &&
                    g->b->content_square[droite].object == EMPTY)
                {
                    tomber = true;
                }
            }
        }
    }
    if (tomber)
    {
        game_init_animation(ANIM_MOVE_FALL);
    }
    else
    {
        game_init_animation(ANIM_IDLE);
    }
}

void game_handle_refill(game *g, int k)
{
    if (!g->puzzle)
        return;
    if (g->b->q == NULL)
        return;
    if (k < 0 || k >= getsize_queue(g->b->q))
        return;

    cell gem;
    queue *temp = copy_queue(g->b->q);

    for (int i = 0; i < k; i++)
    {
        dequeue(temp);
    }

    gem = dequeue(temp);
    delete_queue(temp);

    int w = g->b->width;
    int h = g->b->height;
    int position = -1;

    if (g->b->direction == DOWN)
    {
        for (int j = 0; j < w; j++)
        {
            int i = 0;
            int current = i * w + j;
            if (g->b->content_square[current].object == EMPTY)
            {
                position = current;
                break;
            }
        }
    }
    else if (g->b->direction == UP)
    {
        for (int j = 0; j < w; j++)
        {
            int i = h - 1;
            int current = i * w + j;
            if (g->b->content_square[current].object == EMPTY)
            {
                position = current;
                break;
            }
        }
    }
    else if (g->b->direction == LEFT)
    {
        for (int i = 0; i < h; i++)
        {
            int j = w - 1;
            int current = i * w + j;
            if (g->b->content_square[current].object == EMPTY)
            {
                position = current;
                break;
            }
        }
    }
    else if (g->b->direction == RIGHT)
    {
        for (int i = 0; i < h; i++)
        {
            int j = 0;
            int current = i * w + j;
            if (g->b->content_square[current].object == EMPTY)
            {
                position = current;
                break;
            }
        }
    }

    if (position == -1)
        return;

    g->b->content_square[position] = gem;

    for (int i = 0; i < k; i++)
    {
        enqueue(dequeue(g->b->q), g->b->q);
    }
    dequeue(g->b->q);
    for (int i = k; i < getsize_queue(g->b->q); i++)
    {
        enqueue(dequeue(g->b->q), g->b->q);
    }

    game_init_animation(ANIM_MOVE_FALL);
}

void marquer_gemmes(game *g)
{
    int size = g->b->width * g->b->height;

    for (int k = 0; k < size; k++)
    {
        g->b->gems_cible[k] = false;
    }
    if (g->lines)
    {
        for (int i = 0; i < g->b->height; i++)
        {
            for (int j = 0; j < g->b->width; j++)
            {

                element type = g->b->content_square[i * g->b->width + j].object;
                if (type == BOMB && g->b->content_square[i * g->b->width + j].countdown == 0)
                {
                    for (int b1 = i - 1; b1 < i + 2; b1++)
                    {
                        if (b1 < 0 || g->b->width < b1)
                        {
                            continue;
                        }
                        for (int b2 = j - 1; b2 < j + 2; b2++)
                        {
                            if (b2 < 0 || g->b->height < b2)
                            {
                                continue;
                            }
                            g->b->gems_cible[b1 * g->b->width + b2] = true;
                        }
                    }
                }
                else if (g->b->content_square[i * g->b->width + j].power == GEM_STAR)
                {

                    for (int b1 = 0; b1 < g->b->height; b1++)
                    {
                        g->b->gems_cible[b1 * g->b->width + j] = true;
                    }
                    for (int b2 = 0; b2 < g->b->width; b2++)
                    {
                        g->b->gems_cible[i * g->b->width + b2] = true;
                    }
                }

                else
                {
                    int count = 1;
                    while (j + count < g->b->width &&
                           g->b->content_square[i * g->b->width + (j + count)].object == type)
                    {
                        count++;
                    }
                    if (count >= 3)
                    {
                        for (int k = 0; k < count; k++)
                        {
                            g->b->gems_cible[i * g->b->width + (j + k)] = true;
                        }
                    }
                    j += count - 1;
                }
            }
        }

        for (int j = 0; j < g->b->width; j++)
        {
            for (int i = 0; i < g->b->height; i++)
            {
                element type = g->b->content_square[i * g->b->width + j].object;
                if (type != EMPTY && type != ROCK && type != BOMB)
                {
                    int count = 1;
                    while (i + count < g->b->height &&
                           g->b->content_square[(i + count) * g->b->width + j].object == type)
                    {
                        count++;
                    }
                    if (count >= 3)
                    {
                        for (int k = 0; k < count; k++)
                        {
                            g->b->gems_cible[(i + k) * g->b->width + j] = true;
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

        for (int i = 0; i < g->b->height; i++)
        {
            for (int j = 0; j < g->b->width; j++)
            {
                int idx = i * g->b->width + j;
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
    // printf("gravity gemmes commencees\n");
    int i, j, dest, index, dest_index;

    if (b->direction == DOWN)
    {
        for (j = 0; j < b->width; j++)
        {
            dest = b->height - 1;
            for (i = b->height - 1; i >= 0; i--)
            {
                index = i * b->width + j;
                if (b->content_square[index].object != EMPTY)
                {
                    if (dest != i)
                    {
                        dest_index = dest * b->width + j;
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
        for (j = 0; j < b->width; j++)
        {
            dest = 0;
            for (i = 0; i < b->height; i++)
            {
                index = i * b->width + j;
                if (b->content_square[index].object != EMPTY)
                {
                    if (dest != i)
                    {
                        dest_index = dest * b->width + j;
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
        for (i = 0; i < b->height; i++)
        {
            dest = 0;
            for (j = 0; j < b->width; j++)
            {
                index = i * b->width + j;
                if (b->content_square[index].object != EMPTY)
                {
                    if (dest != j)
                    {
                        dest_index = i * b->width + dest;
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
        for (i = 0; i < b->height; i++)
        {
            dest = b->width - 1;
            for (j = b->width - 1; j >= 0; j--)
            {
                index = i * b->width + j;
                if (b->content_square[index].object != EMPTY)
                {
                    if (dest != j)
                    {
                        dest_index = i * b->width + dest;
                        b->content_square[dest_index] = b->content_square[index];
                        b->content_square[index].object = EMPTY;
                    }
                    dest--;
                }
            }
        }
    }
    // printf("gravity gemmes terminees\n");
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
                int random = rand() % 8, randomV = rand() % 7;
                if (g->bombs && random == 7)
                {
                    g->b->content_square[k].object = BOMB;
                    g->b->content_square[k].countdown = rand() % 10;
                }
                else if (g->gravity_gems)
                {
                    int randg = rand() % 100;
                    if (randg < 5)
                    {
                        g->b->content_square[k].object = randomV;
                        g->b->content_square[k].power = (rand() % 3) + 1;
                        g->b->content_square[k].countdown = 0;
                    }
                    else
                    {
                        g->b->content_square[k].object = random;
                        g->b->content_square[k].power = GEM_BASIC;
                        g->b->content_square[k].countdown = 0;
                    }
                }
                else
                {
                    g->b->content_square[k].object = random;
                    g->b->content_square[k].power = GEM_BASIC;
                    g->b->content_square[k].countdown = 0;
                }
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
        game_init_animation(ANIM_MOVE_ROLLBACK);
        return;
    }

    else if (type == ANIM_MOVE_GOODSWAP)
    {
        marquer_gemmes(g);
        return;
    }

    else if (type == ANIM_KILL)
    {
        int total_points = 0;
        int flame_count = 0;
        int star_count = 0;
        int hypercube_count = 0;
        int bomb_count = 0;

        for (int k = 0; k < size; k++)
        {
            if (g->b->gems_cible[k])
            {
                element obj = g->b->content_square[k].object;
                gem_power power = g->b->content_square[k].power;

                if (obj >= AMBER && obj <= TOPAZ)
                {
                    if (power == GEM_BASIC || power == GEM_GRAVITY)
                    {
                        total_points += 100;
                    }
                    else if (power == GEM_FLAME)
                    {
                        total_points += 400;
                        flame_count++;
                    }
                    else if (power == GEM_STAR)
                    {
                        total_points += 800;
                        star_count++;
                    }
                }
                else if (obj == HYPERCUBE)
                {
                    total_points += 1000;
                    hypercube_count++;
                }
                else if (obj == ROCK || obj == BOMB)
                {
                    total_points += 50;
                    bomb_count++;
                }
                if (obj == BOMB)
                {
                    g->b->content_square[k].countdown--;
                }
                g->b->content_square[k].object = EMPTY;
                g->b->gems_cible[k] = false;
            }
        }
        g->score->scores[SC_GLOBAL] += total_points;
        g->score->level_score += total_points;
        g->score->scores[SC_FLAMED] += flame_count;
        g->score->scores[SC_STARRED] += star_count;
        g->score->scores[SC_HYPERCUBES] += hypercube_count;
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
    else if (type == ANIM_HINT)
    {
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
