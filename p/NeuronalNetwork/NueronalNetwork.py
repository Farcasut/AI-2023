import torch

class NeuronalNetwork(torch.nn.Module):
    def __init__(self, device):
        super().__init__()
        self.to(device=device)
        self.layer1 = torch.nn.Linear(28 ** 2, 128, device=device)
        self.layer1_activation = torch.nn.ReLU()
        self.layer2 = torch.nn.Linear(128, 64, device=device)
        self.layer2_activation = torch.nn.ReLU()
        self.layer3 = torch.nn.Linear(64, 10, device=device)
        self.layer3_activation = torch.nn.Softmax(dim=0)
        self.expected_outputs = {digit: torch.tensor([float(0) if i != digit else float(1) for i in range(10)], device=device) for digit in range(10)}

    def forward(self, inp):
        x = self.layer1_activation(self.layer1(inp))
        x = self.layer2_activation(self.layer2(x))
        x = self.layer3_activation(self.layer3(x))
        return x

    def train_nn(self, epochs, data_set, optimiser, loss):
        for i in range(epochs):
            epoch_correct_predictions = 0
            epoch_total_predictions = 0
            optimiser.zero_grad()
            for j, data in enumerate(data_set):
                inp, label = data
                optimiser.zero_grad()
                output = self(inp)
                o_index = torch.argmax(output).item()
                actual_label = label.item()
                if o_index == actual_label:
                    epoch_correct_predictions += 1
                epoch_total_predictions += 1
                l = loss(output, label)
                l.backward()
                optimiser.step()
            print(f"Epoch[{i+1}/{epochs}], Accuracy: {epoch_correct_predictions/epoch_total_predictions}")
            if epoch_correct_predictions/epoch_total_predictions > 0.95:
                return

    def validate_nn(self, data_set):
        true_positives = 0
        true_negatives = 0
        for inp, label in data_set:
            output = self(inp)
            predicted_label = torch.argmax(output).item()
            actual_label = label.item()
            if predicted_label == actual_label:
                true_positives += 1
            else:
                true_negatives += 1

        print("Accuracy at validation:", true_positives/(true_positives+true_negatives))