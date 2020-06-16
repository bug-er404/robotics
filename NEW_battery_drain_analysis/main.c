#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include "elisa3-lib.h"
#include <stdlib.h>

#include <signal.h>

#ifdef _WIN32
    #include "windows.h"
#endif

#define OBSTACLE_THR 50
#define NUM_ROBOTS 8 //be a multiple of 4 (might be a library issue)

int robotAddress[NUM_ROBOTS];

unsigned int robProx[NUM_ROBOTS][8];
unsigned char exitProg=0;
	// received from robot

	// sent to robot
char robLSpeed[NUM_ROBOTS], robRSpeed[NUM_ROBOTS];
char robRedLed, robGreenLed, robBlueLed;

// sent to robot
// various
unsigned int delayCounter=1000;

//experiment types
void drain_min(int threshold, char* trial_name);
void drain_std(int threshold);
void drain_max(int threshold);

static volatile int keepRunning = 1;

void intHandler(int dummy) 
{
	    keepRunning = 0;
}

unsigned char updateRGB(char *red, char *green, char *blue) {
    static unsigned int i=0;
    unsigned int rndNum;

    i = (i+1)%65000;  // use to change the rgb leds
    if(i==0) {
        rndNum = rand()%400;
        if(rndNum < 100) {
            *red = rand()%100;
            *green = rand()%100;
            *blue = 0;
        } else if(rndNum < 200) {
            *red = rand()%100;
            *green = 0;
            *blue = rand()%100;
        } else if(rndNum < 300) {
            *red = 0;
            *green = rand()%100;
            *blue = rand()%100;
        } else {
            *red = rand()%100;
            *green = rand()%100;
            *blue = rand()%100;
        }
        return 1;
    }
    return 0;
}

void avoidObstacles(unsigned int *prox, char *left, char *right) {
    int rightProxSum=0, leftProxSum=0;	// sum of proximity values on right or left

    // obstacle avoidance using the 3 front proximity sensors
    rightProxSum = prox[0]/2 + prox[1];
    leftProxSum = prox[0]/2 + prox[7];

    rightProxSum /= 5;                 // scale the sum to have a moderate impact on the velocity
    leftProxSum /= 5;
    if(rightProxSum > 60) {             // velocity of the motors set to be from -30 to +30
        rightProxSum = 60;
    }
    if(leftProxSum > 60) {
        leftProxSum = 60;
    }
    *right = 30-leftProxSum;     // set the speed to the motors
    *left = 30-rightProxSum;

}

int main(int argc, char *argv[])
{
	//check for cmd line input
	//	op_code: the experiment type
	//	robot list: txt file of number of robots and list of robot ids
	if (argc != 5)
	{
		fprintf(stderr, "usage: sudo ./main [op_code] [threshold] [trial name] [robot list]\n");
		exit(EXIT_FAILURE);
	}

	srand(time(NULL));

	signal(SIGINT, intHandler);

    unsigned int k=0;
	//try to open the robot list file
	FILE * r_ids;
   	int err_num;
  	r_ids = fopen(argv[4], "r");

 	if (r_ids == NULL)
	{
      	//fprintf(stderr, "error: opening robot list - %s\n", strerror(errno));
      	printf("error: opening robot list");
		exit(EXIT_FAILURE);
   	}

	//input the robot ids
	int i, trash;
	for (i = 0; i < NUM_ROBOTS; i++)
	{
		trash = fscanf(r_ids, "%d", &robotAddress[i]);
	}
	fclose(r_ids);

//    for(k=0; k<NUM_ROBOTS; k++)
//    {
//        printf("\r\nInsert the robot address %d: ", k);
//        scanf("%d", &robotAddress[k]);
//    }

    // init the communication with the robots; set the addresses and number of the robots to control
    startCommunication(robotAddress, NUM_ROBOTS);
	
	for (i = 0; i < 50; transferData(), i++);

/*	switch (atoi(argv[1]))
	{
		case 0:
			drain_min(atoi(argv[2]), argv[3]);
			break;
		case 1:
			//drain_std(atoi(argv[2]));
			break;
		case 2:
			//drain_max(atoi(argv[2]));
			break;
		default:
      		fprintf(stderr, "error: invlaid op_code (%d)\n", atoi(argv[1]));
			exit(EXIT_FAILURE);
	}
*/
	//FILE* test = fopen("TEST.txt", "w");
  	FILE* battery_files[NUM_ROBOTS];
  	FILE* file = fopen("battery_drain_analysis_max_all_08042017_trial_01.txt", "w");
	unsigned int raw = 0, percent = 0;

	//open file for each robot and add header
/*  	for (i = 0; i < NUM_ROBOTS; i++)
	{
	 	char file[80];
	 	sprintf(file, "%s_r%d.txt",argv[3],robotAddress[i]);
	 	battery_files[i] = fopen(file, "w");
    	
		fprintf(battery_files[i],"Percent Raw\n");
	}
*/
	//turn on the green prox led so we know when batteries have depleated
///	for (i = 0; i < NUM_ROBOTS; i++)
 // 	{
   // 	turnOnSmallLeds(robotAddress[i]);
  //	}
    
	unsigned int robProx[NUM_ROBOTS][8];
    unsigned int robGround[NUM_ROBOTS][4];

	for (i = 0; i < NUM_ROBOTS; i++)
 	{
   		turnOnSmallLeds(robotAddress[i]);
		turnOnFrontIRs(robotAddress[i]);
		turnOnBackIR(robotAddress[i]);
  	}

	int j = 0;
	while(keepRunning)
	{
		//j++;
        transferData();
		getAllGroundFromAll(robGround);
        getAllProximityFromAll(robProx);
		getAllProximityAmbientFromAll(robProx);
		getAllGroundAmbientFromAll(robGround);
		
		//if(j > delayCounter)
		{
			//printf("HERE\n\n");
			for(k = 0; k < NUM_ROBOTS; k++)
			{
        		transferData();

				//get the battery data from the robots
				raw = getBatteryAdc(robotAddress[k]);
				percent = getBatteryPercent(robotAddress[k]);
				trash = getAccX(robotAddress[i]);
				trash = getAccY(robotAddress[i]);
				trash = getAccZ(robotAddress[i]);
				trash = (int) getSelector(robotAddress[i]);
				trash = getOdomTheta(robotAddress[i]);
				trash = getOdomXpos(robotAddress[i]);
				
				fprintf(file, "%4d %2d %3d\n", robotAddress[k], percent, raw);
		 		//fprintf(battery_files[k],"%2d %3d\n\r", percent, raw);
			}
			//j = 0;
		}
		sleep(3);
		#ifndef DISABLE_MOTORS			
			for(k = 0; k < NUM_ROBOTS; k++)
			{
				setLeftSpeed(robotAddress[k], 30);
        		setRightSpeed(robotAddress[k], -30);
			}
		#endif

      if(updateRGB(&robRedLed, &robGreenLed, &robBlueLed)) {
            for(k=0; k<NUM_ROBOTS; k++) {
                setRed(robotAddress[k], robRedLed);
                setGreen(robotAddress[k], robGreenLed);
                setBlue(robotAddress[k], robBlueLed);
            }
        }

    }

	fclose(file);
    stopCommunication();
	return 0;

}

void drain_min(int threshold, char* trial_name)
{
    FILE* battery_files[NUM_ROBOTS];
	FILE* t;

	int robots_active = NUM_ROBOTS;
	int active[NUM_ROBOTS];

	unsigned int raw = -1, percent = -1;

	int i;
	//a dead read
	for (i = 0; i < NUM_ROBOTS; raw = getBatteryAdc(robotAddress[i]),percent = getBatteryPercent(robotAddress[i]), i++);
		//sleep(3);
	//for (i = 0; i < NUM_ROBOTS; raw = getBatteryAdc(robotAddress[i]),percent = getBatteryPercent(robotAddress[i]), i++);

	//set all robots to active (1)
	for (i = 0; i < NUM_ROBOTS; active[i] = 1, i++);

	//open a battery file for each robot
	for (i = 0; i < NUM_ROBOTS; i++)
	{
	 	char file[80];
	 	sprintf(file, "%s_r%d.txt",trial_name,robotAddress[i]);
	 	battery_files[i] = fopen(file, "w");
	 }

	//loop while atleast one robot's charge is above the percentage threshold
	//int j = 0;
	//while (j < 10)
	while (robots_active > 0)
	{
	//	j++;
		for (i = 0; i < NUM_ROBOTS; i++)
		{
			if (active[i])
			{
				raw = getBatteryAdc(robotAddress[i]);
				percent = getBatteryPercent(robotAddress[i]);

				if (percent <= threshold)
				{
					robots_active--;
					active[i] = 0;
				}

			//	fprintf(battery_files[i],"Percent: %2d Raw: %3d\n",percent,raw);
			//	fprintf(test,"Percent: %2d Raw: %3d\n",percent,raw);
			}
			else
			{
			//	fprintf(battery_files[i],"Percent: -42 Raw: -42\n");
			}
		}
		//reducing the data
		//sleep(3);
		//printf("\n\r");
	}

	//close the battery files
	for (i = 0; i < NUM_ROBOTS; fclose(battery_files[i]), i++);

	//printf("\n\nLeaving drain()\n\n");
	return;
}

void drain_std(int threshold)
{
	return;
}

void drain_max(int threshold)
{
	return;
}
