#include "game_solver.h"
#include "game_play.h"
#include "game_save.h"
#include "game_ias.h"

/*************************************/
/*+ Hash table (already programmed) +*/
/*************************************/

#define MAX_STATES 4000000

hash_table *create_hash_table(int element_size)
{
    // Allocating memory for the hash table structure.
    hash_table *ht;
    CREATE_MEMORY(ht, 1);

    // Initializing the hash table.
    ht->hash_capacity = 128; // Initial capacity is set to 128.
    ht->hash_size = 0;       // Initially, the hash table is empty.
    CREATE_MEMORY(ht->hash_table, ht->hash_capacity);

    // Setting all slots to ULONG_MAX (indicating empty slots).
    for (ulong i = 0; i < ht->hash_capacity; i++)
    {
        ht->hash_table[i] = ULONG_MAX;
    }

    // Setting the element size.
    ht->element_size = element_size;
    return ht;
}

void delete_hash_table(hash_table *ht)
{
    if (ht == NULL)
    {
        return;
    }
    free(ht->hash_table);
    free(ht);
}

// Hash function using a simple polynomial rolling hash. Hashes an array of
// integers.
static ulong
hash_function(int *elt,           //!< The element to hash.
              int size,           //!< Size of the element (number of integers).
              ulong hash_capacity //!< Capacity of the hash table.
)
{
    ulong hash = 0;
    ulong a = 0x9e3779b9; // fractional bits of the golden ratio
    for (int j = 0; j < size; j++)
    {
        hash = (hash * ((ulong)size + 1) + (ulong)elt[j]) * a %
               hash_capacity;
    }
    return hash;
}

// Rehashing function to resize the hash table when load factor exceeds 0.5
static void hash_table_rehash(hash_table *ht, int *elements)
{
    // Saving the pointer to the old table and its size.
    ulong *old_table = ht->hash_table;
    ulong old_size = ht->hash_capacity;

    // Doubling the size of the hash table.
    ht->hash_capacity *= 2;
    CREATE_MEMORY(ht->hash_table, ht->hash_capacity);
    for (ulong i = 0; i < ht->hash_capacity; i++)
    {
        ht->hash_table[i] = ULONG_MAX;
    }

    // Rehashing the elements from the old table to the new one.
    for (ulong i = 0; i < old_size; i++)
    {
        if (old_table[i] == ULONG_MAX)
        {
            continue; // Skip empty slots
        }
        // Computing the index correspinding to the element with the hash
        // function.
        ulong index =
            hash_function(elements + old_table[i] * ((ulong)ht->element_size),
                          ht->element_size, ht->hash_capacity);

        // linear probing to find the next available slot
        ulong j = 0;
        while (true)
        {
            ulong new_index = (index + j) % ht->hash_capacity; // Linear probing
            if (ht->hash_table[new_index] == ULONG_MAX)
            {
                ht->hash_table[new_index] = old_table[i];
                break; // Successfully inserted
            }
            else
            {
                j++;
            }
        }
    }
    free(old_table);
}

// Compares two array of integers to check if they are equal
static bool hash_array_compare(int *a, int *b, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (a[i] != b[i])
        {
            return false;
        }
    }
    return true;
}

bool hash_table_insert(hash_table *ht, int *elements, ulong i)
{
    // Rehashing if the load factor exceeds 0.5
    if (ht->hash_size >= (ht->hash_capacity >> 1))
    {
        hash_table_rehash(ht, elements);
    }

    // Computing the index where the element should be.
    ulong index = hash_function(elements + i * ((ulong)ht->element_size),
                                ht->element_size, ht->hash_capacity);

    // Linear probing to find either the element or a free spot
    ulong s = 0;
    while (true)
    {
        ulong s_index = (index + s) % ht->hash_capacity; // Linear probing
        if (ht->hash_table[s_index] == ULONG_MAX)
        {
            // If we find an empty spot, we insert the value and return true to
            // indicate that the insertion was performed.
            ht->hash_table[s_index] = i;
            ht->hash_size++;
            return true;
        }
        else if (hash_array_compare(elements + i * ((ulong)ht->element_size),
                                    elements + ht->hash_table[s_index] *
                                                   ((ulong)ht->element_size),
                                    ht->element_size))
        {
            // If the value is already present, we do not insert it and return
            // false to indicate that the insertion failed.
            return false;
        }
        else
        {
            // If the slot is occupied by a different value, we try the next
            // slot.
            s++;
        }
    }
    return false;
}

/**********************/
/*+ Solver functions +*/
/**********************/

int game_to_int_array(game *g, int *int_array)
{
    int element_size = 0;

    // board part
    int k = 0;
    for (int i = 0; i < g->b->height * g->b->width; i++)
    {
        int_array[k] = g->b->content_square[i].object;
        int_array[k + 1] = g->b->content_square[i].power;
        int_array[k + 2] = g->b->content_square[i].countdown;
        k = k + 3;

        element_size = element_size + 3;
    }

    // gravity part
    int_array[element_size] = g->b->direction;
    element_size++;

    // queue part
    int j = 0;
    for (int i = 0; i < g->b->height * g->b->width; i++)
    {
        if (i < get_queuesize(g->b->q))
        {

            cell tmp;
            tmp = dequeue(g->b->q);
            int_array[element_size + j] = tmp.object;
            int_array[element_size + j + 1] = tmp.power;
            int_array[element_size + j + 2] = tmp.countdown;

            j = j + 3;

            enqueue(tmp, g->b->q);
        }
        else
        {
            int_array[element_size + j] = -1;
            int_array[element_size + j + 1] = -1;
            int_array[element_size + j + 2] = -1;

            j = j + 3;
        }
    }
    element_size = element_size + 3 * (g->b->width * g->b->height);

    return element_size;
}

void int_array_to_game(int *int_array, game *g)
{
    int element_size = 0;

    // board part
    int k = 0;
    for (int i = 0; i < g->b->height * g->b->width; i++)
    {
        g->b->content_square[i].object = int_array[k];
        g->b->content_square[i].power = int_array[k + 1];
        g->b->content_square[i].countdown = int_array[k + 2];
        k = k + 3;

        element_size = element_size + 3;
    }

    g->b->direction = int_array[element_size];
    element_size++;

    // queue part
    while (!isempty_queue(g->b->q))
    {
        dequeue(g->b->q);
    }
    int j = 0;
    for (int i = 0; i < g->b->height * g->b->width; i++)
    {
        if (int_array[element_size + j] != -1)
        {
            cell tmp;
            tmp.object = int_array[element_size + j];
            tmp.power = int_array[element_size + j + 1];
            tmp.countdown = int_array[element_size + j + 2];

            j = j + 3;

            enqueue(tmp, g->b->q);
        }
        else
        {
            j = j + 3;
        }
    }
}

bool is_game_win(game *g)
{
    for (int i = 0; i < g->b->height; i++)
    {
        for (int j = 0; j < g->b->width; j++)
        {
            if (g->b->content_square[index_board(g->b, i, j)].object != EMPTY)
            {
                return false;
            }
        }
    }
    return true;
}

solver_move *solve_puzzle(game *g, solver method, int bound, solver_result_type *result, int *nb_moves,
                          int *nb_configs)
{
    // same as game_to_int_array
    int element_size = (6 * (g->b->height * g->b->width)) + 1;

    // hash_table part
    hash_table *hash = create_hash_table(element_size);

    // the extern storage
    int *ext_storage = malloc((MAX_STATES * element_size) * sizeof(*ext_storage));

    // tab to know parents of each state
    ulong *parents = malloc(MAX_STATES * sizeof(*parents));

    // tab to know the game play
    solver_move *moves = malloc(MAX_STATES * sizeof(*moves));

    if (g->lines == false)
    {

        if (method == SOLVER_BFS)
        {
            queue_int *queue_BFS = create_queue_int();

            game_to_int_array(g, ext_storage);

            hash_table_insert(hash, ext_storage, 0);

            enqueue_int(0, queue_BFS);

            parents[0] = -1;

            int next_idx = 1;

            while (!isempty_queue_int(queue_BFS))
            {
                game *tmp = game_clone(g);

                int current_idx = dequeue_int(queue_BFS);

                (*nb_configs)++;

                int_array_to_game(ext_storage + (current_idx * element_size), tmp);

                if (is_game_win(tmp))
                {
                    int final_idx = current_idx;
                    int length_solution = 0;
                    while (current_idx != 0)
                    {
                        current_idx = parents[current_idx];
                        next_idx = current_idx;
                        length_solution++;
                    }

                    // chain list to reverse the tab
                    solver_move *head = NULL;
                    int vic_idx = final_idx;

                    while (vic_idx != 0)
                    {
                        solver_move *new_chain = malloc(sizeof(*new_chain));

                        new_chain->dir = moves[vic_idx].dir;
                        new_chain->index = moves[vic_idx].index;
                        new_chain->type = moves[vic_idx].type;

                        new_chain->next = head;
                        head = new_chain;
                        vic_idx = parents[vic_idx];
                    }

                    *nb_moves = length_solution;
                    *result = SOLVER_SOLVED;
                    return head;
                }

                // test new config
                for (int i = 0; i < tmp->b->height; i++)
                {
                    for (int j = 0; j < tmp->b->width; j++)
                    {
                        if (game_is_legal_move_sccs(tmp->b, i, j, UP))
                        {
                            game *copie1 = game_clone(tmp);
                            game_handle_mouse_click(copie1, i, j);
                            game_handle_mouse_click(copie1, i - 1, j);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie1, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                enqueue_int(next_idx, queue_BFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = UP;
                                moves[next_idx].index = index_board(copie1->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie1);
                        }
                        if (game_is_legal_move_sccs(tmp->b, i, j, DOWN))
                        {
                            game *copie2 = game_clone(tmp);
                            game_handle_mouse_click(copie2, i, j);
                            game_handle_mouse_click(copie2, i + 1, j);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie2, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                enqueue_int(next_idx, queue_BFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = DOWN;
                                moves[next_idx].index = index_board(copie2->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie2);
                        }
                        if (game_is_legal_move_sccs(tmp->b, i, j, RIGHT))
                        {
                            game *copie3 = game_clone(tmp);
                            game_handle_mouse_click(copie3, i, j);
                            game_handle_mouse_click(copie3, i, j + 1);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie3, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                enqueue_int(next_idx, queue_BFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = RIGHT;
                                moves[next_idx].index = index_board(copie3->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie3);
                        }
                        if (game_is_legal_move_sccs(tmp->b, i, j, LEFT))
                        {
                            game *copie4 = game_clone(tmp);
                            game_handle_mouse_click(copie4, i, j);
                            game_handle_mouse_click(copie4, i, j - 1);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie4, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                enqueue_int(next_idx, queue_BFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = LEFT;
                                moves[next_idx].index = index_board(copie4->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie4);
                        }
                    }
                }
                game_release(tmp);
            }
        }
        else if (method == SOLVER_DFS)
        {
            stack_int *stack_DFS = create_stack_int();

            game_to_int_array(g, ext_storage);

            hash_table_insert(hash, ext_storage, 0);

            push_int(0, stack_DFS);

            parents[0] = -1;

            int next_idx = 1;

            while (!isempty_stack_int(stack_DFS))
            {
                game *tmp = game_clone(g);

                int current_idx = pop_int(stack_DFS);

                (*nb_configs)++;

                int_array_to_game(ext_storage + (current_idx * element_size), tmp);

                if (is_game_win(tmp))
                {
                    int final_idx = current_idx;
                    int length_solution = 0;
                    while (current_idx != 0)
                    {
                        current_idx = parents[current_idx];
                        next_idx = current_idx;
                        length_solution++;
                    }

                    // chain list to reverse the tab
                    solver_move *head = NULL;
                    int vic_idx = final_idx;

                    while (vic_idx != 0)
                    {
                        solver_move *new_chain = malloc(sizeof(*new_chain));

                        new_chain->dir = moves[vic_idx].dir;
                        new_chain->index = moves[vic_idx].index;
                        new_chain->type = moves[vic_idx].type;

                        new_chain->next = head;
                        head = new_chain;
                        vic_idx = parents[vic_idx];
                    }

                    *nb_moves = length_solution;
                    *result = SOLVER_SOLVED;
                    return head;
                }

                // test new config
                for (int i = 0; i < tmp->b->height; i++)
                {
                    for (int j = 0; j < tmp->b->width; j++)
                    {
                        if (game_is_legal_move_sccs(tmp->b, i, j, UP))
                        {
                            game *copie1 = game_clone(tmp);
                            game_handle_mouse_click(copie1, i, j);
                            game_handle_mouse_click(copie1, i - 1, j);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie1, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                push_int(next_idx, stack_DFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = UP;
                                moves[next_idx].index = index_board(copie1->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie1);
                        }
                        if (game_is_legal_move_sccs(tmp->b, i, j, DOWN))
                        {
                            game *copie2 = game_clone(tmp);
                            game_handle_mouse_click(copie2, i, j);
                            game_handle_mouse_click(copie2, i + 1, j);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie2, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                push_int(next_idx, stack_DFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = DOWN;
                                moves[next_idx].index = index_board(copie2->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie2);
                        }
                        if (game_is_legal_move_sccs(tmp->b, i, j, RIGHT))
                        {
                            game *copie3 = game_clone(tmp);
                            game_handle_mouse_click(copie3, i, j);
                            game_handle_mouse_click(copie3, i, j + 1);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie3, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                push_int(next_idx, stack_DFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = RIGHT;
                                moves[next_idx].index = index_board(copie3->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie3);
                        }
                        if (game_is_legal_move_sccs(tmp->b, i, j, LEFT))
                        {
                            game *copie4 = game_clone(tmp);
                            game_handle_mouse_click(copie4, i, j);
                            game_handle_mouse_click(copie4, i, j - 1);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie4, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                push_int(next_idx, stack_DFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = LEFT;
                                moves[next_idx].index = index_board(copie4->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie4);
                        }
                    }
                }
                game_release(tmp);
            }
        }

        // A method left and no free memory
        return NULL;
    }

    else if (g->lines == true)
    {

        if (method == SOLVER_BFS)
        {
            queue_int *queue_BFS = create_queue_int();

            game_to_int_array(g, ext_storage);

            hash_table_insert(hash, ext_storage, 0);

            enqueue_int(0, queue_BFS);

            parents[0] = -1;

            int next_idx = 1;

            while (!isempty_queue_int(queue_BFS))
            {
                game *tmp = game_clone(g);

                int current_idx = dequeue_int(queue_BFS);

                (*nb_configs)++;

                int_array_to_game(ext_storage + (current_idx * element_size), tmp);

                if (is_game_win(tmp))
                {
                    int final_idx = current_idx;
                    int length_solution = 0;
                    while (current_idx != 0)
                    {
                        current_idx = parents[current_idx];
                        next_idx = current_idx;
                        length_solution++;
                    }

                    // chain list to reverse the tab
                    solver_move *head = NULL;
                    int vic_idx = final_idx;

                    while (vic_idx != 0)
                    {
                        solver_move *new_chain = malloc(sizeof(*new_chain));

                        new_chain->dir = moves[vic_idx].dir;
                        new_chain->index = moves[vic_idx].index;
                        new_chain->type = moves[vic_idx].type;

                        new_chain->next = head;
                        head = new_chain;
                        vic_idx = parents[vic_idx];
                    }

                    *nb_moves = length_solution;
                    *result = SOLVER_SOLVED;
                    return head;
                }

                // test new config
                for (int i = 0; i < tmp->b->height; i++)
                {
                    for (int j = 0; j < tmp->b->width; j++)
                    {
                        if (game_is_legal_move_lines(tmp->b, i, j, UP))
                        {
                            game *copie1 = game_clone(tmp);
                            game_handle_mouse_click(copie1, i, j);
                            game_handle_mouse_click(copie1, i - 1, j);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie1, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                enqueue_int(next_idx, queue_BFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = UP;
                                moves[next_idx].index = index_board(copie1->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie1);
                        }
                        if (game_is_legal_move_lines(tmp->b, i, j, DOWN))
                        {
                            game *copie2 = game_clone(tmp);
                            game_handle_mouse_click(copie2, i, j);
                            game_handle_mouse_click(copie2, i + 1, j);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie2, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                enqueue_int(next_idx, queue_BFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = DOWN;
                                moves[next_idx].index = index_board(copie2->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie2);
                        }
                        if (game_is_legal_move_lines(tmp->b, i, j, RIGHT))
                        {
                            game *copie3 = game_clone(tmp);
                            game_handle_mouse_click(copie3, i, j);
                            game_handle_mouse_click(copie3, i, j + 1);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie3, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                enqueue_int(next_idx, queue_BFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = RIGHT;
                                moves[next_idx].index = index_board(copie3->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie3);
                        }
                        if (game_is_legal_move_lines(tmp->b, i, j, LEFT))
                        {
                            game *copie4 = game_clone(tmp);
                            game_handle_mouse_click(copie4, i, j);
                            game_handle_mouse_click(copie4, i, j - 1);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie4, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                enqueue_int(next_idx, queue_BFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = LEFT;
                                moves[next_idx].index = index_board(copie4->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie4);
                        }
                    }
                }
                game_release(tmp);
            }
        }
        else if (method == SOLVER_DFS)
        {
            stack_int *stack_DFS = create_stack_int();

            game_to_int_array(g, ext_storage);

            hash_table_insert(hash, ext_storage, 0);

            push_int(0, stack_DFS);

            parents[0] = -1;

            int next_idx = 1;

            while (!isempty_stack_int(stack_DFS))
            {
                game *tmp = game_clone(g);

                int current_idx = pop_int(stack_DFS);

                (*nb_configs)++;

                int_array_to_game(ext_storage + (current_idx * element_size), tmp);

                if (is_game_win(tmp))
                {
                    int final_idx = current_idx;
                    int length_solution = 0;
                    while (current_idx != 0)
                    {
                        current_idx = parents[current_idx];
                        next_idx = current_idx;
                        length_solution++;
                    }

                    // chain list to reverse the tab
                    solver_move *head = NULL;
                    int vic_idx = final_idx;

                    while (vic_idx != 0)
                    {
                        solver_move *new_chain = malloc(sizeof(*new_chain));

                        new_chain->dir = moves[vic_idx].dir;
                        new_chain->index = moves[vic_idx].index;
                        new_chain->type = moves[vic_idx].type;

                        new_chain->next = head;
                        head = new_chain;
                        vic_idx = parents[vic_idx];
                    }

                    *nb_moves = length_solution;
                    *result = SOLVER_SOLVED;
                    return head;
                }

                // test new config
                for (int i = 0; i < tmp->b->height; i++)
                {
                    for (int j = 0; j < tmp->b->width; j++)
                    {
                        if (game_is_legal_move_lines(tmp->b, i, j, UP))
                        {
                            game *copie1 = game_clone(tmp);
                            game_handle_mouse_click(copie1, i, j);
                            game_handle_mouse_click(copie1, i - 1, j);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie1, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                push_int(next_idx, stack_DFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = UP;
                                moves[next_idx].index = index_board(copie1->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie1);
                        }
                        if (game_is_legal_move_lines(tmp->b, i, j, DOWN))
                        {
                            game *copie2 = game_clone(tmp);
                            game_handle_mouse_click(copie2, i, j);
                            game_handle_mouse_click(copie2, i + 1, j);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie2, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                push_int(next_idx, stack_DFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = DOWN;
                                moves[next_idx].index = index_board(copie2->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie2);
                        }
                        if (game_is_legal_move_lines(tmp->b, i, j, RIGHT))
                        {
                            game *copie3 = game_clone(tmp);
                            game_handle_mouse_click(copie3, i, j);
                            game_handle_mouse_click(copie3, i, j + 1);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie3, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                push_int(next_idx, stack_DFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = RIGHT;
                                moves[next_idx].index = index_board(copie3->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie3);
                        }
                        if (game_is_legal_move_lines(tmp->b, i, j, LEFT))
                        {
                            game *copie4 = game_clone(tmp);
                            game_handle_mouse_click(copie4, i, j);
                            game_handle_mouse_click(copie4, i, j - 1);
                            if ((next_idx >= MAX_STATES) || (bound > 0 && next_idx >= bound))
                            {
                                printf("puzzle too hard");
                                return NULL;
                            }
                            game_to_int_array(copie4, ext_storage + (next_idx * element_size));

                            if (hash_table_insert(hash, ext_storage + (element_size * next_idx), next_idx))
                            {
                                push_int(next_idx, stack_DFS);
                                parents[next_idx] = current_idx;
                                moves[next_idx].dir = LEFT;
                                moves[next_idx].index = index_board(copie4->b, i, j);
                                moves[next_idx].type = SOLVER_SWAP;
                                next_idx++;
                            }
                            game_release(copie4);
                        }
                    }
                }
                game_release(tmp);
            }
        }
        // A method left and no free memory
        return NULL;
    }
    return NULL;
}

bool game_implement_solver_move(game *, solver_move *)
{
    return false;
}
