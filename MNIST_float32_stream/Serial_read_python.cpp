#include "Serial_read_python.h"
#include <Arduino.h>

extern char buffer[50];
extern String data_str;
extern int num_stream_data;
extern int num_iterations;

int read_data_from_python(float* stream_data) {
  int data_label;
  if (Serial.available() > 0) {
    data_str = Serial.readStringUntil('\n');
    int i = 0;
    for (int j = 0; j < data_str.length(); j++) { // counts characters until the next ","
      if (j < (data_str.length()-1) && data_str[j] == ',') {
        if (i < len_stream_data) {
          stream_data[i] = data_str.substring(0, j).toInt() / 255.0; // characters until the next "," converted to float
        }
        data_str = data_str.substring(j+1);
        j = -1;
        i++;
      }
    }
    data_label = data_str.toInt();
  }

  return data_label;
}