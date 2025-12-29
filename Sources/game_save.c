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

bool save_game(game *g, const char *filepath)
{
    FILE *f = fopen(filepath, "w");

    if (f != NULL)
    {
        fprintf(f, "%s %d\n", save_keyword_strings[SK_WIDTH], g->b->width);
        fprintf(f, "%s %d\n", save_keyword_strings[SK_HEIGHT], g->b->height);

        fprintf(f, "%s ", save_keyword_strings[SK_BOARD]);
        for (int i = 0; i < g->b->width * g->b->height; i++)
        {
            fprintf(f, "%d ", g->b->content_square[i].object);
            fprintf(f, "%d ", g->b->content_square[i].power);
            fprintf(f, "%d ", g->b->content_square[i].countdown);
        }
        fprintf(f, "\n");

        fprintf(f, "%s %d\n", save_keyword_strings[SK_GRAVITY], g->b->direction);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_REFILL_COUNT], g->b->q->size);

        fprintf(f, "%s ", save_keyword_strings[SK_REFILL_QUEUE]);
        for (int i = 0; i < g->b->q->size; i++)
        {
            cell tmp;
            tmp = dequeue(g->b->q);
            fprintf(f, "%d ", tmp.object);
            fprintf(f, "%d ", tmp.power);
            fprintf(f, "%d ", tmp.countdown);
            enqueue(tmp, g->b->q);
        }
        fprintf(f, "\n");

        fprintf(f, "%s %d\n", save_keyword_strings[SK_PUZZLE], g->puzzle);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_LINES], g->lines);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_UPGRADES], g->upgrades);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_NODELAY], g->nodelay);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_EASY], g->easy);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_WRAP], 0);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_BOMBS], g->bombs);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_REFILL], g->refill_mode);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_GRAVITY_CONTROL], g->gravity_control);

        fprintf(f, "%s %d\n", save_keyword_strings[SK_LEVEL], g->score->level);
        fprintf(f, "%s %d\n", save_keyword_strings[SK_LEVEL_SCORE], g->score->level_score);
        fprintf(f, "%s %d\n", save_keyword_strings[SK_TARGET_SCORE], g->score->target_score);
        fprintf(f, "%s %d\n", save_keyword_strings[SK_SCALE_SCORE], g->score->scale_score);
        fprintf(f, "%s ", save_keyword_strings[SK_SCORE_ARRAY]);
        for (int i = 0; i < SC_SIZE; i++)
        {
            fprintf(f, "%d ", g->score->scores[i]);
        }
        fprintf(f, "\n");

        fclose(f);
        return true;
    }
    return false;
}

bool are_string_equal(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        if (s1[i] != s2[i])
        {
            return false;
        }
        i++;
    }
    return (s1[i] == '\0' && s2[i] == '\0');
}

game *load_game(const char *filepath)
{

    FILE *f = fopen(filepath, "r");

    if (f != NULL)
    {

        // game part
        game *g = malloc(sizeof(*g));

        // board part
        g->b = calloc(1, sizeof(*(g->b)));
        g->b->q = calloc(1, sizeof(*(g->b->q)));
        g->score = calloc(1, sizeof(*(g->score)));

        char comparing_tool[20];

        int tmp;

        while (fscanf(f, "%s", comparing_tool) != EOF)
        {
            int ind = -1;
            for (int i = 0; i < SK_SIZE; i++)
            {
                if (are_string_equal(comparing_tool, save_keyword_strings[i]))
                {
                    ind = i;
                    break;
                }
            }

            switch (ind)
            {
            case (SK_WIDTH):
                fscanf(f, "%d", &g->b->width);
                break;
            case (SK_HEIGHT):
                fscanf(f, "%d", &g->b->height);
                break;
            case (SK_BOARD):
                g->b->content_square = malloc((g->b->height * g->b->width) * sizeof(*(g->b->content_square)));
                for (int i = 0; i < g->b->height * g->b->width; i++)
                {
                    fscanf(f, "%d", &g->b->content_square[i].object);
                    fscanf(f, "%d", &g->b->content_square[i].power);
                    fscanf(f, "%hd", &g->b->content_square[i].countdown);
                }
                break;
            case (SK_GRAVITY):
                fscanf(f, "%d", &g->b->direction);
                break;
            case (SK_REFILL_COUNT):
                fscanf(f, "%d", &tmp);
                break;
            case (SK_REFILL_QUEUE):
                cell que;
                for (int i = 0; i < tmp; i++)
                {
                    fscanf(f, "%d", &que.object);
                    fscanf(f, "%d", &que.power);
                    fscanf(f, "%hd", &que.countdown);
                    enqueue(que, g->b->q);
                }
                break;
            case (SK_PUZZLE):
                fscanf(f, "%d", &tmp);
                g->puzzle = tmp;
                break;
            case (SK_LINES):
                fscanf(f, "%d", &tmp);
                g->lines = tmp;
                break;
            case (SK_UPGRADES):
                fscanf(f, "%d", &tmp);
                g->upgrades = tmp;
                break;
            case (SK_NODELAY):
                fscanf(f, "%d", &tmp);
                g->nodelay = tmp;
                break;
            case (SK_EASY):
                fscanf(f, "%d", &tmp);
                g->easy = tmp;
                break;
            case (SK_WRAP):
                fscanf(f, "%d", &tmp);
                break;
            case (SK_BOMBS):
                fscanf(f, "%d", &tmp);
                g->bombs = tmp;
                break;
            case (SK_REFILL):
                fscanf(f, "%hd", &g->refill_mode);
                break;
            case (SK_GRAVITY_CONTROL):
                fscanf(f, "%hd", &g->gravity_control);
                break;
            case (SK_LEVEL):
                fscanf(f, "%d", &g->score->level);
                break;
            case (SK_LEVEL_SCORE):
                fscanf(f, "%d", &g->score->level_score);
                break;
            case (SK_TARGET_SCORE):
                fscanf(f, "%d", &g->score->target_score);
                break;
            case (SK_SCALE_SCORE):
                fscanf(f, "%d", &g->score->scale_score);
                break;
            case (SK_SCORE_ARRAY):
                for (int i = 0; i < SC_SIZE; i++)
                {
                    fscanf(f, "%d", &g->score->scores[i]);
                }
                break;
            }
        }
        fclose(f);
        return g;
    }
    return NULL;
}
