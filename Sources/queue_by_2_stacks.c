//////////////////////////////////////////////
// Implémentation des files par deux piles. //
//////////////////////////////////////////////

// Le fichier main.h définit la macro QUEUE_BY_TWO_STACKS.
// Vous pouvez la changer.

#include "main.h"

// Si cette macro est vraie, l'implémentation des files par deux piles est
// utilisée, et le fichier queue.c est "vidé" par le précompilateur. C'est ce
// fichier, queue_by_2_stacks.c, qui sera utilisé.

// Si la macro est fausse, l'implémentation des files par tableau circulaire est
// utilisée (fichier queue.c), et le contenu de ce fichier est "vidé".

#include "queue.h"
#include "stack.h"

///////////////////////////////////////////
// Définition du type, à ne pas modifier //
///////////////////////////////////////////

typedef struct queue
{
    stack *left, *right;
    int size;
} queue;

///////////////////////////////////////////////////////////////////////////
// Fonctions primitives à écrire, documentées dans le fichier d'en-tête. //
///////////////////////////////////////////////////////////////////////////

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

bool isempty_queue(const queue *q)
{
    if (isempty_stack(q->left) && isempty_stack(q->right))
    {
        return true;
    }
    return false;
}

void enqueue(int val, queue *q)
{
    push(val, q->left);
    q->size++;
    return;
}

int dequeue(queue *q)
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
