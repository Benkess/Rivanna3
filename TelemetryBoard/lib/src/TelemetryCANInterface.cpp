#include "TelemetryCANInterface.h"
#include "MotorControllerCANStructs.h"
#include "log.h"
#include "MotorCommandsCANStruct.h"
#include "HeartBeatCANStruct.h"

TelemetryCANInterface::TelemetryCANInterface(PinName rd, PinName td,
                                     PinName standby_pin)
    : CANInterface(rd, td, standby_pin) {
    can.frequency(250000);
}


void TelemetryCANInterface::message_handler() {
    while (true) {
        ThisThread::flags_wait_all(0x1);
        CANMessage message;
        while (can.read(message)) {
            char message_data[17];

            // TODO: Write to serial message_id, message_data

            CANInterface::write_CAN_message_data_to_buffer(message_data, &message);
            log_debug(
                "Received CAN message with ID 0x%03X Length %d Data 0x%s ",
                message.id, message.len, message_data);
        }
    }
}

