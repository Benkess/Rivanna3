#include <mbed.h>
#include "MotorInterface.h"
#include "pindef.h"

/**
 * Motor driver that controls the throttle and regen values
 * @param serial_bus Mbed I2C object for throttle and regen communication, uses dual potentiometer so only one bus is needed
 */
MotorInterface::MotorInterface(I2C &serial_bus)
                : bus(serial_bus) {}

// Reference: https://www.mouser.com/datasheet/2/268/22096b-3443658.pdf#page=54.22
uint8_t i2c_cmd[2];
inline int writeI2CPot(I2C &serial_bus, uint16_t val, bool is_throttle) {
    val = 0x100 - val; // Is this line necessary?
    i2c_cmd[0] = (val >> 8) & 1;
    i2c_cmd[1] = val & 0xFF;
    constexpr uint8_t wiper_1_mask = 1 << 4;
    if(is_throttle) {
        i2c_cmd[0] |= wiper_1_mask;
    }
    return serial_bus.write(THROTTLE_REGEN_I2C_ADDRESS, reinterpret_cast<char*>(i2c_cmd), 2);
}

/**
 * Sends throttle value to digital POT
 * @param throttle Throttle value [0, 511]
 */
int MotorInterface::sendThrottle(uint16_t throttle) {
    return writeI2CPot(this->bus, throttle, true);
}

/**
 * Sends regen value to digital POT
 * @param regen Regen value [0, 511]
 */
int MotorInterface::sendRegen(uint16_t regen) {
    return writeI2CPot(this->bus, regen, false);
}
