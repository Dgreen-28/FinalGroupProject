// GROUP D
// By Corey Green
// decoreyon.green@okstate.edu
// CS 4323
// finalGroupProject
// 3-23-22

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>

#define TOTALMUTEX 4

/*
INPUTS:
# of medical pros.
# of total patients
# patient capaity
# sofa space
# max enter time interval
# patient checkup time
*/

struct summary
{
    int successfulCheckups;
    int medicalProAvgWaitTime;
    int patientsThatLeft;
    int patientsAvgWaitTime;
};
struct threadStruct
{
    char *occupation;
    int id;
    int threadID;
};

void *patientThreadFunc();
void *staffThreadFunc();
int enterWaitingRoom();
void sitOnSofa();
void getMedicalCheckup();
void makePayment();
void leaveClinic();
void waitForPatients();
void performMedicalCheckup();
void acceptPayment();

int successfulCheckups;
int avgStaffWaitTime;
int patientsLeft;
int avgPatientsWaitTime;
sem_t mutex[TOTALMUTEX];
int totalRoomCapacity;
int totalSofaCapacity;
int checkupTime;

// allow command line args
int main(int argc, char *argv[])
{
    srand(time(NULL));
    int medicalStaff, totalPatients, roomCapacity, sofaSpace, maxTimeInterval;

    for (int i = 0; i < TOTALMUTEX; i++)
        sem_init(&mutex[i], 0, 1);

    // assigns the arguments to ints
    medicalStaff = atoi(argv[1]);
    totalPatients = atoi(argv[2]);
    roomCapacity = atoi(argv[3]);
    sofaSpace = atoi(argv[4]);
    maxTimeInterval = atoi(argv[5]);
    checkupTime = atoi(argv[6]);

    totalRoomCapacity = roomCapacity;
    totalSofaCapacity = sofaSpace;

    // initializes threads for staff & patients
    pthread_t patientThread[totalPatients];
    pthread_t staffThread[medicalStaff];

    struct threadStruct contents[totalPatients];
    struct threadStruct contentsM[medicalStaff];

    int ms = (rand() % maxTimeInterval) + 1;
    // for loops for the creation and joining of patient and staff threads
    // TODO: put these into functions with int args.
    for (int j = 0; j < medicalStaff; j++)
    {
        contentsM[j].id = j;
        pthread_create(&staffThread[j], NULL, &staffThreadFunc, (void *)&contentsM[j]);
    }
    for (int i = 0; i < totalPatients; i++)
    {

        contents[i].occupation = "Patient";
        contents[i].id = i;

        usleep(ms * 1000);
        pthread_create(&patientThread[i], NULL, &patientThreadFunc, (void *)&contents[i]);
    }
    for (int j = 0; j < medicalStaff; j++)
    {
        pthread_join(staffThread[j], NULL);
    }
    for (int i = 0; i < totalPatients; i++)
    {
        pthread_join(patientThread[i], NULL);
    }

    // printf("medicalstaff = %d, totalPatients = %d, roomCapacity = %d, sofaSpace = %d, maxTimeInterval = %d, checkupTime = %d", medicalStaff, totalPatients, roomCapacity, sofaSpace, maxTimeInterval, checkupTime);

    return 0;
}

void *staffThreadFunc(void *vargp)
{
    struct threadStruct *contentsM = vargp;
    contentsM->threadID = pthread_self;
    waitForPatients(vargp);

    performMedicalCheckup();
    acceptPayment();
    return NULL;
}
void *patientThreadFunc(void *vargp)
{
    struct threadStruct *contents = vargp;
    contents->threadID = pthread_self();

    printf("Patient %d (Thread ID: %d) Arrived to clinic\n", contents->id, contents->threadID);

    if (enterWaitingRoom())
    {
        sitOnSofa(vargp);
        getMedicalCheckup(vargp);
        makePayment();
        leaveClinic(contents);
    }
    else
    {
        printf("Patient %d (Thread ID: %d): Leaving without checkup.\n", contents->id, contents->threadID);
    }
    return NULL;
}

// MARK: funcs used by patients
int enterWaitingRoom()
{
    sem_wait(&mutex[0]);

    if (totalRoomCapacity > 0)
    {
        totalRoomCapacity--;
        printf("-----Current total capacity: %d\n", totalRoomCapacity);
        sem_post(&mutex[0]);
        return 1;
    }
    else
    {
        sem_post(&mutex[0]);
        return 0;
    }
}
void sitOnSofa(struct threadStruct *contents)
{
    while (1)
    {
        sem_wait(&mutex[2]);

        if (totalSofaCapacity > 0)
        {
            totalSofaCapacity--;
            printf("Patient %d (Thread ID: %d): Sitting on a sofa in the waiting room\n", contents->id, contents->threadID);
            printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
            sem_post(&mutex[2]);
            break;
        }
        else
        {
            printf("Patient %d (ThreadID: %d): Standing in the waiting room\n", contents->id, contents->threadID);
            printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
        }
        sem_post(&mutex[2]);
    }

    // TODO: setup
}
void getMedicalCheckup(struct threadStruct *contents)
{
    sem_wait(&mutex[2]);
    totalSofaCapacity++;
    printf("Patient %d (ThreadID: %d): Getting checkup\n", contents->id, contents->threadID);
    printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
    usleep(checkupTime * 1000);
    sem_post(&mutex[2]);

    // TODO: setup
}
void makePayment()
{
    // TODO: setup
}
void leaveClinic(struct threadStruct *contents)
{
    sem_wait(&mutex[1]);
    totalRoomCapacity++;
    sem_post(&mutex[1]);
    printf("Patient %d (ThreadID: %d): Leaving the clinic after receiving checkup\n", contents->id, contents->threadID);
    // TODO: setup
}

// MARK: funcs used by staff
void waitForPatients(struct threadStruct *contentsM)
{
    printf("Medical Professional %d (Thread ID: %d): Waiting for patient \n", contentsM->id, contentsM->threadID);
    // TODO: setup
}
void performMedicalCheckup()
{
    // TODO: setup
}
void acceptPayment()
{
    // TODO: setup
}