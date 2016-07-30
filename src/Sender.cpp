#include <Sender.h>

using namespace std;

ifstream nc_file;

void Sender_SendNextLine()
{
  if (MachineState == "Run")
  {
    if (nc_file.is_open()) //File is already open, send next line until que ammount is reached else open file
    {
      string line;
      getline (nc_file, line);
      line.erase(std::remove(line.begin(), line.end(), '\n'), line.end()); //Remove all newline characters
      line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); //Remove all carage return characters

      printf("Sending Gcode: %s\n", line.c_str());
      Serial_WriteStringAndWaitForOk(line.c_str());
    }
    else
    {
      nc_file.open(current_file);
    }
  }

}
void Sender_Stop()
{
  nc_file.close();
}
