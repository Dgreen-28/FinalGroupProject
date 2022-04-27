#ifndef KYLE_H
#define KYLE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include "Robby.h"
#include "Corey.h"

void waitForPatients();
void performMedicalCheckup();
void acceptPayment();
struct task {
    int selector;
    struct threadStruct* args;
};

struct task queue[256];
int remainingTasks;
void queueTask(struct task task);
struct task dequeue();
#endif