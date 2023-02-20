import tensorflow as tf
import os
import numpy as np
from tensorflow.keras.layers import Conv2D, MaxPooling2D, Input, Flatten, Dense, Dropout
from tensorflow.keras.models import Model
from tensorflow.keras.datasets import mnist


def load_MNIST():
    'Load and normalize MNIST data'
    (x_train, y_train), (x_test, y_test) = mnist.load_data()

    x_train = x_train.astype('float32')
    x_test = x_test.astype('float32')
    x_train = x_train / 255
    x_test = x_test / 255

    x_train = np.expand_dims(x_train, axis=3)
    x_test = np.expand_dims(x_test, axis=3)

    y_train = tf.one_hot(y_train, len(set(y_train)))
    y_test = tf.one_hot(y_test, len(set(y_test)))

    return x_train, y_train, x_test, y_test


def build_model(input_shape):
    input_data = Input(shape=input_shape, name='model_input')
    x = Conv2D(16, (3, 3), activation='relu')(input_data)
    x = Conv2D(16, (3, 3), activation='relu')(x)
    x = MaxPooling2D((2, 2))(x)
    x = Conv2D(32, (3, 3), activation='relu')(x)
    x = Conv2D(32, (3, 3), activation='relu')(x)
    x = MaxPooling2D((2, 2))(x)
    x = Flatten()(x)
    x = Dense(64, activation='relu')(x)
    x = Dropout(0.2)(x)
    x = Dense(32, activation='relu')(x)
    x = Dropout(0.2)(x)
    y = Dense(10, activation='softmax')(x)

    model = Model(inputs=input_data, outputs=y)
    model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=0.0001), loss=tf.keras.losses.CategoricalCrossentropy(), metrics=['accuracy'])
    model.summary()

    return model


def train_model(model, x_train, y_train):
    early_stop = tf.keras.callbacks.EarlyStopping(monitor='val_loss', patience=10, restore_best_weights=True)
    model.fit(x_train, y_train, validation_split=0.2, epochs=25, batch_size=128, callbacks=[early_stop])

    return model


def save_data(model, x_train):
    if not os.path.isdir(os.path.join(os.path.dirname(os.path.realpath(__file__)), "models")):
        os.mkdir(os.path.join(os.path.dirname(os.path.realpath(__file__)), "models"))

    if not os.path.isdir(os.path.join(os.path.dirname(os.path.realpath(__file__)), "data")):
        os.mkdir(os.path.join(os.path.dirname(os.path.realpath(__file__)), "data"))

    model.save(os.path.join(os.path.dirname(os.path.realpath(__file__)), "models", "mnist_model.h5"))

    np.save(os.path.join(os.path.dirname(os.path.realpath(__file__)), "data", "x_train.npy"), x_train)


if __name__ == "__main__":

    x_train, y_train, x_test, y_test = load_MNIST()
    input_shape = (28, 28, 1)
    model = build_model(input_shape)
    model = train_model(model, x_train, y_train)
    metrics = model.evaluate(x_test, y_test)
    print(f"Model accuracy: {metrics[1]*100:.2f}%")
    save_data(model, x_train)
