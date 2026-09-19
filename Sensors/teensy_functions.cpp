#include "teensy_functions.h"

uint8_t tempCheck(uint8_t pin, float min, float max){
  float e = 2.718281828459045;
  int rawRead = analogRead(pin);
  float voltage = (rawRead / 1023.0f) * 3.3f;
  float R =10 *((3.3/ voltage) - 1.0);
  float sys_temp = 90*(pow(e,(-.13*R)));

  Serial.println(rawRead);
  Serial.println(voltage);
  Serial.println(R);
  Serial.println(sys_temp);
  Serial.print(" -> pin ");
  Serial.println(pin);
  if(sys_temp < min){
    sys_temp = min;
  }
  if(sys_temp > max){
    sys_temp = max;
  }
  return sys_temp;
}
void sendHeartbeat() { //sends heartbeat communication to other teensy
  //ack always 0 for now
  String cmd = "HB," + "1" + ",0";

  uint8_t cs = verifyCheckSum(cmd);

  Serial1.print('<');
  Serial1.print(cmd);
  Serial1.print('|');
  Serial1.print(cs);
  Serial1.println('>');
  Serial1.println('\n');

  Serial.print("Sent: ");
  Serial.print('<');
  Serial.print(cmd);
  Serial.print('|');
  Serial.print(cs);
  Serial.println('>');
  Serial.println('\n');
}

void parsePacket(char *cmd, int *command_values){ //parses packet to extract info
  int index = 0;
  char *delim = strtok(cmd, ",");
  while(delim != NULL && index < 1){
    command_values[index] = atoi(delim);
    delim = strtok(NULL, ","); // Start where last "," found
    index++;
  }
}

bool verifyCheckSum(String cmd, int received_check_sum){ //verifies packet to prevent corruption
  int check_sum = 0;
  for(int i = 0; i < static_cast<int>(cmd.length()); i++){
    check_sum ^= cmd[i];
  } 
  return check_sum == received_check_sum;
}


