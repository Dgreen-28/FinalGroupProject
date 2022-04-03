//Initial commit.
//Corey Green.
//3-23-22
#include <stdio.h>

/*
INPUTS:

# of medical pros.
# of total patients
# patient capaity
# sofa space
# max enter time interval 
# patient checkup time
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <pthread.h>

void *patientThread();


// allow command line args
int main(int argc, char *argv[])
{
    pthread_t thread1;

    int medicalStaff, totalPatients, roomCapacity, sofaSpace, maxTimeInterval, checkupTime;

    	medicalStaff = atoi(argv[1]);
        totalPatients = atoi(argv[2]);
    	roomCapacity = atoi(argv[3]);
        sofaSpace = atoi(argv[4]);
	    maxTimeInterval = atoi(argv[5]);
	    checkupTime = atoi(argv[6]);

        pthread_create(&thread1, NULL, &patientThread, NULL);
        pthread_join(thread1, NULL);

//printf("medicalstaff = %d, totalPatients = %d, roomCapacity = %d, sofaSpace = %d, maxTimeInterval = %d, checkupTime = %d", medicalStaff, totalPatients, roomCapacity, sofaSpace, maxTimeInterval, checkupTime);
 
    return 0;
}

void *patientThread(void *vargp)
{
    printf("hola");
    //TODO: enterWaitingRoom, 
    //TODO: sitOnSofa, 
    //TODO: getMedicalCheckup, 
    //TODO: makePayment, 
    //TODO: leaveClinic 
        return NULL;
}
