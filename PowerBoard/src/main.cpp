// #include "ECUCANStructs.h"
#include "PowerCANInterface.h"
#include "Printing.h"
#include "ThisThread.h"
#include "log.h"
#include "pindef.h"
#include <mbed.h>
#include <rtos.h>
#include <MotorInterface.h>
#include <events/EventQueue.h>
#include "ReadPedals.h"
#include "main.h"
#include "MotorCommandsCANStruct.h"
#include "MotorControllerCANInterface.h"
#include "CruiseControl.h"

#define LOG_LEVEL                       LOG_DEBUG
#define SIGNAL_FLASH_PERIOD             1s
#define BRAKE_LIGHTS_UPDATE_PERIOD      10ms
#define MOTOR_CONTROL_PERIOD            10ms
#define MOTOR_REQUEST_FRAMES_PERIOD     10ms

#define MAX_REGEN                       256

EventQueue queue(32 * EVENTS_EVENT_SIZE);

constexpr bool PIN_ON = true;
constexpr bool PIN_OFF = false;

DigitalOut bms_strobe(STROBE_EN);
DigitalOut brake_lights(BRAKE_LIGHT_EN);
DigitalOut right_turn_signal(RIGHT_TURN_EN);
DigitalOut left_turn_signal(LEFT_TURN_EN);
DigitalOut drl(DRL_EN);
DigitalOut mppt_precharge(MPPT_PRE_EN);
DigitalOut charge(CHARGE_EN);
DigitalOut motor_precharge(MTR_PRE_EN);
DigitalOut discharge(DIS_CHARGE_EN);

AnalogIn throttle_pedal(THROTTLE_WIPER, 5.0f);
AnalogIn brake_pedal(BRAKE_WIPER, 5.0f);
AnalogIn contactor(CONT_12);
AnalogIn aux_battery(AUX);

I2C throttle(ACCEL_SDA, ACCEL_SCL);
I2C regen(REGEN_SDA, REGEN_SCL);

MotorInterface motor_interface(throttle, regen);

PowerCANInterface vehicle_can_interface(MAIN_CAN_RX, MAIN_CAN_TX, MAIN_CAN_STBY);
MotorControllerCANInterface motor_controller_can_interface(MTR_CAN_RX, MTR_CAN_TX, NC); // TODO add standby pin

// these are global control variables, mostly set by received CAN messages
bool flashLeftTurnSignal = false;
bool flashRightTurnSignal = false;
bool flashHazards = false;
bool bms_error = false;
bool contact_12_error = false; //TODO currently does nothing
bool has_faulted = false; // Currently used to mean there is any fault that locks the car until reset
bool regen_enabled = false;
bool cruise_control_enabled = false;

bool cruise_control_brake_latch = false;

// Cruise Control constants
constexpr double MOTOR_RPM_TO_MPH_RATIO = (double) 0.0596;

// Cruise Control variables
CruiseControl cruise_control;
double current_speed_mph = 0;
uint16_t motor_rpm = 0;


void request_motor_frames() {
    motor_controller_can_interface.request_frames(true, true, true);
}

// main method
int main() {
    queue.call_every(MOTOR_REQUEST_FRAMES_PERIOD, request_motor_frames);
    queue.dispatch_forever();
}

// DashboardCommands CAN message handler
void PowerCANInterface::handle(DashboardCommands *can_struct){
    return;
}

// BPSError CAN message handler
void PowerCANInterface::handle(BPSError *can_struct) {
    bms_error = can_struct->internal_communications_fault || can_struct-> low_cell_voltage_fault || can_struct->open_wiring_fault || can_struct->current_sensor_fault || can_struct->pack_voltage_sensor_fault || can_struct->thermistor_fault || can_struct->canbus_communications_fault || can_struct->high_voltage_isolation_fault || can_struct->charge_limit_enforcement_fault || can_struct->discharge_limit_enforcement_fault || can_struct->charger_safety_relay_fault || can_struct->internal_thermistor_fault || can_struct->internal_memory_fault;
    has_faulted = true;
}

void MotorControllerCANInterface::message_forwarder(CANMessage *message) {
    return;
}

void MotorControllerCANInterface::handle(MotorControllerPowerStatus *can_struct) {
    can_struct->log(LOG_ERROR);
}

void MotorControllerCANInterface::handle(MotorControllerDriveStatus *can_struct) {
    can_struct->log(LOG_ERROR);
}

void MotorControllerCANInterface::handle(MotorControllerError *can_struct) {
    can_struct->log(LOG_ERROR);
}
