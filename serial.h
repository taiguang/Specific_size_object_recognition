#include <termio.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define Linux

#ifdef Cygwin
#include <windows.h>
#define DevInfo			DCB
#define GetDevInfo		GetCommState
#define SetDevInfo(a,b)		SetCommState(a,b)
#define DevInfoFailed		(0)
#define CloseDev		CloseHandle

#else /*Linux*/

#define HANDLE int
#define BYTE unsigned char
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define DevInfo			struct termio
#define GetDevInfo		tcgetattr
#define SetDevInfo(a,b)		tcsetattr(a, TCSAFLUSH, b)
#define DevInfoFailed		(-1)
#define INVALID_HANDLE_VALUE	(-1)
#define HANDLE			int
#define DWORD			int
#define CloseDev		close
#define BYTE                    unsigned char

#endif
HANDLE setup_serial(char *devfile, int baudrate);
int write_serial(HANDLE fd, BYTE *buf, int size);
int read_serial(HANDLE fd, BYTE *buf, int size);
int close_serial(HANDLE fd);
double get_dtime(void);
