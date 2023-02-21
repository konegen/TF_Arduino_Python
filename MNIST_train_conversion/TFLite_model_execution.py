import numpy as np
import os
from sklearn.metrics import confusion_matrix
import tensorflow as tf
from tensorflow.keras.datasets import mnist


model_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "models", "mnist_model.h5")
data_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "data")


# Load data to test model
(_, _), (x_test, y_test) = mnist.load_data()
x_test = x_test.astype(int)

tflite_model_file = os.path.join(os.path.dirname(os.path.realpath(__file__)), "models", "MNIST_model_uint8_input.tflite")


# Load TFLite model and allocate tensors.
with open(tflite_model_file, 'rb') as f:
    tflite_model = f.read()
    
interpreter = tf.lite.Interpreter(model_content=tflite_model)
interpreter.allocate_tensors()

input = interpreter.get_input_details()[0]
output = interpreter.get_output_details()[0]

x_test = np.expand_dims(x_test, 1)
x_test = np.expand_dims(x_test, -1)
x_test = x_test.astype(np.uint8)
print(x_test[0].shape)

# Gather results for the test data
predictions = []
for sample in x_test:
    interpreter.set_tensor(input['index'], sample)
    interpreter.invoke()
    predictions.append(interpreter.get_tensor(output['index']).argmax())

print(sum(1 for a,b in zip(predictions,y_test) if a == b) / len(predictions))

matrix = confusion_matrix(y_test, predictions)
print(matrix)
