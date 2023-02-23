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

int8_t* pred_time;
int8_t* pred_eval;
float acc_sample;
float acc_sum;
float acc;


void setup() {
  Serial.begin(115200);
  delay(2500);
  setup_model();
}


void loop() {

  while (!Serial.available());  // Wait for incoming data to be received
  command = Serial.readStringUntil('\n');
  
  if(command.equals("time")) {
    // Measure execution time of the model
    // Wait for incoming data to be received
    while (!Serial.available());

    // Serial.print("MNIST_int8 time measurement;");

    int8_t data[len_stream_data];
    label = read_data_from_python(data);
    
    measure_execution_time(num_iterations, data);
  }
  else if(command.equals("acc")) {
    // Evaluate the accuracy of the model
    // Wait for incoming data to be received
    while (!Serial.available());

    // Serial.print("MNIST_int8 accuracy measurement;");
    
    sprintf(buffer, "Number of test samples: %d;", num_stream_data);
    Serial.print(buffer);

    int8_t data[len_stream_data];
    label = read_data_from_python(data);

    acc_sum = 0;
    acc_sample = measure_accuracy(data, label);
    acc_sum += acc_sample;

    for(int i=1; i<num_stream_data; i++){
      // Wait for incoming data to be received
      while (!Serial.available());

      int8_t data[len_stream_data];
      label = read_data_from_python(data);

      acc_sample = measure_accuracy(data, label);
      
      acc_sum += acc_sample;
    }
    acc = acc_sum / num_stream_data;
    sprintf(buffer, "Model accuracy: %.2f%%\n", acc*100);
    Serial.print(buffer);
  }        
}