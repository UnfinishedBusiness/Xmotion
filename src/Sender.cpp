#include <Sender.h>

using namespace std;

ifstream nc_file;

bool ready_to_send_next;

void Sender_Tick()
{
  if (MachineState == "Run" && ready_to_send_next == true)
  {
    ready_to_send_next = false;
    if (nc_file.is_open()) //File is already open, send next line until que ammount is reached else open file
    {
      string line;
      if (!getline (nc_file, line))
      {
        printf("End of file!\n");
        MachineState = "Idle";
        Sender_Stop();
        return;
      }
      line.erase(std::remove(line.begin(), line.end(), '\n'), line.end()); //Remove all newline characters
      line.erase(std::remove(line.begin(), line.end(), '\r'), line.end()); //Remove all carage return characters

      printf("Sending Gcode: %s\n", line.c_str());
      Serial_WriteString(line);
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
