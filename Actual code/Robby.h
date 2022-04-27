#ifndef ROBBY_H
#define ROBBY_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include "Robby.h"
#include "Corey.h"

int successfulCheckups;
int avgStaffWaitTime;
int patientsLeft;
int avgPatientsWaitTime;
pthread_mutex_t mutex[12];
sem_t count[2];
int totalRoomCapacity;
int totalSofaCapacity;
int maxSofaCapacity;
int checkupTime;
int left;
int buffer;
int maxPatients;
struct summary summary;

struct threadStruct
{
    char *occupation;
    int id;
    int threadID;
    int bondId;
    clock_t waitTime;
};

void patientThreadFunc(struct threadStruct*);
void staffThreadFunc(struct threadStruct*);
void enterWaitingRoom();
void sitOnSofa();
void* mainThreadLoop(void* args);
#endif