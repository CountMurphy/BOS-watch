#ifndef _SWITCH_H_
#define _SWITCH_H_

void SwitchInit();
uint8_t GetMode();
uint8_t GetMainMode();
uint8_t GetSubMode();
bool GetExecStatus();
bool GetSubStatus();
bool GetFirstRun();
void CommandSetAck();
bool InterruptTriggered();

#endif