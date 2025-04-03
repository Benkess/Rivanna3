#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

#include <mbed.h>

static constexpr int THROTTLE_REGEN_I2C_ADDRESS = 0b0101;

class MotorInterface {

  public:
    MotorInterface(I2C &serial_bus);
    int sendThrottle(uint16_t throttle);
    int sendRegen(uint16_t regen);

  private:
    I2C &bus;
};

#endif