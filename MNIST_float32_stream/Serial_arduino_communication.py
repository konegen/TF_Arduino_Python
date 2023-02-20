import serial
import os
import numpy as np
import logging
import sys

import tensorflow as tf
from tensorflow.keras.datasets import mnist


if not os.path.isdir(os.path.join(os.path.dirname(os.path.realpath(__file__)), "time_measurements")):
    os.mkdir(os.path.join(os.path.dirname(os.path.realpath(__file__)), "time_measurements"))

# Define logger
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s -- %(filename)s --> %(message)s', datefmt='%Y/%m/%d %H:%M:%S',
    handlers=[
        logging.FileHandler(os.path.join(os.path.dirname(os.path.realpath(__file__)), "time_measurements", "time_measurement_Arduino_float32.log")),
        logging.StreamHandler(sys.stdout)
    ]
)

# Load data to test model
(_, _), (x_test, y_test) = mnist.load_data()

x_test = x_test.astype('float32')
# Input data has to be 1D that it can be loaded to the model
x_test = x_test.reshape(x_test.shape[0], -1)
x_test = x_test / 255


arduino = serial.Serial(port='COM4', baudrate=9600, timeout=.1)


def write_read_arduino(data, label, n_iterations, n_data):
    arduino.flushInput()

    index = 0
    chunk_size = 100
    while index < len(data):
        data_str = ""
        if index+chunk_size < len(data):
            data_str += ",".join([str(x) for x in data[index:index+chunk_size]])
            data_str += ","
        else:
            data_str += ",".join([str(x) for x in data[index:index+chunk_size]])
            data_str += f",{label}"
            data_str += f",{n_iterations}"
            data_str += f",{n_data}"
            data_str += ",\n"
        index += chunk_size
        arduino.write(data_str.encode())
    # data_str += ",".join([str(x) for x in data])
    # data_str += f",{label}"
    # data_str += f",{n_iterations}"
    # data_str += f",{n_data}"
    # data_str += ",\n"


    while(arduino.inWaiting() == 0):
        pass

    arduino_data = arduino.readline()
    arduino_data_split = arduino_data.split(b';')
    for i in range(len(arduino_data_split)):
        if "MNIST" in arduino_data_split[i].decode("utf-8") or "acc" in arduino_data_split[i].decode("utf-8"):
            logging.info(arduino_data_split[i].decode("utf-8") + "\n")
        elif arduino_data_split[i].decode("utf-8"):
            logging.info(arduino_data_split[i].decode("utf-8"))
        else:
            pass


while True:
    # command = input("Enter a command: ")  # Taking input from user
    command = 'start'
    num_iterations = 2
    num_data = 3

    if command == 'end':
        print('Finished program')
        break

    if command == 'start':
        # num_iterations = int(input("Enter a number of iterations to measure time: ")) # Taking input from user
        # num_data = int(input("Enter a number of data to use: ")) # Taking input from user
        # Time measurement
        write_read_arduino(x_test[0], y_test[0], num_iterations, num_data)
        # Accuracy measurement
        for i in range(num_data):
            write_read_arduino(x_test[i], y_test[i], num_iterations, num_data)
