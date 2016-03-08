/* 
 * Copyright(c) 2013 Brayden Zhang. 
 * Mail: pczhang2010@gmail.com
 */

#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>

/* red-black tree:
 * 1. rbt_node should be either black or red;
 * 2. root is black;
 * 3. all leaf rbt_nodes(NULL) are black;
 * 4. red rbt_node only has black children;
 * 5. all simple paths from one rbt_node to all of its leaf rbt_nodes
 *    contain the same number of black rbt_nodes;
 */

typedef enum {RED, BLACK} rbt_color_t;

typedef struct rbt_node
{
	int k;
	rbt_color_t c;
	struct rbt_node *l, *r, *p;
} rbt_node_t;

rbt_node_t* rbt_search(rbt_node_t* r, int k);

rbt_node_t* rbt_search2(rbt_node_t* r, int k);

rbt_node_t* rbt_min(rbt_node_t* r);

rbt_node_t* rbt_max(rbt_node_t* r);

int rbt_is_black(rbt_node_t* n);

int rbt_is_red(rbt_node_t* n);

rbt_node_t* rbt_predecessor(rbt_node_t* r);

rbt_node_t* rbt_successor(rbt_node_t* r);
	
int rbt_insert_node(rbt_node_t** root, int k);

int rbt_delete_node(rbt_node_t** root, rbt_node_t** u);

void rbt_print_node(rbt_node_t* n);

void rbt_traverse_in_order(rbt_node_t* r, void (*func_p)(rbt_node_t* n));

void rbt_traverse_pre_order(rbt_node_t* r, void (*func_p)(rbt_node_t* n));

#endif /* RBT_H */
