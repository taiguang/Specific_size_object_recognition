#include "serial.h"
#include <unistd.h>
#include <stdio.h>
#include <math.h>

void drive( HANDLE fd, int velocity, int radius );
void drive_stop(HANDLE fd);
void drive_directAt( HANDLE fd, int right_vel, int left_vel);
void drive_direct(HANDLE fd, int right_vel, int left_vel, float angle);
void goStraightAt( HANDLE fd,int velocity );
void goStraight( HANDLE fd, int velocity, float distance);
void spinRightAt(HANDLE fd ,int velocity);
void spinRight( HANDLE fd ,int velocity , float angle );
void spinLeftAt(HANDLE fd ,int velocity);
void spinLeft( HANDLE fd ,int velocity , float angle );
