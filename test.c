
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
    long threadID;
};

void *patientThreadFunc();
void *staffThreadFunc();
void enterWaitingRoom();
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
sem_t mutex;
int totalRoomCapacity;
int totalSofaCapacity;

// allow command line args
int main(int argc, char *argv[])
{
    int medicalStaff, totalPatients, roomCapacity, sofaSpace, maxTimeInterval, checkupTime;
    sem_init(&mutex, 0, 1);

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
            contents[i].threadID = (long)patientThread[i];
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
    printf("Occupation: %s\n", contents->occupation);
    printf("Patient %d (Thread ID: %ld) Arrived to clinic\n", contents->id, contents->threadID);
    enterWaitingRoom();
    sitOnSofa();
    getMedicalCheckup();
    makePayment();
    leaveClinic();
        return NULL;
}

//MARK: funcs used by used by patients
void enterWaitingRoom()
{
    
    sem_wait(&mutex);

    if (totalRoomCapacity > 0)
    {
        totalRoomCapacity--;
        sem_post(&mutex);
    }
    else
    {
        sem_post(&mutex);
        leaveClinic();
    }
}
void sitOnSofa()
{
    
    //TODO: setup
}
void getMedicalCheckup()
{
    //TODO: setup
}
void makePayment()
{
    //TODO: setup
}
void leaveClinic()
{
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