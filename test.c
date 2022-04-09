
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

#define TOTALMUTEX 3

/*
INPUTS:
# of medical pros.
# of total patients
# patient capaity
# sofa space
# max enter time interval 
# patient checkup time
*/

struct threadStruct {
    char* occupation;
    int id;
    int threadID;
};

void *patientThreadFunc();
void *staffThreadFunc();
int enterWaitingRoom();
void sitOnSofa();
void getMedicalCheckup();
void makePayment();
void leaveClinic(struct threadStruct* contents);
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

// allow command line args
int main(int argc, char *argv[])
{
    int medicalStaff, totalPatients, roomCapacity, sofaSpace, maxTimeInterval, checkupTime;
    
    for (int i = 0; i < TOTALMUTEX; i++) sem_init(&mutex[i], 0, 1);

        //assigns the arguments to ints
    	medicalStaff = atoi(argv[1]);
        totalPatients = atoi(argv[2]);
    	roomCapacity = atoi(argv[3]);
        sofaSpace = atoi(argv[4]);
	    maxTimeInterval = atoi(argv[5]);
	    checkupTime = atoi(argv[6]);

        totalRoomCapacity = roomCapacity;
        totalSofaCapacity = sofaSpace;

        //initializes threads for staff & patients
        pthread_t patientThread[totalPatients];
        pthread_t staffThread[medicalStaff];


        struct threadStruct contents[totalPatients];


        //for loops for the creation and joining of patient and staff threads
        //TODO: put these into functions with int args.
        for (int j = 0; j < medicalStaff; j++)
        {
            pthread_create(&staffThread[j], NULL, &staffThreadFunc, NULL);
            printf("Medical Professional %d (Thread ID: %ld): Waiting for patient \n", j, (long)staffThread[j]);
        }
        for (int i = 0; i < totalPatients; i++)
        {
            
            contents[i].occupation = "Patient";
            contents[i].id = i;
            pthread_create(&patientThread[i], NULL, &patientThreadFunc, (void*)&contents[i]);
        }
        for (int j = 0; j < medicalStaff; j++)
        {
            pthread_join(staffThread[j], NULL);
        }
        for (int i = 0; i < totalPatients; i++)
        {
            pthread_join(patientThread[i], NULL);
        }
    

//printf("medicalstaff = %d, totalPatients = %d, roomCapacity = %d, sofaSpace = %d, maxTimeInterval = %d, checkupTime = %d", medicalStaff, totalPatients, roomCapacity, sofaSpace, maxTimeInterval, checkupTime);
 
    return 0;
}

void *staffThreadFunc(void *vargp)
{
    waitForPatients();
    performMedicalCheckup();
    acceptPayment();
        return NULL;
}
void *patientThreadFunc(void *vargp)
{
    struct threadStruct* contents = vargp;
    contents->threadID = (int)gettid();
    printf("Patient %d (Thread ID: %d) Arrived to clinic\n", contents->id, contents->threadID);
    
    
    if(enterWaitingRoom()){
        sitOnSofa();
        getMedicalCheckup();
        makePayment();
        leaveClinic(contents);
    } else {
        printf("Leaving without checkup.\n");
    }
        return NULL;
}

//MARK: funcs used by used by patients
int enterWaitingRoom()
{
    sem_wait(&mutex[0]);

    if (totalRoomCapacity > 0)
    {
        totalRoomCapacity--;
        printf("Current capacity: %d\n", totalRoomCapacity);
        sem_post(&mutex[0]);
        return 1;
    }
    else
    {
        sem_post(&mutex[0]);
        return 0;
    }
}
void sitOnSofa()
{
    while(1){
        sem_wait(&mutex[2]);

        if (totalSofaCapacity > 0)
        {
            totalSofaCapacity--;
            printf("Current sofa capacity: %d\n", totalSofaCapacity);
            sem_post(&mutex[2]);
            break;
        }
        sem_post(&mutex[2]);
    }
    
    //TODO: setup
}
void getMedicalCheckup()
{
    sem_wait(&mutex[2]);
    totalSofaCapacity++;
    printf("Current sofa capacity: %d\n", totalSofaCapacity);
    sem_post(&mutex[2]);
    printf("Getting checkup\n");
    sleep(1);
    
    //TODO: setup
}
void makePayment()
{
    //TODO: setup
}
void leaveClinic(struct threadStruct *contents)
{
    sem_wait(&mutex[1]);
    totalRoomCapacity++;
    sem_post(&mutex[1]);
    printf("Patient %d (ThreadID: %d): Leaving the clinic after receiving checkup\n", contents->id, contents->threadID);
    //TODO: setup
}

//MARK: funcs used by used by staff
void waitForPatients()
{
    //TODO: setup
}
void performMedicalCheckup()
{
    //TODO: setup
}
void acceptPayment()
{
    //TODO: setup
}