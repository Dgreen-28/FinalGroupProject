#include "Corey.h"
#include "Kyle.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>

void patientThreadFunc(struct threadStruct *contents)
{
    contents->threadID = (int)gettid();

    printf("Patient %d (Thread ID: %d) Arrived to clinic\n", contents->id, contents->threadID);
    
    
    pthread_mutex_lock(&mutex[0]);
    if (totalRoomCapacity < 1) {
        leaveClinic(contents, 0);
        pthread_mutex_unlock(&mutex[0]);
        return;
    }
    contents->waitTime = clock();
    enterWaitingRoom();
    pthread_mutex_unlock(&mutex[0]);
    
    
    pthread_mutex_lock(&mutex[1]);

    if (totalSofaCapacity <= 0) {
        printf("Patient %d (ThreadID: %d): Standing in the waiting room\n", contents->id, contents->threadID);
    }
    pthread_mutex_unlock(&mutex[1]);
    
    while(1){
        pthread_mutex_lock(&mutex[1]);
        if(totalSofaCapacity > 0) {
            sitOnSofa(contents);
            pthread_mutex_unlock(&mutex[1]);
            break;
        }
        pthread_mutex_unlock(&mutex[1]);
    }
    
    sem_post(&count[0]);
    sem_wait(&count[1]);
    getMedicalCheckup(contents);

    makePayment(contents);
    leaveClinic(contents, 1);
}

void staffThreadFunc(struct threadStruct *contents)
{
    contents->threadID = (int)gettid();
    while(1){
        contents->waitTime = clock(); //start----
        pthread_mutex_lock(&mutex[1]);
        if (totalSofaCapacity == maxSofaCapacity) waitForPatients(contents);
        pthread_mutex_unlock(&mutex[1]);
        sem_post(&count[1]);
        sem_wait(&count[0]);
        if (left >= maxPatients) return;
        contents->waitTime = clock() - contents->waitTime; //end----
        pthread_mutex_lock(&mutex[10]);
        summary.medicalProAvgWaitTime += contents->waitTime;
        pthread_mutex_unlock(&mutex[10]);
        performMedicalCheckup(contents);
        acceptPayment(contents);
    }
}

void enterWaitingRoom()
{
    totalRoomCapacity--;
}

void sitOnSofa(struct threadStruct *contents)
{
    totalSofaCapacity--;
    printf("Patient %d (Thread ID: %d): Sitting on a sofa in the waiting room\n", contents->id, contents->threadID);
}

void queueTask(struct task task){
    pthread_mutex_lock(&mutex[11]);
    queue[remainingTasks++] = task;
    pthread_mutex_unlock(&mutex[11]);
}

struct task dequeue(){
    struct task task = queue[0];
    for (int i = 0; i < remainingTasks-1; i++){
        queue[i] = queue[i+1];
    }
    remainingTasks--;
    return task;
}

void* mainThreadLoop(void* args){
    struct task task;
    int flag = 1;
    while(flag){
        pthread_mutex_lock(&mutex[11]);
        if (remainingTasks < 1){
            pthread_mutex_unlock(&mutex[11]);
            continue;
        }
        task = dequeue();
        
        pthread_mutex_unlock(&mutex[11]);
        switch(task.selector){
            case 0:
                staffThreadFunc(task.args);
                break;
            case 1:
                patientThreadFunc(task.args);
                break;
            case 2:
                flag = 0;
                break;
        }
    }
}