import os


model_name = "MNIST_model"

if __name__ == "__main__":

    for input_data in ["float32_input", "int8_input"]:

        with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "models", f"{model_name}_{input_data}.tflite"), 'rb') as f:
            content = f.read().hex()
            result = bytearray.fromhex(content)
            with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "models", f"{model_name}_{input_data}.cpp"), "wb") as w:
                values_in_row = 0
                num_values = 0

                w.write(bytearray('#include "' + model_name + '_' + input_data + '.h"\n'
                                    "\n"
                                    "// We need to keep the data array aligned on"
                                    "some architectures.\n"
                                    "#ifdef __has_attribute\n"
                                    "#define HAVE_ATTRIBUTE(x) __has_attribute(x)\n"
                                    "#else\n"
                                    "#define HAVE_ATTRIBUTE(x) 0\n"
                                    "#endif\n"
                                    "#if HAVE_ATTRIBUTE(aligned) || (defined("
                                    "__GNUC__) && !defined(__clang__))\n"
                                    "#define DATA_ALIGN_ATTRIBUTE __attribute__("
                                    "(aligned(4)))\n"
                                    "#define DATA_ALIGN_ATTRIBUTE __attribute__("
                                    "(aligned(4)))\n"
                                    "#else\n"
                                    "#define DATA_ALIGN_ATTRIBUTE\n"
                                    "#endif\n"
                                    "\n"

                                    "const unsigned char " + model_name + '_' + input_data + "_tflite[]"
                                    " DATA_ALIGN_ATTRIBUTE = {\n    ", 'utf-8'))

                for value in result:
                    num_values += 1
                    values_in_row += 1
                    value = "0x{:02x}".format(value)

                    if values_in_row == 1:
                        w.write(bytearray(value, 'utf-8'))
                    elif values_in_row == 12:
                        w.write(bytearray(", " + str(value) + ",\n    ", 'utf-8'))
                        values_in_row = 0
                    else:
                        w.write(bytearray(', ' + str(value), 'utf-8'))

                w.write(bytearray("};\nconst int " + model_name + '_' + input_data + "_tflite_len = "
                                    + str(num_values) + ";", 'utf-8'))

        with open(os.path.join(os.path.dirname(os.path.realpath(__file__)), "models", f"{model_name}_{input_data}.h"), "w") as f:
            f.write('// This is a standard TensorFlow Lite model file '
                    'that has been converted into a\n'
                    '// C data array, so it can be easily compiled into '
                    'a binary for devices that\n'
                    "// don't have a file system.\n"
                    '\n'
                    '#ifndef TENSORFLOW_LITE_MODEL_DATA_H_\n'
                    '#define TENSORFLOW_LITE_MODEL_DATA_H_\n'
                    '\n'
                    'extern const unsigned char ' + model_name + '_' + input_data + '_tflite[];\n'
                    'extern const int ' + model_name + '_' + input_data + '_tflite_len;\n'
                    '\n'
                    '#endif')
