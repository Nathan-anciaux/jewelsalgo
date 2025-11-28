////////////////////////////
/// Primitives des files ///
////////////////////////////

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct cell cell;
typedef struct stack stack;

/////////////////////////
// Déclaration du type //
/////////////////////////

typedef struct queue
{
    stack *left, *right;
    int size;
} queue;

// #include "game.h"
// #include "stack.h"

///////////////////////////
// Fonctions primitives. //
///////////////////////////

// Crée une file vide.
queue *create_queue(void);

// Libère la mémoire allouée pour la file.
void delete_queue(queue *);

// Renvoie vrai si la file est vide, faux sinon.
bool isempty_queue(queue *);

// Ajoute une valeur dans la file, à gauche de la valeur la plus à gauche.
void enqueue(cell, queue *);

// Renvoie la valeur la plus à droite de la file et la retire.
cell dequeue(queue *);

// Renvoir size de la queue
int get_queuesize(queue *q);

// Renvoie le nombre d'éléments dans la file, sans la modifier.
int getsize_queue(queue *q);

// Crée une copie d'une file.
queue *copy_queue(queue *q);

///////////////////////////
/// Primitive des piles ///
///////////////////////////

// #include "game.h"

typedef struct cell cell;

typedef struct stack stack;

// Crée une pile vide.
stack *create_stack(void);

// Libère la mémoire allouée pour la pile.
void delete_stack(stack *);

// Renvoie vrai si la pile est vide, faux sinon.
bool isempty_stack(stack *);

// Renvoie la valeur au sommet de la pile et la retire. Si la pile est vide, la
// fonction affiche un message sur la sortie erreur standard et termine le
// programme. Si l'occupation du tableau tombe à 25% après le pop(), le tableau
// est redimensionné par la fonction shrink_stack().
cell pop(stack *);

// Ajoute une valeur au sommet de la pile. Si le tableau est plein, il est
// redimensionné au préalable, par la fonction grow_stack.
void push(cell, stack *);

#include "game.h"
#endif // QUEUE_H_
