  #include <elapsedMillis.h>
  #include "teensy_functions.h"

  void setup() {
    Serial1.begin(115200);
    sinceHbRx = HB_TIMEOUT_MS+1;
    analogReadResolution(10);
    
    for (size_t i = 0; i < NUM_PINS; i++) {
      pinMode(pins[i], INPUT);
    }

    pinMode(KILL_PIN, OUTPUT);
    digitalWrite(KILL_PIN, LOW);        // LOW = KILL
  }

  void loop() {
    if(Serial1.available() > 0){
      comms_ok = true;
      //Grab our packet
      String packet = Serial1.readStringUntil('\n');

      //Remove delimiters:
      packet.trim();
      if(packet.startsWith("<") && packet.endsWith(">")){
        packet = packet.substring(1, packet.length() - 1); // Grab packet contents minus the start/end frame characters
      }
      else{
        Serial1.println("ERROR: Invalid packet format received");
        return;
      }
      
      //Split into command and checksum
      int cmd_end = packet.indexOf("|");
      if(cmd_end == -1){
        Serial1.println("ERROR: No checksum operator detected");
        return;
      }
      String cmd = packet.substring(0, cmd_end);
      int check_sum = packet.substring(cmd_end + 1).toInt();
      if(verifyCheckSum(cmd, check_sum)){
        //Split command 
        int command_values[1];
        char cmd_buffer[cmd.length() + 1]; //For C-style string processing
        cmd.toCharArray(cmd_buffer, cmd.length() + 1); //For C-style string processing
        parsePacket(cmd_buffer, command_values);
        //uint8_t heartbeat_pin = static_cast<uint8_t>(command_values[0]);
        Serial.print("Received: ");
        Serial.print(command_values[0]);
        Serial.print("\n");

      }
      if (millis() - lastSend >= HB_PERIOD_MS) {
      lastSend = millis();
      sendHeartbeat();
      }

    }

    else if(sinceHbRx > HB_TIMEOUT_MS){
      comms_ok = false;
    }
    bool all_ok = true;
    String errorPin = "";
    for (int i = 0; i < pins_array_size; i++) {
      for(int j = 0; j < check_array_size; j++) {
        uint8_t volt_read = voltCheck(pins[i], MIN_VOLT, MAX_VOLT, FILTER_MIN, FILTER_MAX);
        volt_read += volt_check_array;
        volt_read += volt_total;
        uint8_t temp_read = tempCheck(pins[i], MIN_TEMP, MAX_TEMP, FILTER_MIN, FILTER_MAX);
        temp_check_array += temp_read;
        temp_total += temp_read;
      }
      //average values to reduce outlier effect
      volt_avg = volt_total/check_array_size;
      temp_avg = temp_total/check_array_size;
      //error if average too high
      if(volt_avg <= MIN_VOLT){
        Serial.println("ERROR: VOLTAGE UNDERFLOW AT PIN");
        Serial.println(pins[i]);
        volt_error = true;
      }
      if(volt_avg >= MAX_VOLT){
        Serial.println("ERROR: VOLTAGE OVERFLOW AT PIN");
        Serial.println(pins[i]);
        volt_error = true;
      }
      if(temp_avg <= MIN_TEMP){
        Serial.println("ERROR: UNDERHEAT AT PIN");
        Serial.println(pins[i]);
        volt_error = true;
      }
      if(temp_avg >= MAX_TEMP){
        Serial.println("ERROR: OVERHEAT AT PIN");
        Serial.println(pins[i]);
        volt_error = true;
      }
      for(int j = 0; j < pins_array_size; j++ ){
        volt_total = volt_total += volt_check_array[j];
        temp_total = temp_total += temp_check_array[j];
      }

      if (!volt_error || !temp_error) {
        errorPin += String(pins[i]);   
        errorPin += ",";               
      }
      all_ok = all_ok && volt_ok && temp_ok;
    }

    if (!all_ok) {
      for(int i = 0; i < errorPin.length(); i++){
        Serial.print(errorPin[i]);
      }
      kill_tripped = true;
      // for(;;){
      //   delay(2222);
      // }
    }

    // Later AND this with comms_ok from UART heartbeat
    digitalWrite(KILL_PIN, kill_tripped ? LOW : HIGH);
    delay(LOOP_TIME);
  }
