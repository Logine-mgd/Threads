#include<stdlib.h>
#include<pthread.h>
#include <stdio.h>
#define Item(X, I, J, NCOL)      X[((J) + (I) * (NCOL))]

//Matrices A, B, C are defined globally also variables 
//l,m,n are defined globally but will be assigned to some values at matmult v1 and matmult v2
 int *MATRIX_A;
 int *MATRIX_B;
 int *MATRIX_C;
 int column_1;
 int column_2;




void matmult(int* A, int* B, int* C, int l, int m, int n)
{
    for(int i=0; i<l; i++)
        for(int j=0; j<n; j++)
        {
            int sum = 0;
            for(int k=0; k<m; k++)
                sum += Item(A, i, k, m) * Item(B, k, j, n);
            Item(C, i, j, n) = sum;
        }

}

void * calc_element(void* row_indexANDcol_index)
{
     
      int *arr2threads=(int *) (row_indexANDcol_index);  
      int sum = 0;
            for(int k=0; k<column_1; k++)
                sum += Item(MATRIX_A, arr2threads[0], k, column_1) * Item(MATRIX_B, k, arr2threads[1], column_2);
            Item(MATRIX_C, arr2threads[0], arr2threads[1], column_2) = sum;
    return NULL;
} 



void * calc_row_elements(void* row_index)
{
       
          int *arr2threads2=(int *) (row_index);     
          for(int j=0; j<column_2; j++)
           {
           int sum = 0;
            for(int k=0; k<column_1; k++)
                sum += Item(MATRIX_A, arr2threads2[0], k, column_1) * Item(MATRIX_B, k,j, column_2);
            Item(MATRIX_C, arr2threads2[0], j, column_2) = sum;
           }
           
    return NULL;
} 


void matmult_v1(int* A, int* B, int* C, int l, int m, int n)
{
      
      int size=l*n;
      pthread_t threads[size];          //Array of threads with the size = number of elements of Matrix C
      MATRIX_A=A;                       //Threads can access MATRICES A, B , C because they share the same address space
      MATRIX_B=B;
      MATRIX_C=C;
      column_1=m;
      column_2=n;   
      int arr2threads[l*n][2];          //2D array that will have the target row and column
  for(int i=0; i<l; i++)
    {
      for(int j=0; j<n; j++)
        {
      	     //[j+i*n] represent index of the element to be calculated in Matrix C
      	     arr2threads[j+i*n][0]=i;      // arr2threads[j+i*n][0] represents row_index
             arr2threads[j+i*n][1]=j;      // arr2threads[j+i*n][1] represents col_index
      	     pthread_create(&threads[j+i*n],NULL,calc_element,(void*) &arr2threads[j+i*n]);    
      	     //This creates a new thread and sends to it every time a struct with the needed attributes( row_index and col_index ) to perform the element multiplication
      	     //attributes needed to be sent are row_index (i) and col_index (j)
      	    
        }
    }
  for(int i=0; i<l*n; i++)
     {
      
      pthread_join(threads[i],NULL);
             
     } 
     C=MATRIX_C;
}





void matmult_v2(int* A, int* B, int* C, int l, int m, int n)
{
     
      pthread_t threads[l];          //Array of threads with the size = number off rows in Matrix A
      int arr2threads2[l];           //1D array that have the row_index needed to be sent to threads
      MATRIX_A=A;
      MATRIX_B=B;
      MATRIX_C=C;
      column_1=m;
      column_2=n;
      for(int i=0; i<l; i++)
    {
            
      	     arr2threads2[i]=i;
             pthread_create(&threads[i],NULL,calc_row_elements,(void*) &arr2threads2[i]);    
      	     //This creates a new thread and sends to it every time only 1 attribute to perform the element multiplication
      	     //attribute needed to be sent are row_index (i) 
      	     
    }
    
    for(int i=0; i<l; i++)
     {
      
      pthread_join(threads[i],NULL);
             
     } 
      C=MATRIX_C;
}

