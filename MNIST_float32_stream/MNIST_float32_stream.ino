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

float* pred_time;
float* pred_eval;
int acc_sum;
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
    Serial.print("MNIST_float32 time measurement;");

    // Wait for incoming data to be received
    while (!Serial.available());

    float data[len_stream_data];
    label = read_data_from_python(data);

    measure_execution_time(num_iterations, data);
  }
  else if(command.equals("acc")) {
    // Evaluate the accuracy of the model
    Serial.print("MNIST_float32 accuracy measurement;");

    // Wait for incoming data to be received
    while (!Serial.available());

    float data[len_stream_data];
    label = read_data_from_python(data);

    sprintf(buffer, "Number of test samples: %d;", num_stream_data);
    Serial.print(buffer);

    acc_sum = 0;
    acc_sum +=  measure_accuracy(data, label);
    
    for(int i=1; i<num_stream_data; i++){
      Serial.print("\n");
      while (!Serial.available());

      float data[len_stream_data];
      label = read_data_from_python(data);

      acc_sum += measure_accuracy(data, label);
    }
    acc = (float) acc_sum / num_stream_data;
    sprintf(buffer, "Model accuracy: %d / %d = %.2f%%\n", acc_sum, num_stream_data, acc*100);
    Serial.print(buffer);
  }
}