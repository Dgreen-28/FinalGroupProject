#include "Robby.h"
#include "Kyle.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
// func for leaving clinic
void leaveClinic(struct threadStruct *contents, int isSuccessful)
{
    // mutex for keeping up with patients who left
    pthread_mutex_unlock(&mutex[9]);
    pthread_mutex_lock(&mutex[9]);
    left++;
    pthread_mutex_unlock(&mutex[9]);
    if (isSuccessful){
        printf("Patient %d (ThreadID: %d): Leaving the clinic after receiving checkup\n", contents->id, contents->threadID);
        // mutex for getting the paitent's wait time
        contents->waitTime = clock() - contents->waitTime;
        pthread_mutex_lock(&mutex[10]);
        summary.patientsAvgWaitTime += contents->waitTime;
        pthread_mutex_unlock(&mutex[10]);
                // mutex for incrementing patients who got successful checkups
        pthread_mutex_lock(&mutex[10]);
        summary.successfulCheckups++;
        pthread_mutex_unlock(&mutex[10]);
    } else {
        printf("Patient %d (Thread ID: %d): Leaving without checkup.\n", contents->id, contents->threadID);
        // mutex for incrementing patients who got left without checkups
        pthread_mutex_lock(&mutex[10]);
        summary.patientsThatLeft++;
        pthread_mutex_unlock(&mutex[10]);
    }
}
// func for medical checkup behavior
void getMedicalCheckup(struct threadStruct *contents)
{
    int staffId;
    // mutex for incrementing sofa capacity and announcing who's being checked up
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
    // to ensure summary prints last
    usleep(checkupTime * 1000);

}
// func for making payment
void makePayment(struct threadStruct *contents)
{
    // mutex to insure one patient pays at a time
    pthread_mutex_lock(&mutex[8]);
    printf("Patient %d (ThreadID: %d): Making payment to Medical Staff %d\n", contents->id, contents->threadID, contents->bondId);
    pthread_mutex_unlock(&mutex[2]);
    pthread_mutex_lock(&mutex[6]);
    pthread_mutex_unlock(&mutex[8]);
}
