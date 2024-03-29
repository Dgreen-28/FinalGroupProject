// Kyle McCullough
// Group D
// kymcculk@okstate.edu
// 4/26/2022

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

/**
 *   Prints out that medical professional # is waiting for a patient
 *   struct threadStruct *contents is a pointer to the contents struct holding Medical Professional information.
 */
void waitForPatients();

/**
 *   Syncs up the performance of the medical checkup from a medical professional with the getting of the medical check up of the patient.
 *   struct threadStruct *contents is a pointer to the contents struct holding Medical Professional information.
 */
void performMedicalCheckup();

/**
 *   Only allows one thread to accept payment from the patients. Prints out that the medical professional # accepts payment from patient #.
 *   struct threadStruct *contents is a pointer to the contents struct holding Medical Professional information.
 */
void acceptPayment();


#endif