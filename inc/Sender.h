#ifndef SENDER_
#define SENDER_
#include <application.h>

extern bool ready_to_send_next;
void Sender_Tick();
void Sender_Stop();

void Sender_SendNextLine();
#endif
