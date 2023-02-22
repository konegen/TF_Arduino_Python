#include <Arduino.h>
#include <stdint.h>
#include "TF_Lite_exe.h"
#include "Model_evaluation.h"

extern char buffer[50];
extern float start_time;
extern float end_time;
extern float total_time;
extern float mean_time;

extern uint8_t* pred_time;
extern uint8_t* pred_eval;
extern float acc_sample;

int num_of_pred_classes = 10;


void measure_execution_time(int num_iterations, uint8_t* data) {
    // Measure time of x predictions and take the mean
    start_time = millis();
    for(int i=0; i<num_iterations; i++){
      pred_time = model_execute(data);
    }
    end_time = millis();
    total_time = end_time - start_time;
    mean_time = total_time / num_iterations;

    sprintf(buffer, "Start time: %fms;", start_time);
    Serial.print(buffer);
    sprintf(buffer, "End time: %fms;", end_time);
    Serial.print(buffer);
    sprintf(buffer, "Total time: %fms;", total_time);
    Serial.print(buffer);
    sprintf(buffer, "Number of measurements: %d;", num_iterations);
    Serial.print(buffer);
    sprintf(buffer, "Mean inference time: %fs = %fms;", mean_time/1000, mean_time);
    Serial.print(buffer);
    sprintf(buffer, "Mean FPS: %f\n;", 1.0/(mean_time/1000));
    Serial.print(buffer);
}


int measure_accuracy(uint8_t* data, int label) {
  
    pred_eval = model_execute(data);
    // Possible post processing of prediction
    
    // Get predicted class
    uint8_t max_value = pred_eval[0];
    int max_index = 0;

    for (int i = 1; i < num_of_pred_classes; i++) {
        // sprintf(buffer, "i: %d,  pred_eval[i]: %d,  max_value: %d;", i, pred_eval[i], max_value);
        // Serial.print(buffer);
        if (pred_eval[i] > max_value) {
            max_value = pred_eval[i];
            max_index = i;
        }
    }
    
    sprintf(buffer, "Predicted: %d,  True: %d;", max_index, label);
    Serial.print(buffer);

    if (max_index == label) {
        return 1;
    } else {
        return 0;
    }
}