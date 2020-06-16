#include "terminal.h"
#include "input.h"
#include <stdio.h>
#ifdef _WIN32
    #include "windows.h"
#endif
#if defined(__linux__) || defined(__APPLE__)
	#include <ncurses.h>
#endif

extern unsigned int robProx[4][8];
extern unsigned int robGround[4][4];
extern char current_speed;
extern char robLSpeed, robRSpeed;
extern char robRedLed, robGreenLed, robBlueLed;
extern int robotAddress[4];
extern unsigned int robotId;

#ifdef _WIN32
void curPos(int x, int y) {
  HANDLE hStdout;
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  hStdout=GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hStdout, &csbiInfo);
  csbiInfo.dwCursorPosition.X=x;
  csbiInfo.dwCursorPosition.Y=y;
  SetConsoleCursorPosition(hStdout, csbiInfo.dwCursorPosition);
}
#endif

void printHelp() {

#ifdef _WIN32
    system( "cls" );
#endif

#if defined(__linux__) || defined(__APPLE__)
	clear();
#endif

    while(1) {

#ifdef _WIN32
        curPos(0,0);
#endif

#if defined(__linux__) || defined(__APPLE__)
		refresh();
#endif

        printf("************\n\r");
        printf("*** HELP ***\n\r");
        printf("************\n\r");
        printf("\n\rCURRENT ROBOT: 0 to 3 (the current robot controlled through keyboard)\n\r");
        printf("MOVEMENT: when no key is pressed the speed sent to robot is 0\n\r");
        printf("+: increase speed used to control the robot\n\r");
        printf("-: decrease speed used to control the robot\n\r");
        printf("Arrow up: both motors forward\n\r");
        printf("Arrow down: both motors backward\n\r");
        printf("Arrow left: motor right forward\n\r");
        printf("Arrow right: motor left forward\n\r");
        printf("a: pivot counterclockwise (motor right forward, motor left backward)\n\r");
        printf("s: pivot clockwise (motor right backward, motor left forward)\n\r");
        printf("\n\rRGB COLOR:\n\r");
        printf("r: red power increase (max 100)\n\r");
        printf("e: red power decrease (min 0)\n\r");
        printf("g: green power increase (max 100)\n\r");
        printf("f: green power decrease (min 0)\n\r");
        printf("b: blue power increase (max 100)\n\r");
        printf("v: blue power decrease (min 0)\n\r");
        printf("\n\rFLAGS:\n\r");
        printf("k: calibrate all sensors (proximity, ground, accelerometer)\n\r");
        printf("\n\rOTHERS:\n\r");
        printf("spacebar: reset\n\r");
        printf("h: enter/exit help\n\r");
        printf("q: quit\n\r");

        if(HKeyPressed()) {
            break;
        }

    }

#ifdef _WIN32
    system( "cls" );
#endif

#if defined(__linux__) || defined(__APPLE__)
		refresh();
#endif

}

void initTerminal() {
#ifdef _WIN32
    system("mode 75, 60");
#endif

#if defined(__linux__) || defined(__APPLE__)
    initscr();
	keypad(stdscr,1);
    cbreak();
    noecho();
    timeout(100); // non-blocking getch()
#endif

}

void closeTerminal() {
#if defined(__linux__) || defined(__APPLE__)
	endwin();
#endif
}

void printRobotInfo() {

#ifdef _WIN32
        curPos(0,0);
#endif

#if defined(__linux__) || defined(__APPLE__)
		refresh();
#endif

        printf("************************\n\r");
        printf("*** RECEPTION ROBOT0 ***\t\tPress h for help\n\r");
        printf("************************\n\r");
        printf("PROXIMITY\r\n");
        printf("Prox0\t Prox1\t Prox2\t Prox3\t Prox4\t Prox5\t Prox6\t Prox7\r\n");
        printf("%4d\t %4d\t %4d\t %4d\t %4d\t %4d\t %4d\t %4d\t\n\r", robProx[0][0], robProx[0][1], robProx[0][2], robProx[0][3], robProx[0][4], robProx[0][5], robProx[0][6], robProx[0][7]);
        printf("GROUND\r\n");
        printf("ground0\t ground1\t ground2\t ground3\r\n");
        printf("%4d\t %4d\t\t %4d\t\t %4d\t\n\r", robGround[0][0], robGround[0][1], robGround[0][2], robGround[0][3]);
        printf("************************\n\r");
        printf("*** RECEPTION ROBOT1 ***\n\r");
        printf("************************\n\r");
        printf("PROXIMITY\r\n");
        printf("Prox0\t Prox1\t Prox2\t Prox3\t Prox4\t Prox5\t Prox6\t Prox7\r\n");
        printf("%4d\t %4d\t %4d\t %4d\t %4d\t %4d\t %4d\t %4d\t\n\r", robProx[1][0], robProx[1][1], robProx[1][2], robProx[1][3], robProx[1][4], robProx[1][5], robProx[1][6], robProx[1][7]);
        printf("GROUND\r\n");
        printf("ground0\t ground1\t ground2\t ground3\r\n");
        printf("%4d\t %4d\t\t %4d\t\t %4d\t\n\r", robGround[1][0], robGround[1][1], robGround[1][2], robGround[1][3]);
        printf("************************\n\r");
        printf("*** RECEPTION ROBOT2 ***\n\r");
        printf("************************\n\r");
        printf("PROXIMITY\r\n");
        printf("Prox0\t Prox1\t Prox2\t Prox3\t Prox4\t Prox5\t Prox6\t Prox7\r\n");
        printf("%4d\t %4d\t %4d\t %4d\t %4d\t %4d\t %4d\t %4d\t\n\r", robProx[2][0], robProx[2][1], robProx[2][2], robProx[2][3], robProx[2][4], robProx[2][5], robProx[2][6], robProx[2][7]);
        printf("GROUND\r\n");
        printf("ground0\t ground1\t ground2\t ground3\r\n");
        printf("%4d\t %4d\t\t %4d\t\t %4d\t\n\r", robGround[2][0], robGround[2][1], robGround[2][2], robGround[2][3]);
        printf("************************\n\r");
        printf("*** RECEPTION ROBOT3 ***\n\r");
        printf("************************\n\r");
        printf("PROXIMITY\r\n");
        printf("Prox0\t Prox1\t Prox2\t Prox3\t Prox4\t Prox5\t Prox6\t Prox7\r\n");
        printf("%4d\t %4d\t %4d\t %4d\t %4d\t %4d\t %4d\t %4d\t\n\r", robProx[3][0], robProx[3][1], robProx[3][2], robProx[3][3], robProx[3][4], robProx[3][5], robProx[3][6], robProx[3][7]);
        printf("GROUND\r\n");
        printf("ground0\t ground1\t ground2\t ground3\r\n");
        printf("%4d\t %4d\t\t %4d\t\t %4d\t\n\r", robGround[3][0], robGround[3][1], robGround[3][2], robGround[3][3]);
        printf("****************************\n\r");
        printf("*** TRANSFER robot <- pc ***\n\r");
        printf("****************************\n\r");
        printf("CURRENT ROBOT: id=%d, addr=%d\n\r", robotId, robotAddress[robotId]);
		printf("SPEED\r\n");
		printf("speed to send: %+3d\t\r\n", current_speed);
		printf("left: %+3d\t right: %+3d\t\n\r", robLSpeed, robRSpeed);
		printf("RGB LEDS: r: %3d\t g: %3d\t b: %3d\t\n\r", robRedLed, robGreenLed, robBlueLed);

}

