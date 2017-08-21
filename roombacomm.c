#include "serial.h"
#include "roombacomm.h"

void drive( HANDLE fd, int velocity, int radius )
{
  BYTE cmd[] = { (BYTE)137,(BYTE)(velocity >> 8),(BYTE)(velocity & 0xff), 
		 (BYTE)(radius >> 8), (BYTE)(radius & 0xff) };
  write_serial(fd, cmd, 5);
}

void drive_stop(HANDLE fd)
{
  drive(fd,0,0);
}

void drive_directAt( HANDLE fd, int right_vel, int left_vel)
{
  BYTE cmd[] = { (BYTE)145,(BYTE)(right_vel >> 8),(BYTE)(right_vel & 0xff),
		 (BYTE)(left_vel >> 8), (BYTE)(left_vel & 0xff) };
  write_serial(fd,cmd,5);
}

void drive_direct(HANDLE fd, int right_vel, int left_vel, float angle)
{
  BYTE cmd[] = { (BYTE)145,(BYTE)(right_vel >> 8),(BYTE)(right_vel & 0xff),
		 (BYTE)(left_vel >> 8), (BYTE)(left_vel & 0xff) };
  float pausetime = fabs( 235.0 * M_PI / 360.0 * angle / (right_vel - left_vel) );
  write_serial(fd,cmd,5);
  usleep( (int)(pausetime*1000000) );
  drive_stop(fd);
}

void goStraightAt( HANDLE fd,int velocity )
{
  if( velocity > 500 ) velocity = 500;
  if( velocity < -500 ) velocity = -500;
  drive( fd,velocity, 0x8000 );
}

void goStraight( HANDLE fd, int velocity, float distance)
{
  float pausetime = fabs((float)distance / velocity);  // mm/(mm/sec) = sec
  if (distance > 0)
    goStraightAt( fd, velocity );
  else
    goStraightAt( fd, -velocity);
  usleep( (int)(pausetime*1000000) );
  drive_stop(fd);
}

void spinRightAt(HANDLE fd ,int velocity)
{
  drive( fd , velocity , -1 ); 
}

void spinRight( HANDLE fd ,int velocity , float angle )
{
  if( angle < 0 ) return;
  float pausetime = fabs( 235.0 * M_PI / 360.0 * angle / velocity );
  spinRightAt( fd , fabs(velocity) );
  usleep( (int)(pausetime*1000000) );
  drive_stop(fd);  
}

void spinLeftAt(HANDLE fd ,int velocity)
{
  drive( fd , velocity , 1 ); 
}

void spinLeft( HANDLE fd ,int velocity , float angle )
{
  if( angle < 0 ) return;
  float pausetime = fabs( 235.0 * M_PI / 360.0 * angle / velocity );
  spinLeftAt( fd , fabs(velocity) );
  usleep( (int)(pausetime*1000000) );
  drive_stop(fd);
}
