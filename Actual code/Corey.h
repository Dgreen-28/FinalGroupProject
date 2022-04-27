#ifndef COREY_H
#define COREY_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include "Robby.h"
#include "Kyle.h"
// struct to keep up with summary values
struct summary
{   
    int successfulCheckups;
    long medicalProAvgWaitTime;
    int patientsThatLeft;
    long patientsAvgWaitTime;
};

void getMedicalCheckup();
void makePayment();
void leaveClinic();

#endif