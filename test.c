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
#define TOTALCOUNT 2

struct summary
{
    int successfulCheckups;
    int medicalProAvgWaitTime;
    int patientsThatLeft;
    int patientsAvgWaitTime;
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

int totalRoomCapacity;
int totalSofaCapacity;
int initialRoomCapacity;
int checkupTime;
int patientID;
int medicalID;

sem_t mutex[TOTALMUTEX];
sem_t count[TOTALCOUNT];

struct patient
{
    int sofaWaitingTime;
    int standingWaitingTime;
    int id;
    int threadID;
};

struct medicalProfessional
{
    int id;
    int threadID;
};

// allow command line args
int main(int argc, char *argv[])
{
    srand(time(NULL));
    int medicalStaff, totalPatients, roomCapacity, sofaSpace, maxTimeInterval;

    // assigns the arguments to ints
    medicalStaff = atoi(argv[1]);
    totalPatients = atoi(argv[2]);
    roomCapacity = atoi(argv[3]);
    sofaSpace = atoi(argv[4]);
    maxTimeInterval = atoi(argv[5]);
    checkupTime = atoi(argv[6]);

    totalRoomCapacity = roomCapacity;
    initialRoomCapacity = roomCapacity;
    totalSofaCapacity = sofaSpace;

    // Initialze Semaphores
    for (int i = 0; i < TOTALMUTEX; i++)
        sem_init(&mutex[i], 0, 1);

    sem_init(&count[0], 0, 0);
    sem_init(&count[1], 0, 0);

    //  initializes threads for staff & patients
    pthread_t patientThread[totalPatients];
    pthread_t staffThread[medicalStaff];

    struct patient patients[totalPatients];
    struct medicalProfessional medicalProfs[medicalStaff];

    int ms = (rand() % maxTimeInterval) + 1;
    // for loops for the creation and joining of patient and staff threads
    // TODO: put these into functions with int args.
    for (int j = 0; j < medicalStaff; j++)
    {
        medicalProfs[j].id = j;
        pthread_create(&staffThread[j], NULL, &staffThreadFunc, (void *)&medicalProfs[j]);
    }
    for (int i = 0; i < totalPatients; i++)
    {
        patients[i].id = i;

        usleep(ms * 1000);
        pthread_create(&patientThread[i], NULL, &patientThreadFunc, (void *)&patients[i]);
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
    struct medicalProfessional *medicalProfs = vargp;
    medicalProfs->threadID = (int)gettid();
    waitForPatients(vargp);
    performMedicalCheckup(vargp);
    acceptPayment();
    return NULL;
}
void *patientThreadFunc(void *vargp)
{
    struct patient *patients = vargp;
    patients->threadID = (int)gettid();

    printf("Patient %d (Thread ID: %d) Arrived to clinic\n", patients->id, patients->threadID);

    if (enterWaitingRoom())
    {
        sitOnSofa(vargp);
        getMedicalCheckup(vargp);
        makePayment();
        leaveClinic(patients);
    }
    else
    {
        printf("Patient %d (Thread ID: %d): Leaving without checkup.\n", patients->id, patients->threadID);
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
void sitOnSofa(struct patient *patients)
{
    sem_wait(&mutex[1]);

    if (totalSofaCapacity <= 0)
    {
        printf("Patient %d (ThreadID: %d): Standing in the waiting room\n", patients->id, patients->threadID);
        printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
    }
    sem_post(&mutex[1]);

    while (1)
    {
        sem_wait(&mutex[1]);
        if (totalSofaCapacity > 0)
        {
            totalSofaCapacity--;
            printf("Patient %d (Thread ID: %d): Sitting on a sofa in the waiting room\n", patients->id, patients->threadID);
            printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
            sem_post(&mutex[1]);
            break;
        }
        sem_post(&mutex[1]);
    }

    // TODO: setup
}
void getMedicalCheckup(struct patient *patients)
{
    // sem_wait(&mutex[3]);
    // totalSofaCapacity++;
    // printf("Patient %d (ThreadID: %d): Getting checkup\n", patients->id, patients->threadID);
    // printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
    // usleep(checkupTime * 1000);
    // sem_post(&mutex[3]);

    sem_post(&count[0]);
    sem_wait(&count[1]);
    sem_wait(&mutex[1]);
    patientID = patients->id;
    totalSofaCapacity++;
    totalRoomCapacity++;
    printf("Patient %d (ThreadID: %d): Getting checkup\n", patients->id, patients->threadID);
    printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
    printf("-----Current room capacity: %d\n", totalRoomCapacity);
    sem_post(&mutex[1]);
    usleep(checkupTime * 1000);

    // TODO: setup
}
void makePayment()
{
    // TODO: setup
}
void leaveClinic(struct patient *patients)
{
    sem_wait(&mutex[0]);
    totalRoomCapacity++;
    sem_post(&mutex[0]);
    printf("Patient %d (ThreadID: %d): Leaving the clinic after receiving checkup\n", patients->id, patients->threadID);
    // TODO: setup
}

// MARK: funcs used by staff
void waitForPatients(struct medicalProfessional *medicalProfs)
{

    // if (initialRoomCapacity == totalRoomCapacity)
    // {
    //     printf("Medical Professional %d (Thread ID: %d): Waiting for patient \n", medicalProfs->id, medicalProfs->threadID);
    // }
    printf("Medical Professional %d (Thread ID: %d): Waiting for patient \n", medicalProfs->id, medicalProfs->threadID);

    // TODO: setup
}
void performMedicalCheckup(struct medicalProfessional *medicalProfs)
{
    sem_post(&count[1]);
    sem_wait(&count[0]);
    medicalID = medicalProfs->id;
    printf("Medical Professional %d (Thread ID: %d): Checking Patient %d \n", medicalProfs->id, medicalProfs->threadID, patientID);
    usleep(checkupTime * 1000);
    // TODO: setup
}
void acceptPayment()
{
    // TODO: setup
}