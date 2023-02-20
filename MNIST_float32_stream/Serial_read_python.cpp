#include "Serial_read_python.h"
#include <Arduino.h>

extern char buffer[50];
extern String data_str;
extern int num_stream_data;
extern int num_iterations;

int read_data_from_python(float* temp_data) {
  int temp_label;
  if (Serial.available() > 0) {
    data_str = Serial.readStringUntil('\n');
    sprintf(buffer, "data_str.length(): %d;", data_str.length());
    Serial.print(buffer);
    sprintf(buffer, "len_stream_data: %d;", len_stream_data);
    Serial.print(buffer);
    int i = 0;
    for (int j = 0; j < data_str.length(); j++) { // counts characters until the next ","
      if (j < (data_str.length()-1) && data_str[j] == ',') {
        if (i < len_stream_data) {
          temp_data[i] = data_str.substring(0, j).toFloat(); // characters until the next "," converted to float
        } else if (i == len_stream_data) {
          temp_label = data_str.substring(0, j).toInt();
        } else {
          num_iterations = data_str.substring(0, j).toInt();
        }
        data_str = data_str.substring(j+1);
        j = -1;
        i++;
      }
      // sprintf(buffer, "data_str.length(): %d, ", data_str.length());
      // Serial.print(buffer);
      // sprintf(buffer, "i: %d, ", i);
      // Serial.print(buffer);
      // sprintf(buffer, "j: %d: ", j);
      // Serial.print(buffer);
      // Serial.print(data_str);
      // Serial.print(";");
    }
    num_stream_data = data_str.toInt();
  }


  // Serial.print("data: ");
  // for(int i=0; i<len_stream_data; i++){
  //   sprintf(buffer, "%f,", temp_data[i]);
  //   Serial.print(buffer);
  // }
  Serial.print(";");
  sprintf(buffer, "label: %d;", temp_label);
  Serial.print(buffer);
  sprintf(buffer, "num_iterations: %d;", num_iterations);
  Serial.print(buffer);
  sprintf(buffer, "num_stream_data: %d;", num_stream_data);
  Serial.print(buffer);


  return temp_label;
}