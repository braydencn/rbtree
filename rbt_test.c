/* Copyright(c) 2013 Brayden Zhang.
 * Mail: pczhang2010@gmail.com
 */

#include <assert.h>
#include "rbt.h"
#define RBT_NOPRINT

void
rbt_check_one_node(rbt_node_t* n)
{
	/** Rule2, node should be black or red **/
	assert(rbt_is_black(n) || rbt_is_red(n));

	/** Rule3, Nil is black. How to assert this? **/

	/** Rule4, Red nodes' children cannot be red **/
	if(rbt_is_red(n))
		assert(rbt_is_black(n->l) && rbt_is_black(n->r));
}

/* Rule5, all simple paths of a specific node should 
 * have the same black height.
 */
int 
rbt_check_rule5(rbt_node_t* r)
{
	if(!r) 
		return 1;
	int left_black_height = rbt_check_rule5(r->l);
	assert(left_black_height == rbt_check_rule5(r->r));
	return left_black_height + (rbt_is_black(r));
}

int 
rbt_check_tree(rbt_node_t* root)
{
	assert(rbt_is_black(root)); //Rule1. root is black
	rbt_traverse_pre_order(root,&rbt_check_one_node); 
	rbt_check_rule5(root);
}

void 
print_tree(rbt_node_t* root)
{
	printf("######BEGIN A TREE#############\n");
	rbt_traverse_pre_order(root, &rbt_print_node);
	printf("\n");
	rbt_traverse_in_order(root, &rbt_print_node);
	printf("\n");
	printf("#######END A TREE#############\n\n");
}

/* Insert 100 random numbers, then delete from the root */
void
test1()
{
	int i;
	rbt_node_t* root = NULL;
	for(i = 0; i < 100; i++){
		rbt_insert_node(&root, rand() % 10000);
#ifndef RBT_NOPRINT
		print_tree(root);
#endif
		rbt_check_tree(root);
	}
	while(root){
		rbt_delete_node(&root, &root);
#ifndef RBT_NOPRINT
		print_tree(root);
#endif
		rbt_check_tree(root);
	}
}

/* insert 100 random numbers, then delete from the minimum */
void
test2()
{
	int i;
	rbt_node_t* root = NULL;
	for(i = 0; i < 100; i++){
		rbt_insert_node(&root, rand() % 10000);
#ifndef RBT_NOPRINT
		print_tree(root);
#endif
		rbt_check_tree(root);
	}
	while(root){
		rbt_node_t* m = rbt_min(root);
		rbt_delete_node(&root, &m);
#ifndef RBT_NOPRINT
		print_tree(root);
#endif
		rbt_check_tree(root);
	}
}

/* insert 100 random number, then delete from the maximum */
void
test3()
{
	int i;
	rbt_node_t* root = NULL;
	for(i = 0; i < 100; i++){
		rbt_insert_node(&root, rand() % 10000);
#ifndef RBT_NOPRINT
		print_tree(root);
#endif
		rbt_check_tree(root);
	}
	while(root){
		rbt_node_t* m = rbt_max(root);
		rbt_delete_node(&root, &m);
#ifndef RBT_NOPRINT
		print_tree(root);
#endif
		rbt_check_tree(root);
	}
}

/* insert 100 random number, then delete randomly 
 * within root, root->r, root->l 
 */
void
test4()
{
	int i;
	rbt_node_t* root = NULL;
	for(i = 0; i < 100; i++){
		rbt_insert_node(&root, rand() % 10000);
#ifndef RBT_NOPRINT
		print_tree(root);
#endif
		rbt_check_tree(root);
	}
	while(root){
		rbt_node_t* node_2_del;
		if(! root->l && ! root->r)
			node_2_del = root;
		else if(! root->l)
			node_2_del = root->r;
		else if(! root->r)
			node_2_del = root->l;
		else
			node_2_del = (rand() % 2) ? root->l : root->r;
		rbt_delete_node(&root, &node_2_del);
#ifndef RBT_NOPRINT
		print_tree(root);
#endif
		rbt_check_tree(root);
	}
}

int
main(){
	int i;
	for(i = 10000; i > 0; i--){
		test1();
		test2();
		test3();
		test4();
	}
	return 0;
}
