import tensorflow as tf
import os
import numpy as np


def representative_dataset():
    """
    To execute full integer quantization the range of all floating-
    point tensors of the model needs to get calibrated or estimated.
    To do this the TensorFlow lite converter needs a representative
    dataset.
    """
    for input_value in x.take(500):
        # Model has only one input so each data point has one element.
        yield [input_value]


# Define path of model
model_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "models", "mnist_model.h5")
data_path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "data")

if __name__ == "__main__":

    # Convert model for float32 and int8 input
    for input_data_int8 in [True, False]:

        # Load model with custom metrics
        model = tf.keras.models.load_model(model_path)

        # Convert TensorFlow model to TFLite model
        converter = tf.lite.TFLiteConverter.from_keras_model(model)

        if input_data_int8:
            x = np.load(os.path.join(data_path, "x_train.npy"))
            x = tf.cast(x, tf.float32)
            x = tf.data.Dataset.from_tensor_slices(x).batch(1)

            converter.representative_dataset = representative_dataset

            # This quantization approach converts all weights to int8 values. 
            # Also the input and output will be converted to 8-bit integer.
            # Otherwise all weights to int8 values. But the input and output
            # still remain 32-bit float.
            converter.optimizations = [tf.lite.Optimize.DEFAULT]
            converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
            converter.inference_input_type = tf.uint8
            converter.inference_output_type = tf.uint8

        tflite_model = converter.convert()

        if input_data_int8:
            open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "models", "MNIST_model_uint8_input.tflite"), "wb").write(tflite_model)
        else:
            open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "models", "MNIST_model_float32_input.tflite"), "wb").write(tflite_model)