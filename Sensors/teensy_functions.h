#ifndef TEENSY_FUNCTIONS_H
#define TEENSY_FUNCTIONS_H
#include <Arduino.h>

  uint32_t LOOP_TIME = 10; //how often checks should be run
  static constexpr uint32_t HB_PERIOD_MS  = 50; //how often HB should be send
  static constexpr uint32_t HB_TIMEOUT_MS = 250; //how often controller should wait before assuming other controller is dead
  elapsedMillis sinceHbRx; //how often since last heartbeat received
  unsigned long lastSend = 0; //stores when last send occurred in milliseconds

  static const uint8_t check_array_size = 10; //how many readings to take before averaging to mitigate noise effect
  static const uint8_t pins_array_size = 4; //how many pins are checked
  
  uint8_t volt_total = 0; //add up all voltage values for averaging
  uint8_t volt_avg = 0; //stores average of voltage readings
  uint8_t  temp_total = 0; //add up all temperature values for averaging
  uint8_t temp_avg = 0; //stores average of temperature values

  bool volt_error = true; //start errored to start killed, ***DO NOT CHANGE TO FALSE FOR SAFETY***
  bool temp_error = true; //start errored to start killed, ***DO NOT CHANGE TO FALSE FOR SAFETY***
  uint8_t const pins[pins_array_size] = {14, 15, 16, 41}; // 20,21 to control teensy, A0, A1, A2, A3, 14, 15, 16, 17, 
  uint8_t temp_check_array[pins_array_size][check_array_size] = {0}; //stores temperature check values
  uint8_t volt_check_array[pins_array_size][check_array_size] = {0}; //stores voltage check values
  size_t const NUM_PINS = sizeof(pins); //used for averaging, no need to change
  float const MIN_VOLT = 0.0f; //minimum acceptable voltage for machine to run
  float const MAX_VOLT = 1000.0f; //maximum acceptable voltage for machine to run
  float const MIN_TEMP = 0.0f; //minimum temperature voltage for machine to run
  float const MAX_TEMP = 100000.0f; //minimum temperature voltage for machine to run
  float const FILTER_MIN = 0.0f; //filters all readings below this amount
  float const FILTER_MAX = 100.0f; //filters all readings above this amount
  uint8_t const KILL_PIN = 6; //pin used for kill switch
  bool comms_ok = false; // tracks if heartbeat is occurring
  static bool kill_tripped = true; // start killed, ***DO NOT CHANGE TO FALSE FOR SAFETY***

  void sendHeartbeat();
  void parsePacket(char *cmd, int *command_values);
  bool verifyCheckSum(String cmd, int received_check_sum);
  uint8_t tempCheck(uint8_t pin, float cart_min, float cart_max, float filter_min, float filter_max);

#endif