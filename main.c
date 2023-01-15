#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>



typedef struct {
    int ** arr;
    int size;
    sem_t * lock;
    sem_t * exit;
    
} ArrayArgs;





void * BubbleSort(void * args) {
    ArrayArgs  arg = *(ArrayArgs*)args;
    int* array = (int*)malloc(sizeof(int)*arg.size);
    for(int i = 0;i<arg.size;i++) {
        array[i]  = (*arg.arr)[i];
    }
    int counter = 0;
    for(int i = 0;i<arg.size;i++) {
        for(int j = 0;j<arg.size-i-1;j++) {
            if(array[j+1]<array[j]) {
                int temp = array[j+1];
                array[j+1] = array[j];
                array[j] = temp;
                counter++;
            }
        }
        if(counter==0) {
            break;
        } 
        counter = 0;
    }
    
    sem_wait(arg.lock);

    free(*arg.arr);
    (*arg.arr) = array;
    printf("the work of bubble sort\n");
    
    sem_post(arg.exit);
    return NULL;
}

void *  InsertionSort(void * args) {
    ArrayArgs  arg = *(ArrayArgs*)args;
    int *array = (int*)malloc(sizeof(int)*arg.size);
    for(int i = 0;i<arg.size;i++) {
        array[i]  = (*arg.arr)[i];
    }



    for(int i = 1;i<arg.size;i++) {
        int key = array[i];
        int j = i-1;
        while(j>=0&&key<array[j]) {
            array[j+1] = array[j];
            j = j -1;

        }
        array[j+1] = key;
    }
  
    sem_wait(arg.lock);
    free(*arg.arr);
    (*arg.arr) = array;
    printf("the work of insertion sort\n");
    sem_post(arg.exit);
    return NULL;


}

int findMin(int * arr,int startindex,int size) {
    int min = startindex;
    for(int i = startindex;i<size;i++) {
        if(arr[i]<arr[min]) {
            min = i;
        }
    }
    return min;
}

void * SelectionSort(void * args) {
    ArrayArgs arg = *(ArrayArgs*)args;
    
    int *array = (int*)malloc(sizeof(int)*arg.size);
    for(int i = 0;i<arg.size;i++) {
        array[i]  = (*arg.arr)[i];
    }

    int ind = 0;
    for(int i = 0;i<arg.size;i++) {
        int min = findMin(array,i,arg.size);
        int temp = array[ind];
        array[ind] = array[min];
        array[min] = temp;
        ind++;

    }
    sem_wait(arg.lock);
    free(*arg.arr);
    (*arg.arr) = array;
    printf("the work of selection sort\n");
    sem_post(arg.exit);
    return NULL;


}



int main() {
   
    pthread_t insertion;
    pthread_t bubble;
    pthread_t selection;
    ArrayArgs x;
    int * arr = (int*)malloc(sizeof(int)*5);
    arr[0] = 6;
    arr[1] = 7;
    arr[2] = 8;
    arr[3] = 9;
    arr[4] = 10;
    x.arr = &arr;
    x.size = 5;
    x.lock = (sem_t*)malloc(sizeof(sem_t));
    x.exit = (sem_t*)malloc(sizeof(sem_t));
    printf("Elements before : ");
    for(int i = 0;i<5;i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
    sem_init(x.lock,0,1);
    sem_init(x.exit,0,0);


    
    pthread_create(&insertion,NULL,InsertionSort,&x);
    pthread_create(&bubble,NULL,BubbleSort,&x);
    pthread_create(&selection,NULL,SelectionSort,&x);

    

    sem_wait(x.exit);
    pthread_cancel(insertion);
    pthread_cancel(bubble);
    pthread_cancel(selection);
    pthread_join(insertion,NULL);
    pthread_join(bubble,NULL);
    pthread_join(selection,NULL);

    printf("Elements after : ");


    for(int i = 0;i<5;i++) {
        printf("%d ",arr[i]);
    }


    free(x.lock);
    free(x.exit);
    free(arr);

    


    




    pthread_exit(NULL);

    

    return 0;
}