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
int acc_sum;
float acc;


void setup() {
  Serial.begin(115200);
  delay(2500);
  setup_model();
}


void loop() {

  // Wait for incoming data to be received (command for time or accuracy measurement)
  while (!Serial.available());
  
  command = Serial.readStringUntil('\n');
  
  if(command.equals("time")) {
    // Measure execution time of the model

    // Wait for incoming data to be received (Number of iterations for time measurement)
    while (!Serial.available());

    Serial.print("MNIST_int8 time measurement;");

    // Receive information of number of iterations
    num_iterations = Serial.readStringUntil('\n').toInt();

    // Wait for incoming data to be received (Input image and label)
    while (!Serial.available());

    int8_t data[len_stream_data];
    label = read_data_from_python(data);
    
    measure_execution_time(num_iterations, data);
  }
  else if(command.equals("acc")) {
    // Evaluate the accuracy of the model

    // Wait for incoming data to be received (Number of data for accuracy measurement)
    while (!Serial.available());
    
    Serial.print("MNIST_int8 accuracy measurement;");

    // Receive information of number of iterations
    num_stream_data = Serial.readStringUntil('\n').toInt();

    // Wait for incoming data to be received (Input image and label)
    while (!Serial.available());

    int8_t data[len_stream_data];
    label = read_data_from_python(data);
    
    sprintf(buffer, "Number of test samples: %d;", num_stream_data);
    Serial.print(buffer);

    acc_sum = 0;
    acc_sum += measure_accuracy(data, label);

    for(int i=1; i<num_stream_data; i++){
      Serial.print("\n");
    
      // Wait for incoming data to be received (Input image and label)
      while (!Serial.available());

      int8_t data[len_stream_data];
      label = read_data_from_python(data);

      acc_sum += measure_accuracy(data, label);
    }
    acc = (float) acc_sum / num_stream_data;
    sprintf(buffer, "Model accuracy: %d / %d = %.2f%%\n", acc_sum, num_stream_data, acc*100);
    Serial.print(buffer);
  }        
}