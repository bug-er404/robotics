#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include "elisa3-lib.h"
#include <stdbool.h>
#ifdef _WIN32
    #include "windows.h"
#endif

#define GROUND_WHITE_VARIANCE 2 //Hardcoded and handtuned (B.Fine)
#define GROUND_BLACK_VARIANCE 2 //Hardcoded and handtuned (B.Fine)
#define OBSTACLE_THR 50

#define MAX_SPEED 30
#define MIN_SPEED 5
#define TOO_CLOSE 200 

#define NUM_ROBOTS 1 
#define CALIBRATION_GROUND_READ_COUNT 50

#define MANUAL_CALIBRATION
#define DISABLE_MOTORS

void calibrate_move_to_level_0(unsigned int calibration_ground[][NUM_ROBOTS][4], unsigned int
robGround[][4], unsigned int robotAddress[])
{
	printf("Start::Move to Level 0\n\r");
	
	size_t done_count = 0, i;
	while (done_count < NUM_ROBOTS)
	{
		done_count = 0;
		transferData();
		getAllGroundFromAll(robGround);
		
		for (i = 0; i < NUM_ROBOTS; i++)
		{

			//check if the robot is on the black testing strip
			if (robGround[i][0] > calibration_ground[0][i][0]-GROUND_WHITE_VARIANCE &&
				robGround[i][1] > calibration_ground[0][i][1]-GROUND_WHITE_VARIANCE &&
			   	robGround[i][2] > calibration_ground[0][i][2]-GROUND_WHITE_VARIANCE &&	
			   	robGround[i][3] > calibration_ground[0][i][3]-GROUND_WHITE_VARIANCE)
			{
        		setLeftSpeed(robotAddress[i], 0);
        		setRightSpeed(robotAddress[i], 0);
				done_count++;
			}
			else
			{
        		setLeftSpeed(robotAddress[i], 5);
        		setRightSpeed(robotAddress[i], 5);
			}
		}
	}

}

void calibrate_move_to_non_level_0(unsigned int calibration_ground[][NUM_ROBOTS][4], unsigned int
robGround[][4], unsigned int robotAddress[])
{
	printf("Start::Move to Non-Level 0\n\r");
	
	size_t done_count = 0, i;
	while (done_count < NUM_ROBOTS)
	{
		done_count = 0;
		transferData();
		getAllGroundFromAll(robGround);
		
		for (i = 0; i < NUM_ROBOTS; i++)
		{

			//check if the robot is on the black testing strip
			if (robGround[i][0] < calibration_ground[0][i][0]-GROUND_WHITE_VARIANCE &&
				robGround[i][1] < calibration_ground[0][i][1]-GROUND_WHITE_VARIANCE &&
			   	robGround[i][2] < calibration_ground[0][i][2]-GROUND_WHITE_VARIANCE &&	
			   	robGround[i][3] < calibration_ground[0][i][3]-GROUND_WHITE_VARIANCE)
			{
        		setLeftSpeed(robotAddress[i], 0);
        		setRightSpeed(robotAddress[i], 0);
				done_count++;
			}
			else
			{
        		setLeftSpeed(robotAddress[i], 5);
        		setRightSpeed(robotAddress[i], 5);
			}
		}
	}

}

void calibrate_level(unsigned int lvl, unsigned int calibration_ground[][NUM_ROBOTS][4], unsigned int
robGround[][4], unsigned int robotAddress[])
{
	//calibrate the ground sensors for all robots on white
	printf("Start::Calibration Level %1u\n\r", lvl);
	
	size_t i,j;
	for (i = 0; i < CALIBRATION_GROUND_READ_COUNT; i++)
	{
		transferData();
        getAllGroundFromAll(robGround);
		for (j = 0; j < NUM_ROBOTS; j++)
		{
			calibration_ground[lvl][j][0] += robGround[j][0];
			calibration_ground[lvl][j][1] += robGround[j][1];
			calibration_ground[lvl][j][2] += robGround[j][2];
			calibration_ground[lvl][j][3] += robGround[j][3];
		}
	}
	for (i = 0; i < NUM_ROBOTS; i++)
	{
		calibration_ground[lvl][i][0] /= CALIBRATION_GROUND_READ_COUNT;
		calibration_ground[lvl][i][1] /= CALIBRATION_GROUND_READ_COUNT;
		calibration_ground[lvl][i][2] /= CALIBRATION_GROUND_READ_COUNT;
		calibration_ground[lvl][i][3] /= CALIBRATION_GROUND_READ_COUNT;
		printf("\tr%4d::CALIBRATION LEVEL 0 [%4u, %4u, %4u, %4u]\n\r", robotAddress[i],
		calibration_ground[lvl][i][0],calibration_ground[lvl][i][1],
		calibration_ground[lvl][i][2],calibration_ground[lvl][i][3]);
	}


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

//0 - white
//1 - black
void avoidGroundObstacles(unsigned int *ground, char *left, char *right, unsigned int
c[][NUM_ROBOTS][4], unsigned int i) 
{
	//char max_speed = 50;
	char min_speed = 5;
	char motor_step = 3;

	int v;
	double scale;

	//push the values into the calibration range
	ground[0] = (ground[0] > c[0][i][0]) ? c[0][i][0] : ground[0];
	ground[0] = (ground[0] < c[1][i][0]) ? c[1][i][0] : ground[0];
	ground[3] = (ground[3] > c[0][i][3]) ? c[0][i][3] : ground[3];
	ground[3] = (ground[3] < c[1][i][3]) ? c[1][i][3] : ground[3];

	//scale between 0 and 60
	scale = (double)(MAX_SPEED - 0)/(double)(c[0][i][0] - c[1][i][0]);
	
	//left
	v = (ground[0]-c[1][i][0])*scale+0;
	//printf("\t%4f::[%4u, %4d]", scale, ground[0], v); 
	//*left = max_speed-v;
	*left = v;

	//right
	scale = (double)(MAX_SPEED - 0)/(double)(c[0][i][3] - c[1][i][3]);
	v = (ground[3]-c[1][i][3])*scale+0;
	//printf("\t%4f::[%4u, %4d]\n\r",scale, ground[3], v); 
	//*right = max_speed-v;	
	*right = v;	

	*right = (MAX_SPEED/2) - (*right);     // set the speed to the motors
    *left = (MAX_SPEED/2) - (*left);

	if ((*right < 0 && *left < 0) || (*right > 0 && *left > 0))
	{
		//not turning so reverse speed
		*right *= -1;
		*left *= -1;
	}

	//no speed slower than min_speed
	*right = (*right > -1*min_speed && *right <= 0) ? -1*min_speed : *right;
	*right = (*right < min_speed && *right >= 0) ? min_speed : *right;
	*left = (*left > -1*min_speed&& *left <= 0) ? -1*min_speed : *left;
	*left = (*left < min_speed && *left >= 0) ? min_speed : *left;

	//if the values are too close then set both to slowest value
	// this is to help reduce noise drift
	if (abs(*right-*left) < motor_step)
	{
		// check if positive
		if (*right > 0)
		{	
			*right = (*right <= *left) ? *right : *left;
			*left = (*right <= *left) ? *right : *left;
		}
		else
		{
			*right = (*right >= *left) ? *right : *left;
			*left = (*right >= *left) ? *right : *left;
		}
	}
}

void simple_flock(unsigned int *prox, char *left, char *right, bool * f) 
{
	//int min_dist = 20;
	//int max_dist = 150;
	*f = 0;
	//int avoidance_flip = 100;
	
	//char max_speed = 50;
	
	//int v;
	int side_reduction_scale = 3;

    int rightProxSum = 0, leftProxSum = 0;	// sum of proximity values on right or left

    // using the 5 front proximity sensors (scale the sides?)
    rightProxSum = prox[0]/2 + prox[1] + (prox[2]/side_reduction_scale);
    leftProxSum = prox[0]/2 + prox[7] + (prox[6]/side_reduction_scale);
	
	if (rightProxSum > TOO_CLOSE || leftProxSum > TOO_CLOSE)
		*f = 1;

    rightProxSum /= 5;                 // scale the sum to have a moderate impact on the velocity
    leftProxSum /= 5;
    
	//make sure we cap the speeds
	rightProxSum = (rightProxSum > MAX_SPEED) ? MAX_SPEED : rightProxSum;
	leftProxSum = (leftProxSum > MAX_SPEED) ? MAX_SPEED : leftProxSum;

	//*right = (max_speed/2)-leftProxSum;     // set the speed to the motors
    //*left = (max_speed/2)-rightProxSum;
	*right = leftProxSum - (MAX_SPEED/2);     // set the speed to the motors
    *left = rightProxSum - (MAX_SPEED/2);

	//check if we are too close
	if (*f)
	{
		*right *= -1;
		*left *= -1;
	}

	//no influce from agents
	if (*right <= 0 && *left <= 0)
	{
		*right = 0;
		*left = 0;
	}
}

void avoidObstacles(unsigned int *prox, char *left, char *right) 
{
    int rightProxSum = 0, leftProxSum = 0;	// sum of proximity values on right or left

    // obstacle avoidance using the 3 front proximity sensors
    rightProxSum = prox[0]/2 + prox[1];
    leftProxSum = prox[0]/2 + prox[7];

    rightProxSum /= 5;                 // scale the sum to have a moderate impact on the velocity
    leftProxSum /= 5;
    
	if(rightProxSum > 60) 
	{             // velocity of the motors set to be from -30 to +30
        rightProxSum = 60;
    }
    if(leftProxSum > 60) 
	{
        leftProxSum = 60;
    }
    
	*right = 30-leftProxSum;     // set the speed to the motors
    *left = 30-rightProxSum;

}

int main(void) 
{
    unsigned int temp = 0;
    int robotAddress[NUM_ROBOTS];

	// received prox from all robots
    unsigned int robProx[NUM_ROBOTS][8];
	
	// received ground from all robots
    unsigned int robGround[NUM_ROBOTS][4];
    
	// sent to robot
    char robot_speed_l, robot_speed_r, object_speed_l, object_speed_r, flock_speed_l, flock_speed_r;
    char robRedLed, robGreenLed, robBlueLed;

    srand(time(NULL));
    
	unsigned int i = 0;
    for(i = 0; i < NUM_ROBOTS; i++) 
	{
        printf("\r\nInsert the robot address %d: ", i);
        temp = scanf("%d", &robotAddress[i]);
    }

    // init the communication with the robots; set the addresses and number of the robots to control
    startCommunication(robotAddress, NUM_ROBOTS);
	
	for (i = 0; i < 50; transferData(), i++);
	calibrateSensorsForAll();
	for (i = 0; i < 10; transferData(), i++);

#ifdef MANUAL_CALIBRATION	
	unsigned int calibration_ground[2][NUM_ROBOTS][4] = {};

	calibrate_level(0, calibration_ground, robGround, robotAddress);

	printf("Enter 'c' to Continue\n\r");  

	char c = getchar();
	while (getchar() == '\n');
	
	calibrate_level(1, calibration_ground, robGround, robotAddress);

	printf("Enter 'c' to Continue\n\r");  

	c = getchar();
	while (getchar() == '\n');
#endif	

/*	for (i = 1; i < 5; i++)
	{
		//Move straight until not on white for all sensors
		calibrate_move_to_non_level_0(calibration_ground, robGround, robotAddress);
		//calibrate the ground sensors for all robots on black
		calibrate_level(i, calibration_ground, robGround, robotAddress);	
		//Move straight until on white for all sensors
		calibrate_move_to_level_0(calibration_ground, robGround, robotAddress);
	}*/
	
	/*for (i = 0; i < NUM_ROBOTS; i++)
	{
    	setLeftSpeed(robotAddress[i], 0);
        setRightSpeed(robotAddress[i], 0);
	}*/
	
	for (i = 0; i < 10; transferData(), i++);
	
	printf("END CALIBRATION\n\r\n\r");

	bool flip;
    while(1) 
	{

        transferData();
		getAllGroundFromAll(robGround);
        getAllProximityFromAll(robProx);

        for(i = 0; i < NUM_ROBOTS; i++) 
		{
	   
       		int x = getAccX(robotAddress[i]); 
       		int y = getAccY(robotAddress[i]); 
       		int z = getAccZ(robotAddress[i]); 
	  
			printf("r%4d::[%4d, %4d, %4d]\n\r", robotAddress[i], x, y, x);

	   		avoidGroundObstacles(robGround[i], &object_speed_l, &object_speed_r, calibration_ground, i);
        	
			//7 left 0 ahead 1 right
			//printf("r%4d::[%4u, %4u, %4u, %4u, %4u]\n\r\n\r", robotAddress[i], robProx[i][6], robProx[i][7], robProx[i][0], robProx[i][1], robProx[i][2]);
			simple_flock(robProx[i], &flock_speed_l, &flock_speed_r, &flip); 

			//simple average to combine speeds for start if flock is not 0
			if (flock_speed_l != 0 && flock_speed_r != 0)
			{
				robot_speed_l = (object_speed_l+flock_speed_l)/2;
				robot_speed_r = (object_speed_r+flock_speed_r)/2;
			}
			else if (flip)
			{
				robot_speed_l = flock_speed_l;
				robot_speed_r = flock_speed_r;
			}
			else
			{
				robot_speed_l = object_speed_l;
				robot_speed_r = object_speed_r;
			}
		
			//set to min speed
			robot_speed_r = (robot_speed_r > -1*MIN_SPEED && robot_speed_r <= 0) ? -1*MIN_SPEED : robot_speed_r;
			robot_speed_r = (robot_speed_r < MIN_SPEED && robot_speed_r >= 0) ? MIN_SPEED :robot_speed_r;
			robot_speed_l = (robot_speed_l> -1*MIN_SPEED && robot_speed_l <= 0) ? -1*MIN_SPEED:robot_speed_l;
			robot_speed_l = (robot_speed_l < MIN_SPEED && robot_speed_l >= 0) ? MIN_SPEED:robot_speed_l;
		
		#ifndef DISABLE_MOTORS			
			setLeftSpeed(robotAddress[i], robot_speed_l);
        	setRightSpeed(robotAddress[i], robot_speed_r);
		#endif
			//printf("r%4d::[%2d, %2d]+[%2d, %2d]=[%2d, %2d](%1d)\n\r\n\r",
			//robotAddress[i],object_speed_l, object_speed_r, flock_speed_l, flock_speed_r,
			//robot_speed_l, robot_speed_r, flip);
        
			//sleep(1);
		
		}

        /*if(updateRGB(&robRedLed, &robGreenLed, &robBlueLed)) 
		{
            for(k=0; k<NUM_ROBOTS; k++) 
			{
                setRed(robotAddress[k], robRedLed);
                setGreen(robotAddress[k], robGreenLed);
                setBlue(robotAddress[k], robBlueLed);
            }
        }*/
    }

	stopTransferData();
    stopCommunication();

	return 0;
}
