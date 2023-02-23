#include <Arduino.h>
#include <stdint.h>
#include "Serial_read_python.h"

extern char buffer[50];
extern String data_str;
extern int num_stream_data;
extern int num_iterations;

int read_data_from_python(int8_t* stream_data) {
  int temp_label;
  if (Serial.available() > 0) {
    data_str = Serial.readStringUntil('\n');
    int i = 0;
    for (int j = 0; j < data_str.length(); j++) { // counts characters until the next ","
      if (j < (data_str.length()-1) && data_str[j] == ',') {
        if (i < len_stream_data) {
          stream_data[i] = data_str.substring(0, j).toInt() - 128; // characters until the next "," converted to int8_t
        } else if (i == len_stream_data) {
          temp_label = data_str.substring(0, j).toInt();
        } else {
          num_iterations = data_str.substring(0, j).toInt();
        }
        data_str = data_str.substring(j+1);
        j = -1;
        i++;
      }
    }
    num_stream_data = data_str.toInt();
  }
  
  return temp_label;
}