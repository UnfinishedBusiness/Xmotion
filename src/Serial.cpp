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
void replaceAll( string& source, const string& from, const string& to )
{
    string newString;
    newString.reserve( source.length() );  // avoids a few memory allocations

    string::size_type lastPos = 0;
    string::size_type findPos;

    while( string::npos != ( findPos = source.find( from, lastPos )))
    {
        newString.append( source, lastPos, findPos - lastPos );
        newString += to;
        lastPos = findPos + from.length();
    }

    // Care for the rest after last occurrence
    newString += source.substr( lastPos );

    source.swap( newString );
}
void Serial_WriteByte(uint8_t byte)
{
  write(serialfd, &byte, 1);
}
void Serial_WriteString(string s)
{
  char buffer[s.size()+2];
  sprintf(buffer, "%s\n", s.c_str());
  //printf("Buffer: %s\n", buffer);
  write(serialfd, buffer, sizeof(buffer));
}
int Serial_Read()
{
  Serial_Read(true);
}
int Serial_Read(bool report_ok)
{
  int data_ready;
  ioctl(serialfd, FIONREAD, &data_ready);
  unsigned char buffer[data_ready];
  read(serialfd, buffer, sizeof(buffer));
  buffer[strlen((char*)buffer) - 1] = '\0';
  //printf("Read: %s\n", buffer);
  string data(reinterpret_cast<char*>(buffer));
  vector<string> lines = split(data, '\n');
  for(int x = 0; x < lines.size(); x++)
  {
    try
    {
      //printf("Line %i - %s\n", x, lines[x].c_str());
      replaceAll(lines[x], "\r", "");
      if (lines[x].at(0) == '<' && lines[x].at(lines[x].size()-1) == '>')
      {
        //printf("Found info string!\n");
        replaceAll(lines[x], "<", "");
        replaceAll(lines[x], ">", "");

        vector<string> fields = split(lines[x], ',');
        //printf("Size == %i\n", fields.size());
        if (fields.size() == 7)
        {
          MachineState = fields[0];

          //Work positions are 4, 5, 6; 4 contains the poorly formatted "WPos:"
          replaceAll(fields[4], "WPos:", "");
          WPos_X = atof(fields[4].c_str());
          WPos_Y = atof(fields[5].c_str());
          WPos_Z = atof(fields[6].c_str());

        }
      }
      if (lines[x] == "ok")
      {
        //printf("Recieved ok!\n");
        ready_to_send_next = true;
        return 0;
      }
      if (lines[x].find(lines[x]) != std::string::npos)
      {
        if (report_ok)
        {
          ready_to_send_next = true;
        }
        return 1;
      }
    }
    catch (const std::out_of_range& oor)
    {
      //std::cerr << "Out of Range error: " << oor.what() << '\n';
    }
  }
  return -1;
}
