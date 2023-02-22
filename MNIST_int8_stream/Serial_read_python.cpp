#include <Arduino.h>
#include <stdint.h>
#include "Serial_read_python.h"

extern char buffer[50];
extern String data_str;
extern int num_stream_data;
extern int num_iterations;

int read_data_from_python(int8_t* stream_data) {
  int temp_label;
  // uint8_t* temp_data;
  if (Serial.available() > 0) {
    data_str = Serial.readStringUntil('\n');
    int i = 0;
    // Serial.print("data_str.length(): ");
    // Serial.print(data_str.length());
    for (int j = 0; j < data_str.length(); j++) { // counts characters until the next ","
      if (j < (data_str.length()-1) && data_str[j] == ',') {
        if (i < len_stream_data) {
          stream_data[i] = data_str.substring(0, j).toInt() - 128; // characters until the next "," converted to int8_t
        } else if (i == len_stream_data) {
          // Serial.print("temp_label: ");
          // Serial.print(data_str.substring(0, j));
          // Serial.print(" --> ");
          // Serial.print(data_str.substring(0, j).toInt());
          temp_label = data_str.substring(0, j).toInt();
          // Serial.print(" --> ");
          // Serial.print(temp_label);
          // Serial.print(", ");
        } else {
          // Serial.print("num_iterations: ");
          // Serial.print(data_str.substring(0, j));
          // Serial.print(" --> ");
          // Serial.print(data_str.substring(0, j).toInt());
          num_iterations = data_str.substring(0, j).toInt();
          // Serial.print(" --> ");
          // Serial.print(num_iterations);
          // Serial.print(", ");
        }
        data_str = data_str.substring(j+1);
        j = -1;
        i++;
      }
    }
    // Serial.print(";num_stream_data (data_str.toInt()): ");
    // Serial.print(data_str.toInt());
    num_stream_data = data_str.toInt();
    // Serial.print(";num_stream_data: ");
    // Serial.print(data_str.toInt());
    // Serial.print(";");
  }
  
  // sprintf(buffer, "stream_data[%d]: %d;", len_stream_data-5, stream_data[len_stream_data-5]);
  // Serial.print(buffer);
  // sprintf(buffer, "stream_data[%d]: %d;", len_stream_data-4, stream_data[len_stream_data-4]);
  // Serial.print(buffer);
  // sprintf(buffer, "stream_data[%d]: %d;", len_stream_data-3, stream_data[len_stream_data-3]);
  // Serial.print(buffer);
  // sprintf(buffer, "stream_data[%d]: %d;", len_stream_data-2, stream_data[len_stream_data-2]);
  // Serial.print(buffer);
  // sprintf(buffer, "stream_data[%d]: %d;", len_stream_data-1, stream_data[len_stream_data-1]);

  Serial.print("data: ");
  for(int i=0; i<len_stream_data; i++){
    sprintf(buffer, "%d,", stream_data[i]);
    Serial.print(buffer);
  }
  
  sprintf(buffer, ";label: %d;", temp_label);
  Serial.print(buffer);
  sprintf(buffer, "num_iterations: %d;", num_iterations);
  Serial.print(buffer);
  sprintf(buffer, "num_stream_data: %d;", num_stream_data);
  Serial.print(buffer);
  return temp_label;
}