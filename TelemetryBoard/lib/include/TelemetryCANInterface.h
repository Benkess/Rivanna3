#ifndef TELE_CAN_INTERFACE_H
#define TELE_CAN_INTERFACE_H

#include "BPSCANStructs.h"
#include "CANInterface.h"
#include "ECUCANStructs.h"
#include "MotorControllerCANStructs.h"
#include "DashboardCommandsCANStruct.h"
#include "MotorCommandsCANStruct.h"
#include "HeartBeatCANStruct.h" // heartbeat

class TelemetryCANInterface : public CANInterface {
  public:
    TelemetryCANInterface(PinName rd, PinName td, PinName standby_pin);

  private:
    void message_handler() override;
};

#endif