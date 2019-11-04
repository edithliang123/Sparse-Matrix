//-----------------------------------------------------------------------------
//  MatrixTest.c 
//  A test client for the Matrix ADT
//-----------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include"Matrix.h"
     
int main(){
   int n=100000;
   Matrix A = newMatrix(n);
   Matrix B = newMatrix(n);
   Matrix P = newMatrix(n);
   Matrix C, D, E, F, G, H;

   //test changeEntry function
   changeEntry(A, 1,1,1); changeEntry(B, 1,1,1); changeEntry(P, 1, 1, 0);
   changeEntry(A, 1,2,2); changeEntry(B, 1,2,0); changeEntry(P, 1, 2, 0);
   changeEntry(A, 1,3,3); changeEntry(B, 1,3,1); changeEntry(P, 1, 3, 0);
   changeEntry(A, 2,1,4); changeEntry(B, 2,1,0); changeEntry(P, 3, 1, 2);
   changeEntry(A, 2,2,5); changeEntry(B, 2,2,1); changeEntry(P, 3, 2, 1);
   changeEntry(A, 2,3,6); changeEntry(B, 2,3,0); changeEntry(P, 3, 3, 4);
   changeEntry(A, 3,1,7); changeEntry(B, 3,1,1);
   changeEntry(A, 3,2,8); changeEntry(B, 3,2,1);
   changeEntry(A, 3,3,9); changeEntry(B, 3,3,1);


   printf("%s\n", size(A) == size(P) ? "true" : "false");
   printf("\n");

   //test copy function
   H = copy(A);
   printf("%d\n", NNZ(H));
   printMatrix(stdout, H);
   printf("\n");

   printf("%s\n", equals(A, H) ? "true" : "false");
   printf("\n");

   changeEntry(A, 2, 2, 3);

   //test equals after changeEntry
   printf("%s\n", equals(A, H) ? "true" : "false");
   printf("\n");

   //test changeEntry case that if input is 0
   changeEntry(A, 2, 2, 0);

   printf("%d\n", NNZ(A));
   printMatrix(stdout, A);
   printf("\n");

   printf("%d\n", NNZ(B));
   printMatrix(stdout, B);
   printf("\n");

   printf("%d\n", NNZ(P));
   printMatrix(stdout, P);
   printf("\n");

   //test scalarMult with 0
   C = scalarMult(0, A);
   printf("%d\n", NNZ(C));
   printMatrix(stdout, C);
   printf("\n");

   //test scalarMult with negative number
   C = scalarMult(-4, A);
   printf("%d\n", NNZ(C));
   printMatrix(stdout, C);
   printf("\n");

   //test sum of Matrices A and P
   D = sum(A, P);
   printf("%d\n", NNZ(D));
   printMatrix(stdout, D);
   printf("\n");

   //test operation with negative numbers
   changeEntry(A, 1, 1, -1);
   changeEntry(A, 1, 2, -2);
   changeEntry(A, 1, 3, -3);

   //test diff of Matrices A and P
   E = diff(A, P);
   printf("%d\n", NNZ(E));
   printMatrix(stdout, E);
   printf("\n");

   //test transpose for P, which include empty list in middle
   F = transpose(P);
   printf("%d\n", NNZ(F));
   printMatrix(stdout, F);
   printf("\n");

   //test product of Matrices B and P
   G = product(B, P);
   printf("%d\n", NNZ(G));
   printMatrix(stdout, G);
   printf("\n");


   makeZero(P);
   printf("%d\n", NNZ(P));
   printMatrix(stdout, P);

   freeMatrix(&A);
   freeMatrix(&B);
   freeMatrix(&P);
   freeMatrix(&C);
   freeMatrix(&D);
   freeMatrix(&E);
   freeMatrix(&F);
   freeMatrix(&G);
   freeMatrix(&H);

   return EXIT_SUCCESS;
}
