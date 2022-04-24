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

void patientThreadFunc(struct threadStruct*);
void staffThreadFunc(struct threadStruct*);
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
sem_t count[TOTALCOUNT];
int totalRoomCapacity;
int totalSofaCapacity;
int checkupTime;

//--------------------------------------------------------------------------


typedef struct Task {
    void (*taskFunction)(struct threadStruct*);
    struct threadStruct* args;
} Task;

Task queue[256];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void queueTask(Task task){
    pthread_mutex_lock(&mutexQueue);
    queue[taskCount++] = task;
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
}

void* startThread(void* args){
    Task task;
    
    
    while(1){
        pthread_mutex_lock(&mutexQueue);
        while(taskCount < 1){
            pthread_cond_wait(&condQueue, &mutexQueue);
        }
        task = queue[0];
        for (int i = 0; i < taskCount-1; i++){
            queue[i] = queue[i+1];
        }
        taskCount--;
        
        pthread_mutex_unlock(&mutexQueue);
        
        task.taskFunction(task.args);
    }
}





//-----------------------------------------------------------------------

// allow command line args
int main(int argc, char *argv[])
{
    pthread_cond_init(&condQueue, NULL);
    pthread_mutex_init(&mutexQueue, NULL);
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
    totalSofaCapacity = sofaSpace;
    
    // Initialze Semaphores
    for (int i = 0; i < TOTALMUTEX; i++)
        sem_init(&mutex[i], 0, 1);
        
    sem_init(&count[0], 0, 0);
    sem_init(&count[1], 0, 0);
    // initializes threads for staff & patients
    //pthread_t patientThread[totalPatients];
    //pthread_t staffThread[medicalStaff];
    
    pthread_t threads[medicalStaff+totalPatients];

    struct threadStruct contents[totalPatients];
    struct threadStruct contentsM[medicalStaff];
    /*

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
    */

    // printf("medicalstaff = %d, totalPatients = %d, roomCapacity = %d, sofaSpace = %d, maxTimeInterval = %d, checkupTime = %d", medicalStaff, totalPatients, roomCapacity, sofaSpace, maxTimeInterval, checkupTime);
    for (int i = 0; i < medicalStaff+totalPatients; i++){
        if (pthread_create(&threads[i], NULL, &startThread, NULL)){
            perror("Failed to create");
        }
    }
    
    for (int i = 0; i < medicalStaff; i++){
        contentsM[i].id = i;
        contentsM[i].occupation = "Staff";
        Task t = {
            .taskFunction = &staffThreadFunc,
            .args = &contentsM[i]
        };
        queueTask(t);
    }
    
    for (int i = 0; i < totalPatients; i++){
        int ms = (rand() % maxTimeInterval) + 1;
        contents[i].id = i;
        contents[i].occupation = "Patient";
        Task t = {
            .taskFunction = &patientThreadFunc,
            .args = &contents[i]
        };
        queueTask(t);
        usleep(ms * 1000);
    }
    
    for (int i = 0; i < medicalStaff+totalPatients; i++){
        if (!pthread_join(threads[i], NULL)){
            perror("Failed to join");
        }
    }
    
    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);
    
    return 0;
}

void staffThreadFunc(struct threadStruct *contents)
{
    contents->threadID = (int)gettid();
    while(1){
        waitForPatients(contents);

        performMedicalCheckup();
        acceptPayment();
    }
    //return NULL;
}
void patientThreadFunc(struct threadStruct *contents)
{
    contents->threadID = (int)gettid();

    printf("Patient %d (Thread ID: %d) Arrived to clinic\n", contents->id, contents->threadID);

    if (enterWaitingRoom())
    {
        sitOnSofa(contents);
        getMedicalCheckup(contents);
        makePayment();
        leaveClinic(contents);
    }
    else
    {
        printf("Patient %d (Thread ID: %d): Leaving without checkup.\n", contents->id, contents->threadID);
    }
    //return NULL;
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
    sem_wait(&mutex[1]);

    if (totalSofaCapacity <= 0)
    {
        printf("Patient %d (ThreadID: %d): Standing in the waiting room\n", contents->id, contents->threadID);
        printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
    }
    sem_post(&mutex[1]);
    
    while(1){
        sem_wait(&mutex[1]);
        if(totalSofaCapacity > 0){
            totalSofaCapacity--;
            printf("Patient %d (Thread ID: %d): Sitting on a sofa in the waiting room\n", contents->id, contents->threadID);
            printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
            sem_post(&mutex[1]);
            break;
        }
        sem_post(&mutex[1]);
    }

    // TODO: setup
}
void getMedicalCheckup(struct threadStruct *contents)
{
    /*
    sem_wait(&mutex[3]);
    totalSofaCapacity++;
    printf("Patient %d (ThreadID: %d): Getting checkup\n", contents->id, contents->threadID);
    printf("-----Current sofa capacity: %d\n", totalSofaCapacity);
    usleep(checkupTime * 1000);
    sem_post(&mutex[3]);
    */
    sem_post(&count[0]);
    sem_wait(&count[1]);
    sem_wait(&mutex[1]);
    totalSofaCapacity++;
    totalRoomCapacity++;
    printf("Patient %d (ThreadID: %d): Getting checkup\n", contents->id, contents->threadID);
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
void leaveClinic(struct threadStruct *contents)
{
    sem_wait(&mutex[0]);
    totalRoomCapacity++;
    sem_post(&mutex[0]);
    printf("Patient %d (ThreadID: %d): Leaving the clinic after receiving checkup\n", contents->id, contents->threadID);
    // TODO: setup
}

// MARK: funcs used by staff
void waitForPatients(struct threadStruct *contents)
{
    printf("Medical Professional %d (Thread ID: %d): Waiting for patient \n", contents->id, contents->threadID);
    // TODO: setup
}
void performMedicalCheckup()
{
    sem_post(&count[1]);
    sem_wait(&count[0]);
    usleep(checkupTime*1000);
    // TODO: setup
}
void acceptPayment()
{
    // TODO: setup
}