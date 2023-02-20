#include <TensorFlowLite.h>
#include "TF_Lite_exe.h"
#include "Model_evaluation.h"
#include "Serial_read_python.h"


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
  Serial.begin(9600);
  delay(2500);
  setup_model();
}


void loop() {

  while (!Serial.available());
  Serial.print("MNIST_int8;");
  
  // Measure execution time of the model
  int8_t data[len_stream_data];
  
  label = read_data_from_python(data);
  measure_execution_time(num_iterations, data);


  // Evaluate the accuracy of the model
  while (!Serial.available());
  sprintf(buffer, "Number of samples: %d;", num_stream_data);
  Serial.print(buffer);
  
  acc_sum = 0;

  for(int i=0; i<num_stream_data; i++){
    while (!Serial.available());

    int8_t data[len_stream_data];
    label = read_data_from_python(data);

    acc_sample = measure_accuracy(data, label);
    
    acc_sum += acc_sample;
  }

  acc = acc_sum / num_stream_data;
  sprintf(buffer, "Model accuracy: %f %\n", acc*100);
  Serial.print(buffer);
}