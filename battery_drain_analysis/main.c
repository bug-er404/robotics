//////////////////////////////
// Battery Drain Analysis   //
// Author: Benjamin T. Fine //
// Created: 02/17/2017		//
//////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>

#include "elisa3-lib.h"
#include "terminal.h"
#include "input.h"
#include "trial_vars.h"

//#define NUM_ROBOTS 5

//robots unigue id numbers
int robotAddress[NUM_ROBOTS];

//data storage received from robot
unsigned int robProx[NUM_ROBOTS][8];
unsigned int robGround[NUM_ROBOTS][4];

unsigned int robBattLevel = 0;
unsigned int robBattPercent = 0;

////data sent to robot
char robLSpeed=0, robRSpeed=0;
unsigned char robRedLed=0, robGreenLed=0, robBlueLed=0;

// various
unsigned int robotId=0;
char current_speed=0;
unsigned char exitProg = 0; //no bool type in C

//experiment types
void drain_min(int threshold, char* trial_name);
void drain_std(int threshold);
void drain_max(int threshold);

//int main(int argc, char *argv[])
//{
//	robotAddress[0] = 3522;
//	robotAddress[1] = 3607;
//	robotAddress[2] = 3432;
//	robotAddress[3] = 3558;
//	robotAddress[4] = 3546;
//	robotAddress[5] = 3527;
//	robotAddress[6] = 3536;
//	robotAddress[7] = 3485;
//	robotAddress[8] = 3502;
//	robotAddress[9] = 3535;
//	robotAddress[10] = 3569;
//	robotAddress[11] = 3479;
//	robotAddress[12] = 3529;
//
//	initTerminal();
//
//    // init the communication with the robots; set the addresses and number of the robots to control
//    startCommunication(robotAddress, NUM_ROBOTS);
//
//    drain_min(atoi(argv[2]), argv[3]);
//
//		stopCommunication();
//    closeTerminal();
//
//  	return 0;
//
//}

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

	//try to open the robot list file
	FILE * r_ids;
   	int err_num;
  	r_ids = fopen(argv[4], "r");

 	if (r_ids == NULL)
	{
      	fprintf(stderr, "error: opening robot list - %s\n", strerror(errno));
		exit(EXIT_FAILURE);
   	}

	//input the robot ids
	int i, trash;
	for (i = 0; i < NUM_ROBOTS; i++)
	{
		trash = fscanf(r_ids, "%d", &robotAddress[i]);
	}
	fclose(r_ids);

//printf("%d\n", robot_address[0]);
//printf("%d\n", robot_address[1]);
//printf("%d\n", robot_address[2]);
//printf("%d\n", robot_address[3]);
//robot_address[0] = 3546;
//robot_address[1] = 3527;
//robot_address[2] = 3432;
//robot_address[3] = 3558;

	//elisa library set up
	initTerminal();

//printf("After initTerminal\n");
    //init the communication with the robots; set the addresses and number of the robots to control
    startCommunication(robotAddress, NUM_ROBOTS);

//printf("After startCommunication\n");

//printf("switch((%d)", atoi(argv[1]));
	switch (atoi(argv[1]))
	{
		case 0:
//printf("case 0 = %d\n", robot_address[0]);
			drain_min(atoi(argv[2]), argv[3]);
			break;
		case 1:
printf("case 1\n");
			//drain_std(atoi(argv[2]));
			break;
		case 2:
printf("case 2\n");
			//drain_max(atoi(argv[2]));
			break;
		default:
printf("case default\n");
      		fprintf(stderr, "error: invlaid op_code (%d)\n", atoi(argv[1]));
			exit(EXIT_FAILURE);
	}

	//elisa library shutdown
	stopCommunication();
	closeTerminal();

	exit(EXIT_SUCCESS);
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
	//for (i = 0; i < NUM_ROBOTS; raw = getBatteryAdc(robotAddress[i]),percent = getBatteryPercent(robotAddress[i]), i++);
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
	int j = 0;
	while (j < 10)
	//while (robots_active > 0)
	{
		j++;
		for (i = 0; i < NUM_ROBOTS; i++)
		{
			if (active[i])
			{
				//start prox test code -- to be REMOVED
				//getAllProximityFromAll(robProx);

				//int k,l;
				//for (k = 0; k < NUM_ROBOTS; k++)
				//{
				//	for (l = 0; l < 8; l++)
				//		printf("%d ", robProx[k][l]);
				//	printf("\n\r");
				//}
				//end -- to be REMOVED

				raw = getBatteryAdc(robotAddress[i]);
				percent = getBatteryPercent(robotAddress[i]);
	
printf("%d -> %2d < %d\n\r", i, percent, threshold);
				//if (percent <= threshold)
				//{
				//	robots_active--;
				//	active[i] = 0;
				//}

				fprintf(battery_files[i],"Percent: %2d Raw: %3d\n",percent,raw);
			}
			else
			{
				fprintf(battery_files[i],"Percent: -42 Raw: -42\n");
			}
		}
		//reducing the data
		sleep(3);
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
