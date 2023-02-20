import serial
import os
import numpy as np
import logging
import sys
import time

import tensorflow as tf
from tensorflow.keras.datasets import mnist


if not os.path.isdir(os.path.join(os.path.dirname(os.path.realpath(__file__)), "time_measurements")):
    os.mkdir(os.path.join(os.path.dirname(os.path.realpath(__file__)), "time_measurements"))

# Define logger
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s -- %(filename)s --> %(message)s', datefmt='%Y/%m/%d %H:%M:%S',
    handlers=[
        logging.FileHandler(os.path.join(os.path.dirname(os.path.realpath(__file__)), "time_measurements", "time_measurement_Arduino_int8.log")),
        logging.StreamHandler(sys.stdout)
    ]
)

# Load data to test model
(_, _), (x_test, y_test) = mnist.load_data()
x_test = x_test.reshape(x_test.shape[0], -1)


arduino = serial.Serial(port='COM4', baudrate=9600, timeout=.1)


def write_read_arduino(data, label, n_iterations, n_data):
    arduino.flushInput()

    data_str = ""
    data_str += ",".join([str(x) for x in data])
    data_str += f",{label}"
    data_str += f",{n_iterations}"
    data_str += f",{n_data}\n"

    arduino.write(data_str.encode())

    while(arduino.inWaiting() == 0):
        pass

    arduino_data = arduino.readline()
    while arduino.inWaiting() > 0:
        # read the remaining data
        arduino_data += arduino.readline()
    arduino_data_split = arduino_data.split(b';')
    for i in range(len(arduino_data_split)):
        arduino_data_decode = arduino_data_split[i].decode("utf-8")
        if "MNIST" in arduino_data_decode or "acc" in arduino_data_decode:
            logging.info(arduino_data_decode + "\n")
        elif arduino_data_decode:
            logging.info(arduino_data_decode)
        else:
            pass


while True:
    # command = input("Enter a command: ")  # Taking input from user
    command = 'start'
    num_iterations = 2
    num_data = 100

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
