#include "serial.h"
#define BAUDRATE B115200

#ifdef Cygwin
// エラーを表示する関数 from http://msdn.microsoft.com/ja-jp/library/cc428939.aspx
void PrintLastError(char *msg) {
  LPVOID lpMsgBuf;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //言語
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);
  // lpMsgBuf内のすべての挿入シーケンスを処理する。
  // ...
  // 文字列を表示する。
  fprintf(stderr, "%s: %s\n", msg, (LPCTSTR)lpMsgBuf);
  // バッファを解放する。
  LocalFree(lpMsgBuf);
}
#else
#define PrintLastError		perror
#endif

HANDLE setup_serial(char *devfile, int baudrate)
{
  HANDLE fd;
  int status;
  DevInfo tio;
  
#ifdef Cygwin
  fd = CreateFile(devfile, GENERIC_READ|GENERIC_WRITE, 
		  0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
#else
  fd = open (devfile, O_RDWR);
#endif
  if (fd == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "device open failed (\"%s\") => %d\n", devfile, fd);
    exit(1);
  }
  
  fprintf(stderr, "\"%s\" is opened => %d\n", devfile, fd);

  usleep(100000);
  
#ifdef Cygwin
  status = GetDevInfo (fd, &tio);
  if (status == DevInfoFailed) PrintLastError("GetDevInfo");

  tio.BaudRate = baudrate;
  tio.fBinary = TRUE ;
  tio.ByteSize = 8;
  tio.Parity = NOPARITY;
  tio.fParity = FALSE;
  tio.StopBits = ONESTOPBIT;
  /* 2000.2.11 added for hangup trouble on TPad770X */
  tio.fOutxCtsFlow = FALSE;
  tio.fOutxDsrFlow = FALSE;
  tio.fDtrControl = DTR_CONTROL_DISABLE;
  tio.fDsrSensitivity = FALSE;
  tio.fTXContinueOnXoff = FALSE;
  tio.fTXContinueOnXoff = FALSE;
  tio.fOutX = FALSE;
  tio.fInX = FALSE;
  tio.fRtsControl = RTS_CONTROL_DISABLE;
  
#if 1
  {   COMMTIMEOUTS CommTimeouts;
    CommTimeouts.ReadTotalTimeoutConstant = 1000; /* [ms] */
    CommTimeouts.ReadTotalTimeoutMultiplier = 1; /* [ms/byte] */
    CommTimeouts.WriteTotalTimeoutConstant = 1000; /* [ms] */
    CommTimeouts.WriteTotalTimeoutMultiplier = 1; /* [ms/byte] */
    if (SetCommTimeouts(fd, &CommTimeouts) == 0) {
      PrintLastError("SetCommTimeouts");
    }
  }
#endif
  
  status = SetDevInfo (fd, &tio);
  if (status == DevInfoFailed) PrintLastError("GetDevInfo");
  
#else

  status = ioctl (fd, TCGETA, &tio);//現在のシリアルポートの設定を取得する
  if (status < 0) perror("TCGETA");
  printf("status=%d\n",status);
  /*speed_t rate;
  
  switch (baudrate) {
  case 1200: rate = B1200; break;
  case 2400: rate = B2400; break;
  case 4800: rate = B4800; break;
  case 9600: rate = B9600; break;
  case 19200: rate = B19200; break;
  case 38400: rate = B38400; break;
  case 57600: rate = B57600; break;
  case 115200: rate = B115200; break;
  case 153600: rate = B230400; break;
  case 307200: rate = B460800; break;
  case 230400: rate = B230400; break;
  case 460800: rate = B460800; break;
  default: rate = baudrate;
  }*/
  
  //久禮君の roombacomm.c からコピペ
  
  ////tio.c_cflag =  CLOCAL | CREAD;
  //tio.c_cflag &= ~CRTSCTS;
  
  /*   tio.c_cflag |= B115200; */
  tio.c_cflag = 0;//一度0にセットして全てのフラグをリセットする．
  tio.c_cflag |= (BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD);
  //tio.c_cflag |= rate;
  ////tio.c_lflag &= ~ECHO;  /* 入力された文字列をエコーしない */
  
  ////tio.c_lflag &= ~ICANON;	/* カノニカルモード有効、無効 */
  //-isig -icanon -iexten -echo -echoe -echok -echoctl -echoke
  //-icanon -echo -echoe -echok
  
  ////tio.c_cflag |= CREAD;  /*受信文字を有効にする*/
  
  tio.c_cc[VTIME] = 10;
  tio.c_cc[VMIN] = 0;
  
  tio.c_iflag = 0;
  tio.c_oflag = 0;  //要チェック
  
  tio.c_lflag = 0;
  //tio.c_lflag &= ~ISIG; //要チェック
  /*
  tio.c_lflag &= ~PARENB;
  tio.c_lflag &= ~CSIZE; 
  tio.c_cflag |= CS8;
  */
  
  status = ioctl (fd, TCSETA, &tio);//現在のシリアルポートの設定をセットする
  ////tcflush(fd, TCIFLUSH);
  ////status = tcsetattr(fd,TCSANOW,&tio);
  if (status < 0) perror("TCSETA");

#endif
  
  return fd;
}

// close (return 0 if success)
int close_serial(HANDLE fd) {
  int status;
#ifdef Cygwin
  status = CloseHandle(fd);
#else
  status = close(fd);
#endif
  if (status == DevInfoFailed)
    return -1;
  else {
    fprintf(stdout,"Handle closed\n",fd);
    return 0;
  }
}

// write
int write_serial(HANDLE fd, BYTE *buf, int size) {
  int status;
  DWORD written;
#ifdef Cygwin
  status = WriteFile (fd, buf, size, &written, NULL);
  if (status==0) PrintLastError("WriteFile");
#else
  status = write (fd, buf, size);
  if (status<=0) PrintLastError("write");
  written = status;
#endif
  if (written!=size) fprintf(stderr, "only sent %d bytes (of %d)\n", written, size);
  //else fprintf(stdout,"status=%d:Written\n",status);
  return written;
}

// read
int read_serial(HANDLE fd, BYTE *buf, int size) {
  int status;
  DWORD gotten;
#ifdef Cygwin
#if 0
  while (size>0) {
    status = ReadFile (fd, buf, size, &gotten, NULL);
    if (status==0) PrintLastError("ReadFile");
    size -= gotten;
    if (size>0) fprintf(stderr, "only received %d bytes (of %d)\n", gotten, size);
  }
#endif
  status = ReadFile (fd, buf, size, &gotten, NULL);
  if (status==0) PrintLastError("ReadFile");
  if (gotten<size)
    fprintf(stderr, "only received %d bytes (of %d)\n", gotten, size);
#else
  status = read(fd, buf, size);
  if (status<=0) PrintLastError("read");
  gotten = status;
#endif
  //fprintf(stdout,"status=%d:Red\n",status);
  return gotten;
}

double get_dtime(void){
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((double)(tv.tv_sec)*1000*1000 + (double)(tv.tv_usec));
}
