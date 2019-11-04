#include "List.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>


// NodeObj
typedef struct NodeObj {
	void* item;
	struct NodeObj* next;
	struct NodeObj* previous;
} NodeObj;

// Node
typedef NodeObj* Node;

// newNode()
// constructor of the Node type
Node newNode(void* x) {
	Node N = malloc(sizeof(NodeObj));
	assert(N != NULL);
	N->item = x;
	N->previous = NULL;
	N->next = NULL;
	return(N);
}

// freeNode()
// destructor for the Node type
void freeNode(Node* pN) {
	if (pN != NULL && *pN != NULL) {
		free(*pN);
		*pN = NULL;
	}
}

typedef struct ListObj {
	Node front;
	Node back;
	Node cursor;
	int number;
	int size;
} ListObj;

List newList(void)
{
	List L = malloc(sizeof(ListObj));
	assert(L != NULL);
	L->front = NULL;
	L->back = NULL;
	L->cursor = NULL;
	L->number = -1;
	L->size = 0;
	return L;
}

void freeList(List* pL)
{
	if (pL != NULL && *pL != NULL) {
		if (length(*pL) != 0) clear(*pL);
		free(*pL);
		*pL = NULL;
	}
}

int length(List L)
{
	return L->size;
}

int index(List L)
{
	return L->number;
}

void* front(List L)
{
	return L->front->item;
}

void* back(List L)
{
	return L->back->item;
}

void* get(List L)
{
	return L->cursor->item;
}


void clear(List L)
{
	int count = L->size;
	for (int i = 0; i < count; i++) {
		moveBack(L);
		delete(L);
	}
}

void moveFront(List L)
{
	if (L->size != 0) {
		L->cursor = L->front;
		L->number = 0;
	}
}

void moveBack(List L)
{
	if (L->size != 0) {
		L->cursor = L->back;
		L->number = L->size - 1;
	}
}

void movePrev(List L)
{
	if (L->cursor != NULL && L->cursor->item != L->front->item) {
		L->cursor = L->cursor->previous;
		L->number--;
	}
	else {
		if (L->cursor != NULL && L->cursor->item == L->front->item) {
			L->cursor = NULL;
			L->number = -1;
		}
	}
}

void moveNext(List L)
{
	if (L->cursor != NULL && L->cursor->item != L->back->item) {
		L->cursor = L->cursor->next;
		L->number++;
	}
	else {
		if (L->cursor != NULL && L->cursor->item == L->back->item) {
			L->cursor = NULL;
			L->number = -1;
		}
	}
}

void prepend(List L, void* data)
{
	Node W = newNode(data);
	Node N = NULL;
	if (L->size == 0) {
		L->front = W;
		L->back = W;
	}
	else {
		N = L->front;
		W->next = N;
		N->previous = NULL;
		N->previous = W;
		L->front = W;
		if (L->number != -1) {
			L->number++;
		}
	}
	L->size++;
}

void append(List L, void* data)
{
	Node F = newNode(data);
	Node N = NULL;
	if (L->size == 0) {
		L->front = F;
		L->back = F;
	}
	else {
		N = L->back;
		L->back = F;
		N->next = F;
		F->previous = N;
	}
	L->size++;
}

void insertBefore(List L, void* data)
{
	if (L->size != 0 && L->number != -1) {
		Node N = NULL;
		Node P = NULL;
		Node C = NULL;
		P = newNode(data);
		if (L->size == 1) {
			L->front = P;
			P->next = L->cursor;
			L->cursor->previous = P;
		}
		else {
			if (L->cursor->item == L->front->item) {
				C = L->cursor;
				C->previous = P;
				P->next = C;
				L->front = P;
			}
			else {
				C = L->cursor;
				N = C->previous;
				N->next = P;
				P->previous = N;
				P->next = C;
				C->previous = P;
			}
		}
		L->number++;
		L->size++;
	}
}

void insertAfter(List L, void* data)
{
	if (L->size != 0 && L->number != -1) {
		Node N = NULL;
		Node P = NULL;
		Node C = NULL;
		P = newNode(data);
		if (L->size == 1) {
			L->back = P;
			P->previous = L->cursor;
			L->cursor->next = P;
		}
		else {
			if (L->cursor->item == L->back->item) {
				C = L->cursor;
				C->next = P;
				P->previous = C;
				L->back = P;
			}
			else {
				C = L->cursor;
				N = C->next;
				N->previous = P;
				P->next = N;
				P->previous = C;
				C->next = P;
			}
		}
		L->size++;
	}
}

void deleteFront(List L)
{
	if (L->size != 0) {
		Node N = NULL;
		Node P = NULL;
		if (L->size == 1) {
			N = L->front;
			freeNode(&N);
			L->front = NULL;
			L->back = NULL;
			if (L->number != -1) {
				L->cursor = NULL;
				L->number = -1;
			}
		}
		else {
			if (L->cursor != NULL && L->cursor->item != L->front->item) {
				L->number--;
			}
			if (L->cursor != NULL && L->cursor->item == L->front->item) {
				L->cursor = NULL;
				L->number = -1;
			}
			N = L->front;
			P = L->front->next;
			freeNode(&N);
			P->previous = NULL;
			L->front = P;
		}
		L->size--;
	}
}

void deleteBack(List L)
{
	if (L->size != 0) {
		Node N = NULL;
		Node P = NULL;
		if (L->size == 1) {
			N = L->back;
			freeNode(&N);
			L->front = NULL;
			L->back = NULL;
			if (L->number != -1) {
				L->cursor = NULL;
				L->number = -1;
			}
		}
		else {
			if (L->cursor != NULL && L->cursor->item == L->back->item) {
				L->cursor = NULL;
				L->number = -1;
			}
			N = L->back;
			P = L->back->previous;
			freeNode(&N);
			P->next = NULL;
			L->back = P;
		}
		L->size--;
	}
}

void delete(List L) {
	if (L->size != 0 && L->number != -1) {
		Node N = NULL;
		Node P = NULL;
		Node C = NULL;
		C = L->cursor;
		P = L->cursor->previous;
		N = L->cursor->next;
		if (L->size == 1) {
			freeNode(&C);
			L->front = NULL;
			L->back = NULL;
		}
		else {
			C->next = NULL;
			C->previous = NULL;
			freeNode(&C);
			if (P != NULL && N == NULL) {
				P->next = NULL;
				L->back = P;

			}
			else if (P == NULL && N != NULL) {
				N->previous = NULL;
				L->front = N;
			}
			else {
				P->next = N;
				N->previous = P;
			}
		}
		L->cursor = NULL;
		L->number = -1;
		L->size--;
	}
}

