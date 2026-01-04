#include "game_scores.h"
#include "game_save.h"

score_table *load_score_table(const char *filepath, int *player_score)
{
    score_table *table = calloc(1, sizeof(*table));

    FILE *f = fopen(filepath, "r");

    if (f != NULL)
    {
        // find size of our tabs -> lines
        fscanf(f, "%d", &(table->size));

        table->names = malloc(table->size * sizeof(*(table->names)));
        for (int i = 0; i < table->size; i++)
        {
            fscanf(f, "%s", table->names[i]);
        }

        table->scores = malloc(table->size * sizeof(*(table->scores)));
        for (int i = 0; i < table->size; i++)
        {
            table->scores[i] = malloc(SC_SIZE * sizeof(int));
        }
        for (int i = 0; i < table->size; i++)
        {
            for (int j = 0; j < SC_SIZE; j++)
            {
                fscanf(f, "%d", &(table->scores[i][j]));
            }
        }
        fclose(f);
    }

    table->permutation = malloc(table->size * sizeof(*table->permutation));
    for (int i = 0; i < table->size; i++)
    {
        table->permutation[i] = i;
    }

    if (player_score)
    {
        table->size++;
        table->names = realloc(table->names, table->size * sizeof(*(table->names)));
        sprintf(table->names[table->size - 1], "Player");

        table->permutation = realloc(table->permutation, table->size * sizeof(*(table->permutation)));

        table->scores = realloc(table->scores, table->size * sizeof(*(table->scores)));
        table->scores[table->size - 1] = malloc(SC_SIZE * sizeof(int));
        for (int i = 0; i < SC_SIZE; i++)
        {
            table->scores[table->size - 1][i] = player_score[i];
        }

        table->player_index = table->size - 1;

        // permutation sort
        for (int i = 0; i < table->size - 1; i++)
        {
            for (int k = 0; k < table->size - i - 1; k++)
            {
                if (table->scores[table->permutation[k]][0] < table->scores[table->permutation[k + 1]][0])
                {
                    int tmp = table->permutation[k];
                    table->permutation[k] = table->permutation[k + 1];
                    table->permutation[k + 1] = tmp;
                }
            }
        }
    }
    else
    {
        table->player_index = -1;
    }

    return table;
}

void save_score_table(score_table *table, const char *filepath, const char *playername)
{
    // Put the real player name insted of player
    if (playername != NULL && table->player_index != -1)
    {
        sprintf(table->names[table->player_index], "%s", playername);
    }

    FILE *f = fopen(filepath, "w");

    fprintf(f, "%d", table->size);
    fprintf(f, "\n");

    // name part
    for (int i = 0; i < table->size; i++)
    {
        fprintf(f, "%s\n", table->names[i]);
    }

    // score part
    for (int i = 0; i < table->size; i++)
    {
        for (int j = 0; j < SC_SIZE; j++)
        {
            fprintf(f, "%d ", table->scores[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);

    return;
}

void delete_score_table(score_table *table)
{
    free(table->names);
    free(table->permutation);
    for(int i = 0; i < table->size; i++)
    {
        free(table->scores[i]);
    }
    free(table->scores);
    free(table);
    return;
}

void sort_score_table(score_table * table, score_type type)
{
    for (int i = 1; i< table->size; i++)
    {
        int pos = table->permutation[i];
        int j = i-1;
        while (j >=0 && table->scores[table->permutation[j]][type] < table->scores[pos][type])
        {
            table->permutation[j+1] = table->permutation[j];
            j--;
        }
        table->permutation[j + 1] = pos;
    }
    return;
}
