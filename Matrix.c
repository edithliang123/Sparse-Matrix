#include "Matrix.h"
#include "List.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

//EntryObj
typedef struct EntryObj {
	int column;
	double value;
} EntryObj;

//Entry
typedef EntryObj* Entry;

//newEntry
//constructor of the Entry type
Entry newEntry(int x, double y) {
	Entry N = malloc(sizeof(EntryObj));
	assert(N != NULL);
	N->column = x;
	N->value = y;
	return (N);
}

//freeEntry()
//destructor for the Entry type
void freeEntry(Entry* pN) {
	if (pN != NULL && *pN != NULL) {
		free(*pN);
		*pN = NULL;
	}
}

typedef struct MatrixObj {
	int size;
	int NNZ;
	List* row;
} MatrixObj;

// Matrix
typedef MatrixObj* Matrix;

Matrix newMatrix(int n) {
	Matrix M = malloc(sizeof(MatrixObj));
	assert(M != NULL);
	M->size = n;
	M->NNZ = 0;
		M->row = malloc((n + 1) * sizeof(List));
		for (int i = 0; i <= M->size; i++) {
			M->row[i] = newList();
		}
	return M;
}

void freeMatrix(Matrix* pM) {
	if (pM != NULL && *pM != NULL) {
		if (size(*pM) != 0) makeZero(*pM);
		for (int i = 0; i <= (*pM)->size; i++) {
			freeList(&((*pM)->row[i]));
		}
		free((*pM)->row);
		free(*pM);
		*pM = NULL;
	}
}

int size(Matrix M) {
	return M->size;
}

int NNZ(Matrix M) {
	return M->NNZ;
}

int equals(Matrix A, Matrix B) {
	int eq = 0;
	if (A == NULL || B == NULL) {
		printf("Matrix Error: calling equals() on NULL Matrix reference\n");
		exit(1);
	}
	eq = (A->size == B->size);
	eq = eq && (A->NNZ == B->NNZ);
	for (int i = 1; i <= A->size; i++) {
		if (length(A->row[i]) != 0 && length(B->row[i]) != 0) {
			moveFront(A->row[i]);
			moveFront(B->row[i]);
			while (eq && index(A->row[i]) != -1) {
				eq = eq && (((Entry)get(A->row[i]))->column == ((Entry)get(B->row[i]))->column);
				eq = eq && (((Entry)get(A->row[i]))->value == ((Entry)get(B->row[i]))->value);
				moveNext(A->row[i]);
				moveNext(B->row[i]);
			}
		}
		else {
			break;
		}
	}
	return eq;
}

void makeZero(Matrix M) {
	if (M == NULL) {
		fprintf(stderr, "makeZero() called on NULL Matrix reference\n");
		exit(1);
	}
	for (int i = 1; i <= M->size; i++) {
		List temp = M->row[i];
		moveFront(M->row[i]);
		while (index(temp) != -1) {
			Entry W = get(temp);
			freeEntry(&W);
			moveNext(M->row[i]);
		}
		clear(temp);
	}
	M->NNZ = 0;
}

void changeEntry(Matrix M, int i, int j, double x) {
	if (i >= 1 && i <= M->size && j >= 1 && j <= M->size) {
		if (length(M->row[i]) != 0) {
			int find = 0;
			moveFront(M->row[i]);
			for (int y = 1; y <= length(M->row[i]); y++) {
				if (((Entry)get(M->row[i]))->column == j) {
					if (x != 0) {
						Entry W = get(M->row[i]);
						W->value = x;
						find = 1;
					}
					else {
						Entry W = get(M->row[i]);
						freeEntry(&W);
						delete(M->row[i]);
						M->NNZ--;
					}
					break;
				}
				moveNext(M->row[i]);
			}
			if (find == 0 && x != 0) {
				Entry W = newEntry(j, x);
				if (j > ((Entry)back(M->row[i]))->column) {
					append(M->row[i], W);
					M->NNZ++;
				}
				else {
					moveFront(M->row[i]);
					for (int z = 1; z <= length(M->row[i]); z++) {
						if (j < ((Entry)get(M->row[i]))->column) {
							insertBefore(M->row[i], W);
							M->NNZ++;
							break;
						}
						moveNext(M->row[i]);
					}
				}
			}
		}
		else {
			if (x != 0) {
				append(M->row[i], newEntry(j, x));
				M->NNZ++;
			}
		}
	}
}


Matrix copy(Matrix A) {
	Matrix B = newMatrix(A->size);
	B->NNZ = A->NNZ;
	for (int i = 1; i <= A->size; i++) {
		moveFront(A->row[i]);
		if (length(A->row[i]) != 0) {
			for (int j = 1; j <= length(A->row[i]); j++) {
				append(B->row[i], newEntry(((Entry)get(A->row[i]))->column, ((Entry)get(A->row[i]))->value));
				moveNext(A->row[i]);
			}
		}
	}
	return B;
}


Matrix transpose(Matrix A) {
	Matrix C = newMatrix(A->size);
	C->NNZ = A->NNZ;
	int j;
	for (int i = 1; i <= C->size; i++) {
		j = 1;
		if (length(A->row[i]) != 0) {
			moveFront(A->row[i]);
			while (j <= A->size) {
				if (index(A->row[i]) != -1) {
					if (((Entry)get(A->row[i]))->column == j) {
						append(C->row[j], newEntry(i, ((Entry)get(A->row[i]))->value));
						moveNext(A->row[i]);
						j++;
					}
					else {
						j++;
					}
				}
				else {
					break;
				}
			}
		}
	}
	return C;
}


Matrix scalarMult(double x, Matrix A) {
	double y = 0.0;
	Matrix S = newMatrix(A->size);
	if (x != 0) {
		S->NNZ = A->NNZ;
		for (int i = 1; i <= A->size; i++) {
			if (length(A->row[i]) != 0) {
				moveFront(A->row[i]);
				for (int j = 1; j <= length(A->row[i]); j++) {
					y = ((Entry)get(A->row[i]))->value;
					append(S->row[i], newEntry(((Entry)get(A->row[i]))->column, y * x));
					moveNext(A->row[i]);
				}
			}
		}
	}
	return S;
}


Matrix sum(Matrix A, Matrix B) {
	Matrix C = NULL;
	C = newMatrix(A->size);
	if (A != NULL && B != NULL && A->size == B->size) {
		int count;
		int find = -1;
		int find2 = -1;
		double value = 0.0;
		double value2 = 0.0;
		for (int i = 1; i <= A->size; i++) {
			count = 1;
			if (length(A->row[i]) != 0 && length(B->row[i]) != 0) {
				for (int s = 1; s <= A->size; s++) {
					moveFront(A->row[i]);
					for (int j = 1; j <= length(A->row[i]); j++) {
						if (count == ((Entry)get(A->row[i]))->column) {
							find = 1;
							value = ((Entry)get(A->row[i]))->value;
							break;
						}
						moveNext(A->row[i]);
					}
					moveFront(B->row[i]);
					for (int x = 1; x <= length(B->row[i]); x++) {
						if (count == ((Entry)get(B->row[i]))->column) {
							find2 = 1;
							value2 = ((Entry)get(B->row[i]))->value;
							break;
						}
						moveNext(B->row[i]);
					}
					if (find != -1 && find2 != -1) {
						if (value + value2 != 0) {
							append(C->row[i], newEntry(count, value + value2));
							C->NNZ++;
						}
					}
					if (find != -1 && find2 == -1) {
						append(C->row[i], newEntry(count, value));
						C->NNZ++;
					}
					if (find == -1 && find2 != -1) {
						append(C->row[i], newEntry(count, value2));
						C->NNZ++;
					}
					count++;
					find = -1;
					find2 = -1;
				}
			}
			if (length(A->row[i]) != 0 && length(B->row[i]) == 0) {
				moveFront(A->row[i]);
				for (int f = 1; f <= length(A->row[i]); f++) {
					append(C->row[i], newEntry(((Entry)get(A->row[i]))->column, ((Entry)get(A->row[i]))->value));
					C->NNZ++;
					moveNext(A->row[i]);
				}
			}
			if (length(A->row[i]) == 0 && length(B->row[i]) != 0) {
				moveFront(B->row[i]);
				for (int w = 1; w <= length(B->row[i]); w++) {
					append(C->row[i], newEntry(((Entry)get(B->row[i]))->column, ((Entry)get(B->row[i]))->value));
					C->NNZ++;
					moveNext(B->row[i]);
				}
			}
		}
	}
	return C;
}


Matrix diff(Matrix A, Matrix B) {
	Matrix D = newMatrix(A->size);
	if (A != NULL && B != NULL && A->size == B->size) {
		int count;
		int find = -1;
		int find2 = -1;
		double value = 0.0;
		double value2 = 0.0;
		for (int i = 1; i <= A->size; i++) {
			count = 1;
			if (length(A->row[i]) != 0 && length(B->row[i]) != 0) {
				for (int s = 1; s <= A->size; s++) {
					moveFront(A->row[i]);
					for (int j = 1; j <= length(A->row[i]); j++) {
						if (count == ((Entry)get(A->row[i]))->column) {
							find = 1;
							value = ((Entry)get(A->row[i]))->value;
							break;
						}
						moveNext(A->row[i]);
					}
					moveFront(B->row[i]);
					for (int j = 1; j <= length(B->row[i]); j++) {
						if (count == ((Entry)get(B->row[i]))->column) {
							find2 = 1;
							value2 = -1 * ((Entry)get(B->row[i]))->value;
							break;
						}
						moveNext(B->row[i]);
					}
					if (find != -1 && find2 != -1) {
						if (value + value2 != 0) {
							append(D->row[i], newEntry(count, value + value2));
							D->NNZ++;
						}
					}
					if (find != -1 && find2 == -1) {
						append(D->row[i], newEntry(count, value));
						D->NNZ++;
					}
					if (find == -1 && find2 != -1) {
						append(D->row[i], newEntry(count, value2));
						D->NNZ++;
					}
					count++;
					find = -1;
					find2 = -1;
				}
			}
			if (length(A->row[i]) != 0 && length(B->row[i]) == 0) {
				moveFront(A->row[i]);
				for (int f = 1; f <= length(A->row[i]); f++) {
					append(D->row[i], newEntry(((Entry)get(A->row[i]))->column, ((Entry)get(A->row[i]))->value));
					D->NNZ++;
					moveNext(A->row[i]);
				}
			}
			if (length(A->row[i]) == 0 && length(B->row[i]) != 0) {
				moveFront(B->row[i]);
				for (int w = 1; w <= length(B->row[i]); w++) {
					append(D->row[i], newEntry(((Entry)get(B->row[i]))->column, -1 * ((Entry)get(B->row[i]))->value));
					D->NNZ++;
					moveNext(B->row[i]);
				}
			}
		}
	}
	return D;
}

double vectorDot(List P, List Q) {
	int x;
	double total = 0.0;
	if (length(P) > length(Q)) {
		moveFront(Q);
		for (int i = 0; i < length(Q); i++) {
			moveFront(P);
			x = ((Entry)get(Q))->column;
			for (int j = 0; j < length(P); j++) {
				if (((Entry)get(P))->column == x) {
					total += ((Entry)get(P))->value * ((Entry)get(Q))->value;
					break;
				}
				moveNext(P);
			}
			moveNext(Q);
		}
	}
	else {
		moveFront(P);
		for (int i = 0; i < length(P); i++) {
			moveFront(Q);
			x = ((Entry)get(P))->column;
			for (int j = 0; j < length(Q); j++) {
				if (((Entry)get(Q))->column == x) {
					total += ((Entry)get(P))->value * ((Entry)get(Q))->value;
					break;
				}
				moveNext(Q);
			}
			moveNext(P);
		}
	}
	return total;
}

Matrix product(Matrix A, Matrix B) {
	Matrix F = NULL;
	if (A != NULL && B != NULL && A->size == B->size) {
		F = newMatrix(A->size);
		Matrix C = transpose(B);
		int count;
		double total;
		for (int i = 1; i <= A->size; i++) {
			count = 1;
			if (length(A->row[i]) != 0) {
				for (int j = 1; j <= C->size; j++) {
					if (length(C->row[j]) != 0) {
						total = vectorDot(A->row[i], C->row[j]);
						if (total != 0) {
							append(F->row[i], newEntry(count, total));
							F->NNZ++;
						}
					}
					count++;
				}
			}
		}
		freeMatrix(&C);
	}
	return F;
}


void printMatrix(FILE* out, Matrix M) {
	for (int i = 1; i <= M->size; i++) {
		if (M->row[i] != NULL && length(M->row[i]) != 0) {
			fprintf(out, "%d: ", i);
			moveFront(M->row[i]);
			for (int j = 1; j <= length(M->row[i]); j++) {
				fprintf(out, "(%d, %0.1f) ", ((Entry)get(M->row[i]))->column, ((Entry)get(M->row[i]))->value);
				moveNext(M->row[i]);
			}
			fprintf(out, "\n");
		}
	}
}

