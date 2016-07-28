#include <Serial.h>

using namespace std;

string SerialDevice;
int serialfd;
string line = "";
bool parse_now = false;

int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 20;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
        {
                printf ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}
void set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tggetattr", errno);
                return;
        }
        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf ("error %d setting term attributes", errno);
}

int Serial_Init()
{
  serialfd = open (SerialDevice.c_str(), O_RDWR | O_NDELAY);
  if (serialfd < 0)
  {
      cout << KGREEN << "Serial Device " << KRED << " -> Cant open " << SerialDevice << "!" << KNORMAL << endl;
      return -1;
  }
  set_interface_attribs (serialfd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
  set_blocking (serialfd, 1);
  return 0;
}
void Serial_Close()
{
   close(serialfd);
}

long map_range(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
void Serial_WriteByte(uint8_t byte)
{
  write(serialfd, &byte, 1);
}
void Serial_WriteString(string s)
{
  for (int x = 0; x < s.size(); x++)
  {
    //printf("Writing byte: %c\n\n", s.c_str()[x]);
    Serial_WriteByte(s.c_str()[x]);
  }
  Serial_WriteByte('\r');

  fcntl(serialfd, F_SETFL, 0); // block until data comes in

  char buffer[50];

  read(serialfd, &buffer, sizeof(buffer));

  printf("You entered: %s\n", buffer);

}
void Serial_Parse()
{
  while(!quit)
  {
    if (parse_now)
    {
      parse_now = false;
      //printf("Read: %s\n", buffer);
    }
  }
}
void Serial_Read()
{
  char c;
  while(!quit)
  {
    while (read(serialfd, &c, sizeof(char)) > 0)
    {
      if (c == '\r')
      {
        printf("Read: %s\n", line.c_str());
        line.clear();
        break;
      }
      else if (c == '\n')
      {
      }
      else
      {
        line.push_back(c);
      }
      //printf("%c", c);
      //fflush(stdout);
    }
  }
}
