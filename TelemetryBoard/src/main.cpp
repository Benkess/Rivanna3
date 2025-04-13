#include "TelemetryCANInterface.h"
#include "Printing.h"
#include "ThisThread.h"
#include "log.h"
#include "pindef.h"
#include <mbed.h>
#include <rtos.h>

DigitalOut led(PC_13);
TelemetryCANInterface can_interface(MAIN_CAN_RX, MAIN_CAN_TX, MAIN_CAN_STBY);

int main() {

    log_set_level(LOG_DEBUG);
    log_debug("Telemetry Board started");
    uint8_t data[8];

    for(int i=0; i<8; ++i) {
        data[i] = (char) i;
    }

    CANMessage to_send(12, data);
    while (true) {
        ThisThread::sleep_for(1000ms);
        // log_debug("Telemetry Board is running");
        can_interface.send_message(&to_send);
    }
}