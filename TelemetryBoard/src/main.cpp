#include "TelemetryCANInterface.h"
#include "Printing.h"
#include "ThisThread.h"
#include "log.h"
#include "pindef.h"
#include <mbed.h>
#include <rtos.h>

DigitalOut led(PC_13);

int main() {
    TelemetryCANInterface can_interface(PA_11, PA_12, PA_10);

    log_set_level(LOG_DEBUG);
    log_debug("Telemetry Board started");
    while (true) {
        ThisThread::sleep_for(1000ms);
        log_debug("Telemetry Board is running");
    }
}