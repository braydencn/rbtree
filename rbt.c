/* Copyright(c) 2013 Brayden Zhang.
 * Mail: pczhang2010@gmail.com
 */

#include "rbt.h"

/* red-black tree:
 * 1. node should be either black or red;
 * 2. root is black;
 * 3. all leaf nodes(NULL) are black;
 * 4. red node only has black children;
 * 5. all simple paths from one node to all of its leaf nodes
 *    contain the same number of black nodes;
 */

void 
right_rotate(rbt_node_t** root, rbt_node_t* n)
{
	rbt_node_t* p = n->p;
	rbt_node_t* a = p->p;
	n->p = a;
	if(a == NULL){
		*root = n;
	}else if(a->l == p){
		a->l = n;
	}else a->r = n;
	p->l = n->r;
	if(n->r != NULL)
		n->r->p = p;
	n->r = p;
	p->p = n;
}

void
left_rotate(rbt_node_t** root, rbt_node_t* n)
{
	rbt_node_t* p = n->p;
	rbt_node_t* a = p->p;
	n->p = a;
	if(a == NULL){
		*root = n;
	}else if(a->l == p)
		a->l = n;
	else a->r = n;
	p->r = n->l;
	if(n->l != NULL)
		n->l->p = p;
	n->l = p;
	p->p = n;
}

rbt_node_t* 
rbt_new_node(int k)
{
	rbt_node_t* n = (rbt_node_t*) malloc(sizeof(rbt_node_t));
	if(n == NULL){
		fprintf(stderr, "failed to allocate memory.\n");
		exit(-1);
	}
	n->k = k;
	n->c = RED;
	n->l = n->r = n->p = NULL;
	return n;
}

rbt_node_t* 
rbt_search(rbt_node_t* r, int k)
{
	if(r == NULL || r->k == k)
		return r;
	if(r->k > k)
		rbt_search(r->l, k);
	else
		rbt_search(r->r, k);
}

rbt_node_t* 
rbt_search2(rbt_node_t* r, int k)
{
	while(r != NULL && r->k != k){
		if( r->k > k)
			r = r->l;
		else
			r = r->r;
	}
	return r;
}

rbt_node_t* 
rbt_min(rbt_node_t* r)
{
	while(r->l != NULL)
		r = r->l;
	return r;
}

rbt_node_t* 
rbt_max(rbt_node_t* r)
{
	while(r->r != NULL)
		r = r->r;
	return r;
}

rbt_node_t* 
rbt_predecessor(rbt_node_t* r)
{
	if(r->l != NULL)
		return rbt_max(r->l);
	rbt_node_t *p = r->p;
	while(p != NULL && r != p->r){
		r = p;
		p = p->p;
	}
	return p;
}

rbt_node_t* 
rbt_successor(rbt_node_t* r)
{
	if(r->r != NULL)
		return rbt_min(r->r);
	rbt_node_t *p = r->p;
	while(p != NULL && r != p->l){
		r = p;
		p = p->p;
	}
	return p;
}
	
void 
swap(rbt_node_t** n, rbt_node_t** n1)
{
	rbt_node_t* t = *n;
	*n = *n1;
	*n1 = t;
}

int 
rbt_is_black(rbt_node_t* n)
{
	return n == NULL || n->c == BLACK;
}

int
rbt_is_red(rbt_node_t* n)
{
	return n != NULL && n->c == RED;
}

void 
set_color(rbt_node_t* n, rbt_color_t c)
{
	n->c = c;
}

void
set_black(rbt_node_t* n)
{
	set_color(n, BLACK);
}

void
set_red(rbt_node_t* n)
{
	set_color(n, RED);
}

void 
swap_color(rbt_node_t* n, rbt_node_t* n1)
{
	rbt_color_t tmp = n->c;
	set_color(n, n1->c);
	set_color(n1,tmp); 
}

/* New node insert is initially red. We should ajust as:
 * case1: new node is root -> 
 * 		set node to black;
 * case2: parent is black -> 
 *		no action
 * case3: parent and uncle are red -> 
 * 		turn them to black;
 *		change grandparent to red;
 *		recursively ajust the red nodes.
 * case4: parent is red, uncle is black ->
 *		if LR, left-rotate parent;
 *		if RL, right-rotate parent;
 *		thus only LL and RR left to be ajusted ->
 *			set parent to black; 
 *			set grandparent to red;
 *			if LL, right-rotate grantparent;
 *			if RR, left-rotate grantparent.
 * NOTE: steps above are not restrict to newly-added nodes as fixup_insert 
 *       maybe recursily done for ajustment
 */
void 
rbt_fixup_insert_node(rbt_node_t** root, rbt_node_t* n)
{
	rbt_node_t* p = n->p;
	if(!p){ //case1
		set_black(n);
		return; 
	}
	rbt_node_t* pp = p->p;
	/* case2. note this includes the case that p is root */
	if(rbt_is_black(p))
		return;
	rbt_node_t* u;
	if(pp->l == p)
		u = pp->r;
	else u = pp->l;
	if(u && u->c == RED){ //case3
		set_black(p);
		set_black(u);
		set_red(pp);
		rbt_fixup_insert_node(root, pp);
	}else{  //case4
		if(p == pp->l && n == p->r){
			left_rotate(root, n);
			swap(&p, &n);
		}
		if(p == pp->r && n == p->l){
			right_rotate(root, n);
			swap(&p, &n);
		}
		set_black(p);
		set_red(pp);
		if(p == pp->l && n == p-> l)
			right_rotate(root, p);
		if(p == pp->r && n == p->r)
			left_rotate(root, p);
	}
}

int 
rbt_insert_node(rbt_node_t** root, int k)
{
	rbt_node_t* r = *root;
	rbt_node_t* p = r;
	if(!r){
		rbt_node_t* n = rbt_new_node(k);
		*root = n;
		rbt_fixup_insert_node(root, n);
		return 0;
	}
	while( r && r->k != k){
		p=r;
		if(r->k > k)
			r = r->l;
		else
			r = r->r;
	}
	if(!r){
		rbt_node_t* n = rbt_new_node(k);
		if(p->k > k)
			p->l = n;
		else 
			p->r = n;
		n->p = p;
		rbt_fixup_insert_node(root, n);
		return 0;
	}
	return 1;
}


/* Fixup after delete.
 * Note that for bst delete, we only need to consider node which has
 * at most one child.
 * case1: node deleted is red -> 
 *		no action needed.
 * case2: node deleted is black, its child is red ->
 * 		mark child to black.
 * case3: node deleted is black, its child is black ->
 * 		case3.1 node deleted is root ->
 *			no action needed.
 *		case3.2 sibling is red, node is left child ->
 *			set sibling to black; set parent to red;
 *			left-rotate parent
 *			(change this to case 3.3, 3.4, 3,5)
 *		case3.2.1 sibling is red, node is right child ->
 *			same as above, but right-rotate parent.
 *		case3.3 sibling is black, parent is black,
 *			sibling's children are black ->
 *			set sibling to red; recursively ajust parent from case3.
 *		case3.4 sibling is black, sibling's children are black,
 *			parent is red ->
 *			set sibling to red, set parent to black.
 *		case3.5 sibling is black, sibling's left child is red, 
 *			sibling's right child is black, node is left child 
 *			->
 *			set sibling to red,set sibling's left child to black
 *			right-rotate sibling. (change this to case 3.6)
 *		case3.5.1 sibling is black, sibling's right child is red,
 *                      sibling's left child is black, node is right child
 *                      ->
 *                      set sibling to red,set sibling's right child to bla 
 *                      left-rotate sibling. (change this to case 3.6.1)
 *		case3.6 sibling is black, sibling's right child is red
 *			node is left child ->
 *			left-rotate parent, exchange color of node and pare
 *			set sibling's right child to black
 *		case3.6.1 sibling is black, sibling's left child is red
 *			node is right child ->
 *			right-rotate parent, exchange color of node and par
 *			set sibling's left child to black
 * Parameters:
 * @ rbt_node_t** u -- node deleted
 * @ rbt_node_t* v  -- child node of the node deleted. Note: v is the only child of u
 */
void rbt_fixup_delete_node2(rbt_node_t** root, rbt_node_t* p, rbt_node_t* v);

void
rbt_fixup_delete_node(rbt_node_t** root, rbt_node_t* n, rbt_node_t* v) // only deal with case1 and case2
{
	//if(is_red(n))//case1, just do nothing
	if(rbt_is_black(n)){
		if(v && rbt_is_red(v)){ //case2
			set_black(v);
		}else rbt_fixup_delete_node2(root, n->p, v);
	}
}

/* color of v is BLACK; 
 * all path of v lack of 1 black node;
 * thus violate Rule5.
 */
void 
rbt_fixup_delete_node2(rbt_node_t** root, rbt_node_t* p, rbt_node_t* v) 
{
	if(!p) //case3.1
		return;
	rbt_node_t* s = p->l; //sibling is never NULL due to Rule5
	if(s == v)
		s = p->r;
	rbt_node_t* sl = s->l;
	rbt_node_t* sr = s->r;
	if(rbt_is_red(s)){// case 3.2, 3.2.1; ajust will continue after case3.2, 3.2.1.
		set_black(s);
		set_red(p);
		if(v == p->l){
			left_rotate(root, s);
			s = sl;
		}else{
			right_rotate(root, s);
			s = sr;
		}
		sl = s->l; 
		sr = s->r;
	}
	if(rbt_is_black(s) && (! sl || rbt_is_black(sl)) && (! sr || rbt_is_black(sr))){ 
		if(rbt_is_black(p)){//case3.3
			set_red(s);
			rbt_fixup_delete_node2(root,p->p, p);
		}else{ //case3.4
			set_red(s);
			set_black(p);
			return;
		}
	}
	if(v == p->l){
		if(rbt_is_black(s) && rbt_is_red(sl) && rbt_is_black(sr)){//case3.5
			set_red(s);
			set_black(sl);
			right_rotate(root, sl);
			s = sl;
			sl = s->l;
			sr = s->r;
		}
		if(rbt_is_black(s) && rbt_is_red(sr)){ //case3.6
			swap_color(p, s);
			set_black(sr);
			left_rotate(root, s);
		}
	}else{
		if(rbt_is_black(s) && rbt_is_red(sr) && rbt_is_black(sl)){//case3.5.1
			set_red(s);
			set_black(sr);
			left_rotate(root, sr);
			s = sr;
			sl = s->l;
			sr = s->r;
		}
		if(rbt_is_black(s) && rbt_is_red(sl)){ //case3.6.1
			swap_color(p, s);
			set_black(sl);
			right_rotate(root, s);
		}
	}	
}

void 
transplant(rbt_node_t** root, rbt_node_t** u, rbt_node_t* v)
{
	rbt_node_t* n = *u;
	if(n->p == NULL){
		*u = v;
		*root = v;
	}else if(n == n->p->l)
		n->p->l = v;
	else n->p->r = v;
	if(v != NULL) v->p = n->p;
	rbt_fixup_delete_node(root, n, v);
	free(n);
}

int 
rbt_delete_node(rbt_node_t** root, rbt_node_t** u)
{
	rbt_node_t* n = *u;
	if(n->l == NULL)
		transplant(root, u, n->r);
	else if(n->r == NULL)
		transplant(root, u, n->l);
	else{
		rbt_node_t* p = rbt_predecessor(n); // max(n->l)
		n->k = p->k;
		rbt_delete_node(root, &p);
		return 0;
	}
}

void 
rbt_print_node(rbt_node_t* n)
{
	printf(" (%d,%c) ", n->k, n->c ? 'B' : 'R');
}

void
rbt_traverse_in_order(rbt_node_t* r, void (*func_p)(rbt_node_t* n))
{
	if(r == NULL) return;
	rbt_traverse_in_order(r->l, func_p);
	(*func_p)(r);
	rbt_traverse_in_order(r->r, func_p);
}

void
rbt_traverse_pre_order(rbt_node_t* r, void (*func_p)(rbt_node_t* n))
{
	if(r == NULL) return;
	(*func_p)(r);
	rbt_traverse_pre_order(r->l, func_p);
	rbt_traverse_pre_order(r->r, func_p);
}
