
#include "queue.h"

typedef struct cell cell;

typedef struct stack
{
    int capacity;   // Taille totale du tableau alloué en mémoire.
    int size_stack; // Nombre d'éléments dans la pile représentée.
    cell *array;    // Tableau contenant les valeurs de la pile représentée.
} stack;

// Double la taille du tableau utilisé dans la représentation.
// Cette fonction sera utilisée lorsque le tableau est plein et qu'on veut y
// ajouter une valeur.
static void grow_stack(stack *p)
{
    p->array = realloc(p->array, 2 * p->capacity * (sizeof(cell)));
    p->capacity *= 2;
    return;
}

// Divise par deux la taille du tableau utilisé dans la représentation.
// Cette fonction sera utilisée lorsque le tableau est rempli à moins de 25%
// de sa capacité.
static void shrink_stack(stack *p)
{
    if (p->capacity == 1)
    {
        printf("the table is too short !");
        return;
    }
    p->array = realloc(p->array, p->capacity / 2 * sizeof(cell));
    p->capacity /= 2;
    return;
}

stack *create_stack(void)
{
    stack *p = malloc(sizeof(stack));
    p->capacity = 1;
    p->size_stack = 0;
    p->array = malloc((p->capacity) * sizeof(cell));
    return p;
}

void delete_stack(stack *p)
{
    free(p->array);
    free(p);
    return;
}

bool isempty_stack(stack *p)
{
    return p->size_stack == 0;
}

cell pop(stack *p)
{

    if (isempty_stack(p))
    {
        cell c;
        c.object = EMPTY;
        c.countdown = 0;
        c.power = GEM_BASIC;
        fprintf(stderr, "the table is too short ! \n");
        return c;
    }
    p->size_stack--;
    cell result = p->array[p->size_stack];
    if ((p->size_stack * 100 / p->capacity) < 25)
    {
        shrink_stack(p);
    }
    return result;
}

void push(cell val, stack *p)
{
    if (p->capacity == p->size_stack)
    {
        grow_stack(p);
    }
    p->array[p->size_stack] = val;
    p->size_stack = p->size_stack + 1;
    return;
}

queue *create_queue(void)
{
    queue *q = malloc(sizeof(*q));
    q->left = create_stack();
    q->right = create_stack();
    q->size = 0;
    return q;
}

void delete_queue(queue *q)
{
    delete_stack(q->right);
    delete_stack(q->left);
    free(q);
    return;
}

bool isempty_queue(queue *q)
{
    return isempty_stack(q->left) && isempty_stack(q->right);
}

void enqueue(cell val, queue *q)
{
    push(val, q->left);
    q->size++;
    return;
}

cell dequeue(queue *q)
{
    if (isempty_stack(q->right))
    {
        while (!isempty_stack(q->left))
        {
            push(pop(q->left), q->right);
        }
    }
    q->size--;
    return pop(q->right);
}

int get_queuesize(queue *q)
{
    if (q == NULL)
    {
        return -1;
    }
    return q->size;
}

int getsize_queue(queue *q)
{
    int cpt = 0;
    queue *q1 = create_queue();
    while (!isempty_queue(q))
    {
        enqueue(dequeue(q), q1);
        cpt += 1;
    }
    while (!isempty_queue(q1))
    {
        enqueue(dequeue(q1), q);
    }
    delete_queue(q1);
    return cpt;
}

queue *copy_queue(queue *q)
{
    queue *qc = create_queue();
    for (int i = 0; i < getsize_queue(q); i++)
    {
        cell val = dequeue(q);
        enqueue(val, qc);
        enqueue(val, q);
    }
    return qc;
}
