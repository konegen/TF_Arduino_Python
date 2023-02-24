import serial
import os
import logging
import sys
from sklearn.utils import shuffle

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
x_test = x_test.astype(int)
# x_test = x_test / 255
# Input data has to be 1D that it can be loaded to the model
x_test = x_test.reshape(x_test.shape[0], -1)


arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)


def write_read_arduino(data, label):
    arduino.flushInput()

    data_str = ""
    data_str += ",".join([str(x) for x in data])
    data_str += f",{label}\n"

    arduino.write(data_str.encode())
    arduino.flush()  # flush the serial buffer to ensure the data is sent

    while(arduino.inWaiting() == 0):  # wait for the data to be available on the serial port before
        pass                          # proceeding to read the data. This ensures that the script
                                      # only reads complete data

    arduino_data = b''
    while True:
        arduino_data += arduino.readline()
        if "\n" in arduino_data.decode("utf-8"):
            break

    arduino_data_split = arduino_data.split(b';')
    for i in range(len(arduino_data_split)):
        arduino_data_decode = arduino_data_split[i].decode("utf-8")
        if arduino_data_decode != "\n":
            if "MNIST" in arduino_data_decode or "Model accuracy" in arduino_data_decode:
                logging.info(arduino_data_decode + "\n")
            elif arduino_data_decode:
                logging.info(arduino_data_decode)
            else:
                pass
            pass


while True:
    command = input("Enter a command: ")  # Taking input from user

    x_test, y_test = shuffle(x_test, y_test)

    if command == 'end':
        print('Finished program')
        break

    if command == 'time':
        # Time measurement
        arduino.write(bytes(command + '\n', 'utf-8'))
        num_iterations = input("Enter a number of iterations to measure time: ") # Taking input from user
        arduino.write(bytes(num_iterations + '\n', 'utf-8'))
        write_read_arduino(x_test[0], y_test[0])

    if command == 'acc':
        # Accuracy measurement
        arduino.write(bytes(command + '\n', 'utf-8'))
        num_data = input("Enter a number of data to use: ") # Taking input from user
        arduino.write(bytes(num_data + '\n', 'utf-8'))
        for i in range(int(num_data)):
            write_read_arduino(x_test[i], y_test[i])
