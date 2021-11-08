#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#define N 100000 
#define M 25000

void createNumbers(float* array);
void* calculateSum(void* args);

struct args {
	float* array;
	int* start;
	float* sums;
	int* index;
} ;
typedef struct args Args;

int main(int argc,char **argv){

	float array[N];
	int starts[4];
	float sums[4];
	int indexes[4];
	Args arguments[4];
	pthread_t threads[4];
	createNumbers(array);
	
	int start=0;
	for(int i=0;i<4;i++){
		
		starts[i]=start;
		indexes[i]=i;

		arguments[i].array=array;
		arguments[i].start=&starts[i];
		arguments[i].sums=sums;
		arguments[i].index=&indexes[i];

		if(pthread_create(&threads[i],NULL,&calculateSum,(void* )&arguments[i])!=0){
			printf("Error creating thread.");
			exit(1);
		}

		start=start+M;
	}

	for(int i=0;i<4;i++){
		if(pthread_join(threads[i],NULL)!=0){
			printf("Error joining thread.");
			exit(1);
		}
	}

	float totalSum=0;
	for(int i=0;i<4;i++){
		printf("\nsum=%f\n",sums[i]);
		totalSum=totalSum+sums[i];
	}
	printf("\ntotalSum=%f\n",totalSum);

	return 0;
}

void createNumbers(float* array){

	// The block of this function creates 100000 real numbers from 0 to 5 and saves them in an array created in main.

	srand(228524);
	float a=5.0;
	for (int i=0;i<N;i++){
		array[i]=(float)rand()/(float)(RAND_MAX)*a; //create random real number from 0 to 5 and save it in the array.
	}
	/*
	printf("[");
	for(int i=0;i<N;i++){
		if(i!=N-1){
			printf("%f,",array[i]);
		}
		else{
			printf("%f",array[i]);
		}
	}
	printf("]\n");
	*/
}

void* calculateSum(void* args){

	// The block of this function is the block each thread executes and calculates the sum from 25000 numbers out of 10000. Four threads in total execute this block.

	Args* arguments=(Args*) args; //In arguments pointer is saved the address of one out of four structs in array arguments (created in main). There are four structs created (one for each thread) and in each one is saved the appropriate information each thread has to use in order to calculate the sum of 25000 numbers out of 100000 and save this sum in the appropriate address in memory. This information is the start point in the initial array (its address is saved in start pointer) and the index pointer where is saved the address of the correct index of the array sums. The correct index is the one where the thread has to save its sum (the sum of 25000 numbers) in the array sums. (The array sums is the one where the 4 sums are saved in order to be calculated the total sum from the main thread) 

	float sum=0;
	int start=*arguments->start;
	int end=start+M;

	for(int i=start;i<end;i++){
		sum=sum+( arguments->array[i] );
	}
	
	int index=*arguments->index;
	//printf("\nindex=%d\n",index);
	arguments->sums[index]=sum; // With the help of index variable we calculate here the address where the sum will be saved. This sum is the sum calculated from one out of four threads.
	//printf("\npointer=%p\n",&arguments->sums[index]);
	//printf("sum=%f\n",arguments->sums[index]);

}

