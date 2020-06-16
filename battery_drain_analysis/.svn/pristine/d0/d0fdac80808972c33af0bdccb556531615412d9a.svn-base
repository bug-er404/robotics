#include "input.h"
#include "terminal.h"
#ifdef _WIN32
    #include "windows.h"
#endif
#if defined(__linux__) || defined(__APPLE__)
	#include <ncurses.h>
#endif

extern int robotAddress[4];
extern unsigned int delayCounter;
extern char robLSpeed, robRSpeed;
extern char current_speed;
extern char robRedLed, robGreenLed, robBlueLed;
extern unsigned char exitProg;
extern unsigned int robotId;
#if defined(__linux__) || defined(__APPLE__)
int ch=0;
#endif

unsigned char HKeyPressed() {

#ifdef _WIN32
    if(GetKeyState (0x48) < 0) {     // 'h'
        return 1;
    } else {
        return 0;
    }
#endif

#if defined(__linux__) || defined(__APPLE__)
    ch = getch();
    if (ch != ERR) {
    	if(ch == 'h') {
			return 1;
        } else {
			return 0;
		}
	} else {
		return 0;
	}
#endif

}

void handleKeyboardInput() {

#ifdef _WIN32
        delayCounter++;
        if(delayCounter>40) {   // wait a little from one command to the other
            delayCounter = 0;

            if (GetKeyState (VK_LEFT) < 0) {
                robRSpeed = current_speed;
                robLSpeed = 0;
            } else if (GetKeyState (VK_RIGHT) < 0) {
                robLSpeed = current_speed;
                robRSpeed = 0;
            } else if (GetKeyState (VK_DOWN) < 0) {
                robLSpeed = -current_speed;
                robRSpeed = -current_speed;
            } else if (GetKeyState (VK_UP) < 0) {
                robLSpeed = current_speed;
                robRSpeed = current_speed;
            } else if (GetKeyState (VK_ADD) < 0) {
                current_speed += 5;
                if(current_speed > 100) {
                    current_speed = 100;
                }
            } else if (GetKeyState (VK_SUBTRACT) < 0) {
                current_speed -= 5;
                if(current_speed < -100) {
                    current_speed = -100;
                }
            } else if (GetKeyState (VK_SPACE) < 0) {
                current_speed = 0;
                robRedLed = 0;
                robGreenLed = 0;
                robBlueLed = 0;
            } else if (GetKeyState (0x41) < 0) {    // 'a'
                robLSpeed = -current_speed;
                robRSpeed = current_speed;
            } else if (GetKeyState (0x53) < 0) {    // 's'
                robLSpeed = current_speed;
                robRSpeed = -current_speed;
            } else if (GetKeyState (0x52) < 0) {    // 'r'
                robRedLed++;
                if(robRedLed > 100) {
                    robRedLed = 100;
                }
            } else if (GetKeyState (0x45) < 0) {    // 'e'
                robRedLed--;
                if(robRedLed < 0) {
                    robRedLed = 0;
                }
            } else if (GetKeyState (0x47) < 0) {    // 'g'
                robGreenLed++;
                if(robGreenLed > 100) {
                    robGreenLed = 100;
                }
            } else if (GetKeyState (0x46) < 0) {    // 'f'
                robGreenLed--;
                if(robGreenLed < 0) {
                    robGreenLed = 0;
                }
            } else if (GetKeyState (0x42) < 0) {    // 'b'
                robBlueLed++;
                if(robBlueLed > 100) {
                    robBlueLed = 100;
                }
            } else if (GetKeyState (0x56) < 0) {    // 'v'
                robBlueLed--;
                if(robBlueLed < 0) {
                    robBlueLed = 0;
                }
            } else if(GetKeyState (0x4b) < 0) {     // 'k'
                calibrateSensors(robotAddress[robotId]);
            } else if(GetKeyState (0x48) < 0) {     // 'h'
                printHelp();
            } else if(GetKeyState (0x51) < 0) {     // 'q'
                exitProg = 1;
            } else if(GetKeyState (0x30) < 0) {     // '0'
                robotId = 0;
            } else if(GetKeyState (0x31) < 0) {     // '1'
                robotId = 1;
            } else if(GetKeyState (0x32) < 0) {     // '2'
                robotId = 2;
            } else if(GetKeyState (0x33) < 0) {     // '3'
                robotId = 3;
            } else {
                robRSpeed = 0;
                robLSpeed = 0;
            }

        }

#endif

#if defined(__linux__) || defined(__APPLE__)
        ch = getch();
        if (ch != ERR) {
            if(ch == KEY_LEFT) {
				//printf("left\r\n");
                robRSpeed = current_speed;
                robLSpeed = 0;
            } else if(ch == KEY_RIGHT) {
				//printf("right\r\n");
                robLSpeed = current_speed;
                robRSpeed = 0;
            } else if(ch == KEY_DOWN) {
				//printf("down\r\n");
                robLSpeed = -current_speed;
                robRSpeed = -current_speed;
            } else if(ch == KEY_UP) {
				//printf("up\r\n");
                robLSpeed = current_speed;
                robRSpeed = current_speed;
            } else if(ch == '+') {
				//printf("+\r\n");
                current_speed += 5;
                if(current_speed > 100) {
                    current_speed = 100;
                }
            } else if(ch == '-') {
				//printf("-\r\n");
                current_speed -= 5;
                if(current_speed < -100) {
                    current_speed = -100;
                }
            } else if (ch == ' ') {
                current_speed = 0;
                robRedLed = 0;
                robGreenLed = 0;
                robBlueLed = 0;
            } else if(ch == 'a') {
                robLSpeed = -current_speed;
                robRSpeed = current_speed;
            } else if(ch == 's') {
                robLSpeed = current_speed;
                robRSpeed = -current_speed;
            } else if(ch == 'r') {
				robRedLed++;
				if(robRedLed > 100) {
					robRedLed = 100;
				}
            } else if(ch == 'e') {
				robRedLed--;
				if(robRedLed < 0) {
					robRedLed = 0;
				}
            } else if(ch == 'g') {
				robGreenLed++;
				if(robGreenLed > 100) {
					robGreenLed = 100;
				}
            } else if(ch == 'f') {
				robGreenLed--;
				if(robGreenLed < 0) {
					robGreenLed = 0;
				}
            } else if(ch == 'b') {
				robBlueLed++;
				if(robBlueLed > 100) {
					robBlueLed = 100;
				}
            } else if(ch == 'v') {
				robBlueLed--;
				if(robBlueLed < 0) {
					robBlueLed = 0;
				}
            } else if(ch == 'k') {
                calibrateSensors(robotAddress[robotId]);
            } else if(ch == 'h') {
                printHelp();
            } else if(ch == 'q') {
                exitProg = 1;
            } else if(ch == '0') {
                robotId = 0;
            } else if(ch == '1') {
                robotId = 1;
            } else if(ch == '2') {
                robotId = 2;
            } else if(ch == '3') {
               	robotId = 3;
            } else {
				//printf("pressed %c\r\n", ch);
			}
        } else {
            robRSpeed = 0;
            robLSpeed = 0;
        }
#endif

}

