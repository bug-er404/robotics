
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

//robots unigue id numbers
int robotAddress[NUM_ROBOTS];

//data storage received from robot
unsigned int robProx[NUM_ROBOTS][8];
unsigned int robGround[NUM_ROBOTS][4];

//unsigned int robBattLevel = 0;
//unsigned int robBattPercent = 0;

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

int main(int argc, char *argv[])
{
	robotAddress[0] = 3569;
	robotAddress[1] = 3535;
	robotAddress[2] = 3479;
	robotAddress[3] = 3536;
	robotAddress[4] = 3502;
	robotAddress[5] = 3453;
	robotAddress[6] = 3447;
	robotAddress[7] = 3607;
	robotAddress[8] = 3572;
	robotAddress[9] = 3529;

	initTerminal();

  unsigned int raw[NUM_ROBOTS] = {0};
  unsigned int percent[NUM_ROBOTS] = {0};

    // init the communication with the robots; set the addresses and number of the robots to control
    startCommunication(robotAddress, NUM_ROBOTS);

    while(!exitProg){
      refresh();

      for (int i = 0; i < NUM_ROBOTS; i++){

            raw[i] = getBatteryAdc(robotAddress[i]);
            percent[i] = getBatteryPercent(robotAddress[i]);
      }

      for (int i = 0; i < NUM_ROBOTS; i++){
          printf("Robot: %d \n\rPercent: (~%3d%%)\n\rRaw: %4d\n\r", robotAddress[i], percent[i], raw[i]);

      }
      printf("--------------------------------------\n\r");
      sleep(1);
    }

//   stopCommunication();
    closeTerminal();

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
	//set all robots to active (1)
	for (i = 0; i < NUM_ROBOTS;/*active[i] = 1,*/ i++){

	//open a battery file for each robot
	// for (i = 0; i < NUM_ROBOTS; i++)
	// {
	// 	char file[80];
	// 	sprintf(file, "%s_r%d.txt",trial_name,robotAddress[i]);
	// 	battery_files[i] = fopen(file, "w");
	// }

	//loop while atleast one robot's charge is above the percentage threshold
	//while (robots_active > 0)
	//int j = 0;
	//while (j < 20)
	//{
	//j++;
	//	for (i = 0; i < NUM_ROBOTS; i++)
	//	{
	//		if (active[i])
	//		{
				raw = getBatteryAdc(robotAddress[i]);
				percent = getBatteryPercent(robotAddress[i]);

	//			if (percent <= threshold)
	//			{
	//				robots_active--;
	//				active[i] = 0;
	//			}
				sleep(1);
				//fprintf(battery_files[i],"Percent: %2d Raw: %3d\n",percent,raw);
				printf("i: %d Percent: (~%3d%%)\n\rRaw: %4d\n\r", robotAddress[i], percent, raw);
	//		}
	//		else
	//			printf("%d NULL\n",i);
				//fprintf(battery_files[i],"Percent: -42 Raw: -42\n");

			//reducing the data
	//		}
	}

	//close the battery files
//	for (i = 0; i < NUM_ROBOTS; fclose(battery_files[i]),i++);

//	printf("\n\nLeaving drain()\n\n");
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
