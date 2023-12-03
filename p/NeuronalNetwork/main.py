import os.path

import torch
import torch.utils.data
import requests
import NueronalNetwork
import pickle, gzip, numpy

def convert_to_tensors(set, device):
    train, labels = set
    train_tensor = torch.tensor(train, device=device)
    # transform the label from a single digit to a vector of doubles to make it easier to compare with the output of the network using softmax.
    labels_tensor = torch.tensor(labels, device=device)
    return list(zip(train_tensor, labels_tensor))

def get_tensors_for_data(device):
    file_path = "mnist.pkl.gz"

    if not os.path.exists(file_path):
        url = "https://github.com/mnielsen/neural-networks-and-deep-learning/raw/master/data/mnist.pkl.gz"
        with open(file_path, "wb") as fd:
            fd.write(requests.get(url).content)
    with gzip.open(file_path, "rb") as fd:
        train_set, valid_set, test_set = pickle.load(fd, encoding="latin")
        train_set = convert_to_tensors(train_set, device)
        validate_set = convert_to_tensors(valid_set, device)
        test_set = convert_to_tensors(test_set, device)
        return train_set, validate_set, test_set

def main():
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    print("----------", device, "----------")
    print("Network structure: ")
    train_set, validate_set, test_set = get_tensors_for_data(device)
    training_loader = torch.utils.data.DataLoader(train_set, batch_size=100, shuffle=True)
    network = NueronalNetwork.NeuronalNetwork(device)
    print(network)

    optimiser = torch.optim.Adam(network.parameters(), lr=0.005, betas=(0.9, 0.99))
    loss = torch.nn.CrossEntropyLoss()
    network.train_nn(15, train_set, optimiser, loss)
    network.validate_nn(validate_set)
    torch.save(network.state_dict(), "nn.pth")

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main()

