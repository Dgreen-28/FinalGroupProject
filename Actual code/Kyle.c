#include "Robby.h"
#include "Corey.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>

// MARK: funcs used by staff
void waitForPatients(struct threadStruct *contents)
{
    printf("Medical Professional %d (Thread ID: %d): Waiting for patient \n", contents->id, contents->threadID);
}
void performMedicalCheckup(struct threadStruct *contents)
{
    pthread_mutex_lock(&mutex[4]);
    
    contents->bondId = buffer;
    buffer = contents->id;
    
    pthread_mutex_unlock(&mutex[5]);
    
    printf("Medical Professional %d (Thread ID: %d): Checking patient %d\n", contents->id, contents->threadID, contents->bondId);
    
}
void acceptPayment(struct threadStruct *contents)
{
    pthread_mutex_lock(&mutex[7]);
    pthread_mutex_lock(&mutex[2]);
    printf("Medical Professional %d (Thread ID: %d): Accepted payment from Patient %d\n", contents->id, contents->threadID, contents->bondId);
    pthread_mutex_unlock(&mutex[6]);
    pthread_mutex_unlock(&mutex[7]);
    
}
