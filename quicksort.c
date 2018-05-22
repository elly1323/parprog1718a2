#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#define MESSAGES 20
#define numOfThreads 4
#define N 1000
#define CUTOFF 10



struct minima{
	int startOfArray;
	int finishOfArray;
};
typedef struct minima minima;

struct oura{
	minima array[MESSAGES];
	int start;
	int finish;
};
typedef struct oura oura;

void Queue_Init(oura *q)
{
	q->start = 0;
	q->finish = 0;
}

int Queue_Enqueue(oura *q, minima x)
{	
		q->array[q->finish]=x;
		return 0;
}

void inssort(double *a,int n) {
int i,j;
double t;
  
  for (i=1;i<n;i++) {
    j = i;
    while ((j>0) && (a[j-1]>a[j])) {
      t = a[j-1];  a[j-1] = a[j];  a[j] = t;
      j--;
    }
  }

}


void quicksort(double *a,int n) {
	
int first,last,middle;
double t,p;
int i,j;

  // check if below cutoff limit
  if (n<=CUTOFF) {
    inssort(a,n);
    minima sorted;
    sorted.startOfArray = 0;
    sorted.finishOfArray = n-1;
    printf("Sort on %d and %d", sorted.startOfArray, sorted.finishOfArray);
    return;
  }
  
  // take first, last and middle positions
  first = 0;
  middle = n-1;
  last = n/2;  
  
  // put median-of-3 in the middle
  if (a[middle]<a[first]) { t = a[middle]; a[middle] = a[first]; a[first] = t; }
  if (a[last]<a[middle]) { t = a[last]; a[last] = a[middle]; a[middle] = t; }
  if (a[middle]<a[first]) { t = a[middle]; a[middle] = a[first]; a[first] = t; }
    
  // partition (first and last are already in correct half)
  p = a[middle]; // pivot
  for (i=1,j=n-2;;i++,j--) {
    while (a[i]<p) i++;
    while (p<a[j]) j--;
    if (i>=j) break;

    t = a[i]; a[i] = a[j]; a[j] = t;      
  }
   
  // recursively sort halves
  quicksort(a,i);
  quicksort(a+i,n-i);
  
}

void *Thread_Function(void *args){
	quicksort(args, N);
}


// condition variable, signals a put operation (receiver waits on this)
pthread_cond_t msg_in = PTHREAD_COND_INITIALIZER;
// condition variable, signals a get operation (sender waits on this)
pthread_cond_t msg_out = PTHREAD_COND_INITIALIZER;

// mutex protecting common resources
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int main() {
  double *a;
  int i;
  oura *myQueue;
  //mallocing array
  a = (double *)malloc(N*sizeof(double));
  if (a==NULL) {
    printf("error in malloc\n");
    exit(1);
  }
  minima first;
  first.startOfArray = 0;
  first.finishOfArray = N;
  

  // fill array with random numbers
  srand(time(NULL));
  for (i=0;i<N;i++) {
    a[i] = (double)rand()/RAND_MAX;
  }
  
  //Creating the threadpool
  pthread_t threads[numOfThreads];
  int t;
  for (t=0; t<numOfThreads; t++){
    pthread_create(&(threads[t]), NULL, NULL, NULL);
  }
  
  return 0;
}







