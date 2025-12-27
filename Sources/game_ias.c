#include "game_ias.h"
#include "game_play.h"

bool (*ia_functions[IA_CLASSIC_SIZE])(game *) = {ia_random, ia_safe, ia_agressive};
char *ia_names[IA_CLASSIC_SIZE] = {"Random", "Safe", "Agressive"};

bool game_is_legal_move_lines(board *b, int i, int j, direction dir)
{
    int r = j + 1;
    int l = j - 1;
    int d = i + 1;
    int u = i - 1;

    // avoid out of board

    cell tmp_up;

    if (dir == UP)
    {
        // avoid out of board
        if (u < 0)
        {
            return false;
        }
        // up part
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, u, j)].object;
        b->content_square[index_board(b, u, j)].object = tmp_up.object;
        if (is_there_destructible_zone_col(b) || is_there_destructible_zone_row(b))
        {
            tmp_up.object = b->content_square[index_board(b, i, j)].object;
            b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, u, j)].object;
            b->content_square[index_board(b, u, j)].object = tmp_up.object;
            return true;
        }
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, u, j)].object;
        b->content_square[index_board(b, u, j)].object = tmp_up.object;
    }
    if (dir == DOWN)
    {
        // avoid out of board
        if (d >= b->height)
        {
            return false;
        }
        // down part
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, d, j)].object;
        b->content_square[index_board(b, d, j)].object = tmp_up.object;
        if (is_there_destructible_zone_col(b) || is_there_destructible_zone_row(b))
        {
            tmp_up.object = b->content_square[index_board(b, i, j)].object;
            b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, d, j)].object;
            b->content_square[index_board(b, d, j)].object = tmp_up.object;
            return true;
        }
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, d, j)].object;
        b->content_square[index_board(b, d, j)].object = tmp_up.object;
    }
    if (dir == RIGHT)
    {
        // avoid out of board
        if (r >= b->width)
        {
            return false;
        }
        // right part
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, r)].object;
        b->content_square[index_board(b, i, r)].object = tmp_up.object;
        if (is_there_destructible_zone_col(b) || is_there_destructible_zone_row(b))
        {
            tmp_up.object = b->content_square[index_board(b, i, j)].object;
            b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, r)].object;
            b->content_square[index_board(b, i, r)].object = tmp_up.object;
            return true;
        }
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, r)].object;
        b->content_square[index_board(b, i, r)].object = tmp_up.object;
    }
    if (dir == LEFT)
    {
        // avoid out of board
        if (l < 0)
        {
            return false;
        }
        // left part
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, l)].object;
        b->content_square[index_board(b, i, l)].object = tmp_up.object;
        if (is_there_destructible_zone_col(b) || is_there_destructible_zone_row(b))
        {
            tmp_up.object = b->content_square[index_board(b, i, j)].object;
            b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, l)].object;
            b->content_square[index_board(b, i, l)].object = tmp_up.object;
            return true;
        }
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, l)].object;
        b->content_square[index_board(b, i, l)].object = tmp_up.object;
    }

    return false;
}

bool game_is_legal_move_sccs(board *b, int i, int j, direction dir)
{
    int r = j + 1;
    int l = j - 1;
    int d = i + 1;
    int u = i - 1;

    cell tmp_up;

    if (dir == UP)
    {
        // avoid out of board
        if (u < 0)
        {
            return false;
        }
        // up part
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, u, j)].object;
        b->content_square[index_board(b, u, j)].object = tmp_up.object;
        if (is_there_destrucible_zone_sccs(b))
        {
            tmp_up.object = b->content_square[index_board(b, i, j)].object;
            b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, u, j)].object;
            b->content_square[index_board(b, u, j)].object = tmp_up.object;
            return true;
        }
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, u, j)].object;
        b->content_square[index_board(b, u, j)].object = tmp_up.object;
    }
    if (dir == DOWN)
    {
        // avoid out of board
        if (d >= b->height)
        {
            return false;
        }
        // down part
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, d, j)].object;
        b->content_square[index_board(b, d, j)].object = tmp_up.object;
        if (is_there_destrucible_zone_sccs(b))
        {
            tmp_up.object = b->content_square[index_board(b, i, j)].object;
            b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, d, j)].object;
            b->content_square[index_board(b, d, j)].object = tmp_up.object;
            return true;
        }
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, d, j)].object;
        b->content_square[index_board(b, d, j)].object = tmp_up.object;
    }
    if (dir == RIGHT)
    {
        // avoid out of board
        if (r >= b->width)
        {
            return false;
        }
        // right part
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, r)].object;
        b->content_square[index_board(b, i, r)].object = tmp_up.object;
        if (is_there_destrucible_zone_sccs(b))
        {
            tmp_up.object = b->content_square[index_board(b, i, j)].object;
            b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, r)].object;
            b->content_square[index_board(b, i, r)].object = tmp_up.object;
            return true;
        }
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, r)].object;
        b->content_square[index_board(b, i, r)].object = tmp_up.object;
    }
    if (dir == LEFT)
    {
        // avoid out of board
        if (l < 0)
        {
            return false;
        }
        // left part
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, l)].object;
        b->content_square[index_board(b, i, l)].object = tmp_up.object;
        if (is_there_destrucible_zone_sccs(b))
        {
            tmp_up.object = b->content_square[index_board(b, i, j)].object;
            b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, l)].object;
            b->content_square[index_board(b, i, l)].object = tmp_up.object;
            return true;
        }
        tmp_up.object = b->content_square[index_board(b, i, j)].object;
        b->content_square[index_board(b, i, j)].object = b->content_square[index_board(b, i, l)].object;
        b->content_square[index_board(b, i, l)].object = tmp_up.object;
    }

    return false;
}

bool ia_random(game *)
{
    return false;
}

bool ia_safe(game *g)
{
    // lines mode
    if (g->lines == true)
    {
        if (g->b->direction == DOWN)
        {
            for (int i = 0; i < g->b->height; i++)
            {
                for (int j = 0; j < g->b->width; j++)
                {
                    if (game_is_legal_move_lines(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == RIGHT)
        {
            for (int j = 0; j < g->b->width; j++)
            {
                for (int i = 0; i < g->b->height; i++)
                {
                    if (game_is_legal_move_lines(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == UP)
        {
            for (int i = g->b->height - 1; i >= 0; i--)
            {
                for (int j = 0; j < g->b->width; j++)
                {
                    if (game_is_legal_move_lines(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == LEFT)
        {
            for (int j = g->b->width - 1; j >= 0; j--)
            {
                for (int i = 0; i < g->b->height; i++)
                {
                    if (game_is_legal_move_lines(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
    }

    // scc mode
    if (g->lines == false)
    {
        if (g->b->direction == DOWN)
        {
            for (int i = 0; i < g->b->height; i++)
            {
                for (int j = 0; j < g->b->width; j++)
                {
                    if (game_is_legal_move_sccs(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == RIGHT)
        {
            for (int j = 0; j < g->b->width; j++)
            {
                for (int i = 0; i < g->b->height; i++)
                {
                    if (game_is_legal_move_sccs(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == UP)
        {
            for (int i = g->b->height - 1; i >= 0; i--)
            {
                for (int j = 0; j < g->b->width; j++)
                {
                    if (game_is_legal_move_sccs(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == LEFT)
        {
            for (int j = g->b->width - 1; j >= 0; j--)
            {
                for (int i = 0; i < g->b->height; i++)
                {
                    if (game_is_legal_move_sccs(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool ia_agressive(game *g)
{

    // lines mode
    if (g->lines == true)
    {
        if (g->b->direction == UP)
        {
            for (int i = 0; i < g->b->height; i++)
            {
                for (int j = 0; j < g->b->width; j++)
                {
                    if (game_is_legal_move_lines(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == LEFT)
        {
            for (int j = 0; j < g->b->width; j++)
            {
                for (int i = 0; i < g->b->height; i++)
                {
                    if (game_is_legal_move_lines(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == DOWN)
        {
            for (int i = g->b->height - 1; i >= 0; i--)
            {
                for (int j = 0; j < g->b->width; j++)
                {
                    if (game_is_legal_move_lines(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == RIGHT)
        {
            for (int j = g->b->width - 1; j >= 0; j--)
            {
                for (int i = 0; i < g->b->height; i++)
                {
                    if (game_is_legal_move_lines(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_lines(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
    }

    // scc mode
    if (g->lines == false)
    {
        if (g->b->direction == DOWN)
        {
            for (int i = g->b->height - 1; i >= 0; i--)
            {
                for (int j = 0; j < g->b->width; j++)
                {
                    if (game_is_legal_move_sccs(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == RIGHT)
        {
            for (int j = g->b->width - 1; j >= 0; j--)
            {
                for (int i = 0; i < g->b->height; i++)
                {
                    if (game_is_legal_move_sccs(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == UP)
        {
            for (int i = 0; i < g->b->height; i++)
            {
                for (int j = 0; j < g->b->width; j++)
                {
                    if (game_is_legal_move_sccs(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
        if (g->b->direction == LEFT)
        {
            for (int j = 0; j < g->b->width; j++)
            {
                for (int i = 0; i < g->b->height; i++)
                {
                    if (game_is_legal_move_sccs(g->b, i, j, UP))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i - 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, DOWN))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i + 1, j);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, RIGHT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j + 1);
                        return true;
                    }
                    if (game_is_legal_move_sccs(g->b, i, j, LEFT))
                    {
                        game_handle_mouse_click(g, i, j);
                        game_handle_mouse_click(g, i, j - 1);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
