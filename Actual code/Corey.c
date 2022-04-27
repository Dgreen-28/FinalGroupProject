#include "Robby.h"
#include "Kyle.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
void leaveClinic(struct threadStruct *contents, int isSuccessful)
{
    pthread_mutex_lock(&mutex[9]);
    left++;
    pthread_mutex_unlock(&mutex[9]);
    if (isSuccessful){
        printf("Patient %d (ThreadID: %d): Leaving the clinic after receiving checkup\n", contents->id, contents->threadID);
        contents->waitTime = clock() - contents->waitTime;
        pthread_mutex_lock(&mutex[10]);
        summary.patientsAvgWaitTime += contents->waitTime;
        pthread_mutex_unlock(&mutex[10]);
        pthread_mutex_lock(&mutex[10]);
        summary.successfulCheckups++;
        pthread_mutex_unlock(&mutex[10]);
    } else {
        printf("Patient %d (Thread ID: %d): Leaving without checkup.\n", contents->id, contents->threadID);
        pthread_mutex_lock(&mutex[10]);
        summary.patientsThatLeft++;
        pthread_mutex_unlock(&mutex[10]);
    }
}
void getMedicalCheckup(struct threadStruct *contents)
{
    int staffId;
    
    pthread_mutex_lock(&mutex[1]);
    totalSofaCapacity++;
    pthread_mutex_unlock(&mutex[1]);
    pthread_mutex_lock(&mutex[0]);
    totalRoomCapacity++;
    printf("Patient %d (ThreadID: %d): Getting checkup\n", contents->id, contents->threadID);

    pthread_mutex_unlock(&mutex[0]);
    
    pthread_mutex_lock(&mutex[3]);
    
    buffer = contents->id;
    pthread_mutex_unlock(&mutex[4]);
    pthread_mutex_lock(&mutex[5]);
    
    contents->bondId = buffer;
    
    pthread_mutex_unlock(&mutex[3]);
    
    usleep(checkupTime * 1000);

}
void makePayment(struct threadStruct *contents)
{
    pthread_mutex_lock(&mutex[8]);
    printf("Patient %d (ThreadID: %d): Making payment to Medical Staff %d\n", contents->id, contents->threadID, contents->bondId);
    pthread_mutex_unlock(&mutex[2]);
    pthread_mutex_lock(&mutex[6]);
    pthread_mutex_unlock(&mutex[8]);
}
