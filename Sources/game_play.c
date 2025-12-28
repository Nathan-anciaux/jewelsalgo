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
    if (g->b->index_selected_square ==-1)
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
        g->b->index_selected_square = index;
        game_init_animation(ANIM_IDLE);
        return;
    }
    if (index == g->b->index_selected_square){
        g->b->index_selected_square = -1;
        game_init_animation(ANIM_IDLE);
        return;
    }
    if (is_adjacent(g,i,j)){
        int ind1 = g->b->index_selected_square;
        int ind2 = index_board(g->b,i,j);
        cell tmp = g->b->content_square[ind1];
        g->b->content_square[ind1] = g->b->content_square[ind2];
        g->b->content_square[ind2] = tmp;
        if (is_there_destructible_zone_col(g->b) || is_there_destructible_zone_row(g->b))
        {
            g->b->index_selected_square = -1;
            game_init_animation(ANIM_MOVE_GOODSWAP);
        }
        else 
        {
            tmp = g->b->content_square[ind1];
            g->b->content_square[ind1] = g->b->content_square[ind2];
            g->b->content_square[ind2] = tmp;
            g->b->index_selected_square = -1;
            game_init_animation(ANIM_MOVE_BADSWAP);
        }
        return;
        
    }
    else
    {
        g->b->index_selected_square = index;
        game_init_animation(ANIM_IDLE);
        return;
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

void game_set_gravity(game *, direction)
{
    return;
}

void game_cycle_gravity(game *g)
{
    return;
}

void game_handle_refill(game *g, int)
{
    return;
}

void marquer_gemmes(game* g)
{
    int count;
     for( int k = 0; k <g->b->width * g->b->height;k++){
        g->b->gems_cible[k] = false;
    }
    for (int i= 0;i < g->b->height;i++)
    {
        for (int j = 0; j< g->b->width;j++)
        {
            int index = i * g->b->width + j;
            element type = g->b->content_square[index].object;
            if (type != EMPTY)
            {
                count = 1;
                while (j + count < g->b->width && g->b->content_square[i * g->b->width +(j + count)].object == type)
                {
                    count ++;
                }
            
                if (count >=3)
                {
                    for (int k = 0;k <count ;k++)
                    {
                        g->b->gems_cible[i * g->b->width +(j+k)] = true;
                    }
                }
            }
        }
    }
    for (int j= 0;j < g->b->width;j++)
    {
        for (int i = 0; i< g->b->height;i++)
        {
            int index = i * g->b->width + j;
            element type = g->b->content_square[index].object;
            if (type != EMPTY)
            {
                count = 1;
                while (i + count < g->b->height && g->b->content_square[(i+count) *g->b->width + j].object == type)
                {
                    count ++;
                }
            
                if (count >=3)
                {
                    for (int k = 0;k <count ;k++)
                    {
                        g->b->gems_cible[(i+k)* g->b->width + j] = true;
                    }
                }
            }
        }
    }
    
    
}

void gems_gravity(board *b)
{
    int w = b->width;
    int h = b->height;
    int i, j, dest, index, dest_index;
    
    if (b->direction == DOWN) {
        for (j = 0; j < w; j++) {
            dest = h - 1;
            for (i = h - 1; i >= 0; i--) {
                index = i * w + j;
                if (b->content_square[index].object != EMPTY) {
                    if (dest != i) {
                        dest_index = dest * w + j;
                        b->content_square[dest_index] = b->content_square[index];
                        b->content_square[index].object = EMPTY;
                    }
                    dest--;
                }
            }
        }
    }
    else if (b->direction == UP) {
        for (j = 0; j < w; j++) {
            dest = 0;
            for (i = 0; i < h; i++) {
                index = i * w + j;
                if (b->content_square[index].object != EMPTY) {
                    if (dest != i) {
                        dest_index = dest * w + j;
                        b->content_square[dest_index] = b->content_square[index];
                        b->content_square[index].object = EMPTY;
                    }
                    dest++;
                }
            }
        }
    }
    else if (b->direction == LEFT) {
        for (i = 0; i < h; i++) {
            dest = 0;
            for (j = 0; j < w; j++) {
                index = i * w + j;
                if (b->content_square[index].object != EMPTY) {
                    if (dest != j) {
                        dest_index = i * w + dest;
                        b->content_square[dest_index] = b->content_square[index];
                        b->content_square[index].object = EMPTY;
                    }
                    dest++;
                }
            }
        }
    }
    else if (b->direction == RIGHT) {
        for (i = 0; i < h; i++) {
            dest = w - 1;
            for (j = w - 1; j >= 0; j--) {
                index = i * w + j;
                if (b->content_square[index].object != EMPTY) {
                    if (dest != j) {
                        dest_index = i * w + dest;
                        b->content_square[dest_index] = b->content_square[index];
                        b->content_square[index].object = EMPTY;
                    }
                    dest--;
                }
            }
        }
    }
}
void fill_empty_cells(game *g)
{
    if (!g->puzzle) {
        int size = g->b->width * g->b->height;
        
        for (int k = 0; k < size; k++) {
            if (g->b->content_square[k].object == EMPTY) {
                g->b->content_square[k].object = rand() % 7;
                g->b->content_square[k].power = GEM_BASIC;
                g->b->content_square[k].countdown = 0;
            }
        }
    }
}

/*gem_kill game_get_cell_kill(game *g, int i, int j)
{
    int index = i * g->b->width + j;
    if (index >= 0 && index < g->b->width * g->b->height) {
        if (g->b->gems_cible != NULL && g->b->gems_cible[index]) {
            return KILL_SHRINK;
        }
    }
    return KILL_NOKILL;
}
    */

void game_end_animation(game *g, anim_type type)
{
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
        game_init_animation(ANIM_KILL);
    }
    else if (type == ANIM_KILL)
    {
        int size = g->b->width * g->b->height;
        for(int k = 0; k <size;k++){
            if(g->b->gems_cible[k]){
                g->b->content_square[k].object = EMPTY;
            }
        }
        for(int k = 0; k <size;k++)
        {
            g->b->gems_cible[k] = false;
        }
        game_init_animation(ANIM_MOVE_FALL);       
        
    }
    else if (type == ANIM_MOVE_FALL)
    {
        gems_gravity(g->b);
        bool combo = false;
        if (g->lines) {
            combo = is_there_destructible_zone_row(g->b) || 
                       is_there_destructible_zone_col(g->b);
        }
        else 
        {
            combo = is_there_destrucible_zone_sccs(g->b);
        }
        if (combo) {
            marquer_gemmes(g);
            game_init_animation(ANIM_KILL);
            return;
        }

        if(!g->puzzle){
            fill_empty_cells(g);
            bool fill_combo = false;
            if(g->lines)
            {
                fill_combo = is_there_destructible_zone_row(g->b) || 
                        is_there_destructible_zone_col(g->b);
            }
            else 
            {
                fill_combo = is_there_destrucible_zone_sccs(g->b);
            }
            if(fill_combo)
            {
                marquer_gemmes(g);
                game_init_animation(ANIM_KILL);
                return;
            }
        }
    }
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
