import os
import numpy as np
from typing import Tuple, List
from random import shuffle, random


def ReLU(x):
    return np.maximum(x, 0)


def ReLU_derived(dA, Z):
    dZ = np.array(dA, copy=True)
    for index, value in enumerate(Z):
        if value < 0:
            dZ[index] = 0

    return dZ


def get_cost_value(prediction, label):
    m = prediction.shape[0]
    cost = np.sum((prediction - label) ** 2) / m
    return np.squeeze(cost)


def convert_prob_into_class(prediction):
    index = 0
    max_value = 0
    for i, value in enumerate(prediction):
        if max_value < value:
            max_value = value
            index = i

    actual_prediction = np.zeros(3).reshape(3, 1)
    actual_prediction[index] = 1
    return actual_prediction


def get_accuracy(prediction, label):
    actual_prediction = convert_prob_into_class(prediction)
    return (actual_prediction == label).all(axis=0).mean()


class NeuralNetwork:

    def __init__(self, architecture):
        np.random.seed(int(random() * 100))
        number_of_layers = len(architecture)
        params_values = {}

        self.nn_length = len(architecture)
        self.params_values = {}

        for index, layer in enumerate(architecture):
            layer_idx = index + 1
            layer_input_size = layer["input_dim"]
            layer_output_size = layer["output_dim"]

            self.params_values['W' + str(layer_idx)] = np.random.randn(layer_output_size, layer_input_size) * 0.1
            self.params_values['b' + str(layer_idx)] = np.random.randn(layer_output_size, 1) * 0.1

    def single_layer_forward_propagation(self, current_input_layer, weights, bias):
        current_layer_output = np.dot(weights, current_input_layer) + bias
        return ReLU(current_layer_output), current_layer_output

    def full_forward_propagation(self, training_single_example):
        self.memory = {}
        current_layer = training_single_example
        current_layer = current_layer.reshape(7, 1)

        for index in range(self.nn_length):
            layer_idx = index + 1
            self.memory["I" + str(index)] = current_layer;
            previous_layer_input = current_layer

            weights_current = self.params_values["W" + str(layer_idx)]
            bias_current = self.params_values["b" + str(layer_idx)]
            current_layer, current_layer_weights = self.single_layer_forward_propagation(previous_layer_input, weights_current, bias_current)

            self.memory["A" + str(index)] = previous_layer_input
            self.memory["Z" + str(layer_idx)] = current_layer_weights

        return current_layer

    def single_layer_backward_propagation(self, dA_curr, W_curr, Z_curr, A_prev):
        m = A_prev.shape[0]
        dZ_curr = ReLU_derived(dA_curr, Z_curr)
        dW_curr = np.dot(dZ_curr, A_prev.T) / m
        db_curr = np.sum(dZ_curr, axis=1, keepdims=True) / m
        dA_prev = np.dot(W_curr.T, dZ_curr)

        return dA_prev, dW_curr, db_curr

    def full_backward_propagation(self, nn_output, label):
        m = label.shape[0]
        label = label.reshape(nn_output.shape)

        dA_prev = 2 * (nn_output - label) / m  # mean square error derived
        # print("Mean square: ", dA_prev)
        for layer_idx_prev in range(self.nn_length - 1, -1, -1):
            layer_idx_curr = layer_idx_prev + 1

            dA_curr = dA_prev

            activated_weights_previous = self.memory["A" + str(layer_idx_prev)]
            Z_curr = self.memory["Z" + str(layer_idx_curr)]

            W_curr = self.params_values["W" + str(layer_idx_curr)]
            b_curr = self.params_values["b" + str(layer_idx_curr)]

            dA_prev, dW_curr, db_curr = self.single_layer_backward_propagation(dA_curr, W_curr, Z_curr, activated_weights_previous)

            self.grads_values.setdefault("dW" + str(layer_idx_curr), np.zeros(dW_curr.shape))
            self.grads_values.setdefault("db" + str(layer_idx_curr), np.zeros(db_curr.shape))
            self.grads_values["dW" + str(layer_idx_curr)] += dW_curr
            self.grads_values["db" + str(layer_idx_curr)] += db_curr

    def update(self, learning_rate):


        for layer_idx in range(1, 2):
            print(f"Updating we3ights {layer_idx} with:", self.grads_values["dW" + str(layer_idx)])
            print(f"Updating bias {layer_idx} with:", self.grads_values["db" + str(layer_idx)])
            self.params_values["W" + str(1)] += (learning_rate * self.grads_values["dW" + str(1)] * self.memory["I"+str(layer_idx)])
            self.params_values["b" + str(1)] += learning_rate * self.grads_values["db" + str(1)] * self.params_values["b" + str(layer_idx)]


        # with correction for the layer 3 and 4
        for layer_idx in range(2, self.nn_length + 1):
            print(f"Updating we3ights {layer_idx} with:", self.grads_values["dW" + str(layer_idx)])
            print(f"Updating bias {layer_idx} with:", self.grads_values["db" + str(layer_idx)])
            self.params_values["W" + str(layer_idx)] += (learning_rate * self.grads_values["dW" + str(layer_idx)].T * self.memory["A"+str(layer_idx-1)]).T
            self.params_values["b" + str(layer_idx)] += learning_rate * self.grads_values["db" + str(layer_idx)] * self.params_values["b" + str(layer_idx)]

    def train(self, training_data, epochs, learning_rate):
        cost_history = []
        accuracy_history = []

        for _ in range(epochs):
            self.grads_values = {}
            for input_data, label in training_data:
                nn_output = self.full_forward_propagation(input_data)
                cost = get_cost_value(nn_output, label)
                cost_history.append(cost)
                accuracy = get_accuracy(nn_output, label)
                accuracy_history.append(accuracy)
                self.full_backward_propagation(nn_output, label)

            self.update(learning_rate)

    def predict(self, example):
        return self.full_forward_propagation(example)

    def get_model(self):
        return self.params_values


def parse_data() -> List[Tuple[np.array, np.array]]:
    matrix = np.genfromtxt("seeds/seeds_dataset.txt", dtype=np.float32, delimiter='\t')

    data = []
    for i, j in zip(matrix[:, :7], matrix[:, 7:8]):
        output = np.zeros(3).reshape(3, 1)
        index_label = int(j[0])
        output[index_label - 1] = 1
        data.append((i, output))

    return data


def compare_prediction_label(prediction, label):
    max_value_index = np.argmax(prediction)

    predicted = np.zeros_like(label, dtype=np.float32)
    predicted[max_value_index] = 1.0

    return np.all(predicted == label)


def main():
    nn_data = parse_data()
    shuffle(nn_data)

    train_data_size = int(len(nn_data) * 0.8)
    train_data = nn_data[:train_data_size]
    test_data = nn_data[train_data_size:]

    print(train_data[0])
    print(test_data[0])

    nn_architecture = [
        {"input_dim": 7, "output_dim": 14},
        {"input_dim": 14, "output_dim": 10},
        {"input_dim": 10, "output_dim": 3},
    ]

    nn = NeuralNetwork(nn_architecture)
    nn.train(train_data, 100, 0.005)

    tp = 0

    for example, label in test_data:
        prediction = nn.predict(example)

        if compare_prediction_label(prediction, label):
            tp += 1
        #print("Prediction: ", prediction)
        #print("Actual", test_data[0][1])

    print("Accuracy:", tp / len(test_data))


if __name__ == '__main__':
    main()
