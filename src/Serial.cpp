#include <Serial.h>

using namespace std;

string SerialDevice;
int serialfd;
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
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

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
  /*serialfd = open (SerialDevice.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  if (serialfd < 0)
  {
      cout << KGREEN << "Serial Device " << KRED << " -> Cant open " << SerialDevice << "!" << KNORMAL << endl;
      return -1;
  }
  set_interface_attribs (serialfd, B115200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
  set_blocking (serialfd, 0);*/
  return 0;
}
void Serial_Close()
{
   //close(serialfd);
}

long map_range(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Serial_Parse()
{
  Render_RenderStack();

  /*long input;

  char c;
  string line = "";
  vector<string> array;
  while (read(serialfd, &c, sizeof(char)) > 0)
  {
    if (c == '\n')
    {
      break;
    }
    else
    {
      line.push_back(c);
    }
    //printf("%c", c);
    //fflush(stdout);
  }
  if (line.find(" ") && line.size() > 1)
  {
    //printf("Serial = %s\n", line.c_str());
    array = split(line, ' ');
    for (int x = 0; x < ObjectStack.size(); x++)
    {
      if (array[0] == ObjectStack[x].tagname)
      {
        if (ObjectStack[x].type == INDICATOR)
        {
          if (array[1].find("ON") || array[1].find("OFF"))
          {
            if (array[1].find("ON"))
            {
              ObjectStack[x].visable = false; //Maybe someday ill figure out why this logic is backwords
            }
            else if (array[1].find("OFF"))
            {
              ObjectStack[x].visable = true;
            }
          }
        }
        else
        {
          if (ObjectStack[x].type == NEEDLE)
          {
            input = atof(array[1].c_str());
            ObjectStack[x].angle = map_range(input, ObjectStack[x].minvalue, ObjectStack[x].maxvalue, ObjectStack[x].minangle, ObjectStack[x].maxangle);
          }
        }
      }
    }
    line.clear();
  }*/
  usleep(100);
}
