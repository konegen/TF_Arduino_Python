#include <TensorFlowLite.h>
#include "TF_Lite_exe.h"
#include "Model_evaluation.h"
#include "Serial_read_python.h"

String command;

char buffer[50];
float start_time;
float end_time;
float total_time;
float mean_time;

int num_stream_data;
const int len_stream_data = 784; // 28*28*1 Pixel
int label;
String data_str;

int num_iterations;

uint8_t* pred_time;
uint8_t* pred_eval;
float acc_sample;
float acc_sum;
float acc;


void setup() {
  Serial.begin(9600);
  delay(2500);
  setup_model();
}


void loop() {

  while (!Serial.available());  // Wait for incoming data to be received
  command = Serial.readStringUntil('\n');
  
  if(command.equals("time")) {
    // Measure execution time of the model
    while (!Serial.available());  // Wait for incoming data to be received

    Serial.print("MNIST_int8;");

    uint8_t data[len_stream_data];
    label = read_data_from_python(data);
    
    measure_execution_time(num_iterations, data);
  }
  else if(command.equals("acc")) {
    // Evaluate the accuracy of the model
    while (!Serial.available());  // Wait for incoming data to be received

    // uint8_t data[len_stream_data];
    // label = read_data_from_python(data);

    // Serial.print("MNIST_int8;");
    // // sprintf(buffer, "Number of test samples: %d;", num_stream_data);
    // // Serial.print(buffer);
    
    // measure_execution_time(num_iterations, data);
    
    acc_sum = 0;

    Serial.print("num_stream_data: ");
    Serial.print(num_stream_data);

    for(int i=0; i<num_stream_data; i++){
      while (!Serial.available());

      uint8_t data[len_stream_data];
      label = read_data_from_python(data);

      acc_sample = measure_accuracy(data, label);
      
      acc_sum += acc_sample;
    }
    acc = acc_sum / num_stream_data;
    sprintf(buffer, "Model accuracy: %f %\n", acc*100);
    Serial.print(buffer);
  }        
}