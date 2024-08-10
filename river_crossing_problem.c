#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdbool.h>
#include <semaphore.h>

sem_t mutex;
sem_t hackerQueue;
sem_t serfQueue;
sem_t Barrier;

int hackers=0,serfs=0;

void * rowBoat(){
    sleep(1);
    printf("\nboat has started rowing\n\n");
}

void * Hacker(){
    while (1){
        bool isCaptain =false;
        sem_wait(&mutex);
        hackers++;
        if(hackers==4){
            sem_post(&hackerQueue); 
            sem_post(&hackerQueue); 
            sem_post(&hackerQueue); 
            sem_post(&hackerQueue);
            hackers-=4;
            isCaptain=true;
        }
        else if(hackers==2&&serfs>=2){
            sem_post(&hackerQueue); 
            sem_post(&hackerQueue); 
            sem_post(&serfQueue); 
            sem_post(&serfQueue);
            serfs-=2;
            hackers-=2;
            isCaptain=true;
        }
        else{
            sem_post(&mutex);
        }
        sem_wait(&hackerQueue);
        sem_wait(&Barrier);
        printf("hacker boarding\n");

        if(isCaptain){
            rowBoat();
            sem_post(&mutex);
        }
        sem_post(&Barrier);
    }
}

void * Serf(){
    while (1)
    {
        bool isCaptain =false;
        sem_wait(&mutex);
        serfs++;
        if(serfs==4){
            sem_post(&serfQueue); 
            sem_post(&serfQueue); 
            sem_post(&serfQueue); 
            sem_post(&serfQueue);
            serfs-=4;
            isCaptain=true;
        }
        else if(serfs==2&&hackers>=2){
            sem_post(&serfQueue); 
            sem_post(&serfQueue); 
            sem_post(&hackerQueue); 
            sem_post(&hackerQueue);
            serfs-=2;
            hackers-=2;
            isCaptain=true;
        }
        else{
            sem_post(&mutex);
        }
        sem_wait(&serfQueue);
        sem_wait(&Barrier);
        printf("serf boarding\n");
        if(isCaptain){
            rowBoat();
            sem_post(&mutex);
        }
        sem_post(&Barrier);
    }
}

int main(){
    sem_init(&mutex,0,1);
    sem_init(&hackerQueue,0,0);
    sem_init(&serfQueue,0,0);
    sem_init(&Barrier,0,4);
    
    pthread_t pid[8];
    for(int i=0;i<4;i++){
        pthread_create(&pid[i],NULL,Hacker,NULL);
        pthread_create(&pid[i+4],NULL,Serf,NULL);
    }
    for(int i=0;i<4;i++){
        pthread_join(pid[i],NULL);
        pthread_join(pid[i+4],NULL);
    }
}