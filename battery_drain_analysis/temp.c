//////////////////////////////
// Battery Drain Analysis   //
// Author: Benjamin T. Fine //
// Created: 02/17/2017		//
//////////////////////////////

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>

#include "elisa3-lib.h"
#include "terminal.h"
#include "input.h"

#define NUM_ROBOTS 10

//for error handling
extern int errno;

//robots unigue id numbers
int robot_address[NUM_ROBOTS];

//data storage received from robot
unsigned int robot_prox[NUM_ROBOTS][8];
unsigned int robot_ground[NUM_ROBOTS][4];

////data sent to robot
//char robot_l_speed = 0, robot_r_speed = 0; // default to zero (no motion)
//unsigned char robot_red_led = 0, robot_green_led = 0, robot_blue_led = 0; // default to off
//
//// various
//unsigned int robotId=0;
//unsigned int delayCounter=0;
//char current_speed=0;
//unsigned char exitProg=0;

int main(int argc, char *argv[])
{
	//check for cmd line input
	//	op_code: the experiment type
	//	robot list: txt file of number of robots and list of robot ids
	if (argc != 3)
	{
		printf("USAGE: sudo ./main [op_code] [robot list]");
		return 0;
	}

	//try to open the robot list file
	FILE * r_ids;
   	int err_num;
   	r_ids = fopen(argv[3], "rb");

   	if (r_ids == NULL)
		{
		err_num = errno;
      	fprintf(stderr, "Error opening file: %s\n", strerror(err_num));

		return 0;
   	}
//
//	//read in the robot ids
//    int i;
//    for(i = 0; i < NUM_ROBOTS; i++)
//        scanf("%d", &robotAddress[i]);
//
//	//elisa library set up
//    initTerminal();
//
//    // init the communication with the robots; set the addresses and number of the robots to control
//    startCommunication(robotAddress, NUM_ROBOTS);
//
//    while(!exitProg)
//	{
//		handleKeyboardInput();
//
//        // set new data for the current robot controlled through the keyboard
//        setLeftSpeed(robotAddress[robotId], robLSpeed);
//        setRightSpeed(robotAddress[robotId], robRSpeed);
//        setRed(robotAddress[robotId], robRedLed);
//        setBlue(robotAddress[robotId], robBlueLed);
//        setGreen(robotAddress[robotId], robGreenLed);
//
//        // update sensors data for all 4 robots
//        getAllProximityFromAll(robProx);
//        getAllGroundFromAll(robGround);
//
//        printRobotInfo();
//
//    }
//
//    stopCommunication();
//	closeTerminal();
//
	return 0;

}

//full drain basic

//full drain full

//full drain motion task
