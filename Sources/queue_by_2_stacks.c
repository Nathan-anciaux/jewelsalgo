
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

// cell part
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

// int part ********************************************************************************

typedef struct stack_int
{
    int capacity;   // Taille totale du tableau alloué en mémoire.
    int size_stack; // Nombre d'éléments dans la pile représentée.
    int *array;     // Tableau contenant les valeurs de la pile représentée.
} stack_int;

// Double la taille du tableau utilisé dans la représentation.
// Cette fonction sera utilisée lorsque le tableau est plein et qu'on veut y
// ajouter une valeur.
static void grow_stack_int(stack_int *p)
{
    p->array = realloc(p->array, 2 * p->capacity * (sizeof(int)));
    p->capacity *= 2;
    return;
}

static void shrink_stack_int(stack_int *p)
{
    if (p->capacity == 1)
    {
        printf("the table is too short !");
        return;
    }
    p->array = realloc(p->array, p->capacity / 2 * sizeof(int));
    p->capacity /= 2;
    return;
}

stack_int *create_stack_int(void)
{
    stack_int *p = malloc(sizeof(stack_int));
    p->capacity = 1;
    p->size_stack = 0;
    p->array = malloc((p->capacity) * sizeof(int));
    return p;
}

void delete_stack_int(stack_int *p)
{
    free(p->array);
    free(p);
    return;
}

bool isempty_stack_int(stack_int *p)
{
    return p->size_stack == 0;
}

int pop_int(stack_int *p)
{

    if (isempty_stack_int(p))
    {
        fprintf(stderr, "the table is too short ! \n");
        return 0;
    }
    p->size_stack--;
    int result = p->array[p->size_stack];
    if ((p->size_stack * 100 / p->capacity) < 25)
    {
        shrink_stack_int(p);
    }
    return result;
}

void push_int(int val, stack_int *p)
{
    if (p->capacity == p->size_stack)
    {
        grow_stack_int(p);
    }
    p->array[p->size_stack] = val;
    p->size_stack = p->size_stack + 1;
    return;
}

queue_int *create_queue_int(void)
{
    queue_int *q = malloc(sizeof(*q));
    q->left = create_stack_int();
    q->right = create_stack_int();
    q->size = 0;
    return q;
}

void delete_queue_int(queue_int *q)
{
    delete_stack_int(q->right);
    delete_stack_int(q->left);
    free(q);
    return;
}

bool isempty_queue_int(queue_int *q)
{
    return isempty_stack_int(q->left) && isempty_stack_int(q->right);
}

void enqueue_int(int val, queue_int *q)
{
    push_int(val, q->left);
    q->size++;
    return;
}

int dequeue_int(queue_int *q)
{
    if (isempty_stack_int(q->right))
    {
        while (!isempty_stack_int(q->left))
        {
            push_int(pop_int(q->left), q->right);
        }
    }
    q->size--;
    return pop_int(q->right);
}

int get_queuesize_int(queue_int *q)
{
    if (q == NULL)
    {
        return -1;
    }
    return q->size;
}

int getsize_queue_int(queue_int *q)
{
    int cpt = 0;
    queue_int *q1 = create_queue_int();
    while (!isempty_queue_int(q))
    {
        enqueue_int(dequeue_int(q), q1);
        cpt += 1;
    }
    while (!isempty_queue_int(q1))
    {
        enqueue_int(dequeue_int(q1), q);
    }
    delete_queue_int(q1);
    return cpt;
}

queue_int *copy_queue_int(queue_int *q)
{
    queue_int *qc = create_queue_int();
    for (int i = 0; i < getsize_queue_int(q); i++)
    {
        int val = dequeue_int(q);
        enqueue_int(val, qc);
        enqueue_int(val, q);
    }
    return qc;
}
