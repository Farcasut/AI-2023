import random


class State:

    def __init__(self, matrix, last_move, zero_position):
        self.matrix = matrix
        self.last_move = last_move
        self.zero_position = zero_position
        self.neighbors = []
        if zero_position == (-1, -1):
            self.find_zero()
        self.find_neighbors()

    def find_zero(self):
        for i in range(len(self.matrix)):
            for j in range(len(self.matrix[1])):
                if self.matrix[i][j] == 0:
                    self.zero_position = (i, j)
                    return
        # Something went wrong
        raise ValueError("Something went wrong! The current state dosen't have a 0")

    def find_neighbors(self):
        if self.zero_position[0] - 1 >= 0:
            self.neighbors.append((self.zero_position[0] - 1, self.zero_position[1]))

        if self.zero_position[0] + 1 < len(self.matrix):
            self.neighbors.append((self.zero_position[0] + 1, self.zero_position[1]))

        if self.zero_position[1] - 1 >= 0:
            self.neighbors.append((self.zero_position[0], self.zero_position[1] - 1))

        if self.zero_position[1] + 1 < len(self.matrix[0]):
            self.neighbors.append((self.zero_position[0], self.zero_position[1] + 1))

        if self.last_move in self.neighbors:
            self.neighbors.remove(self.last_move)
        #random.shuffle(self.neighbors)

    def is_final(self):
        cnt = 1
        for i in range(len(self.matrix)):
            for j in range(len(self.matrix[1])):
                if self.matrix[i][j] == cnt:
                    cnt += 1
                elif self.matrix[i][j] == 0:
                    # 0 is valid in any cell, so we can skip it.
                    continue
                else:
                    # if the cnt is not equal to the current value of the cell then the puzzle is not solved yet.
                    return False
        return True

    def transition(self, position):
        new_matrix = [row[:] for row in self.matrix]  # Create a deep copy of the matrix
        new_matrix[self.zero_position[0]][self.zero_position[1]] = new_matrix[position[0]][position[1]]
        new_matrix[position[0]][position[1]] = 0
        return State(new_matrix, zero_position=position, last_move=self.zero_position)
    def display_puzzle(self):
        print("Sliding Puzzle:")
        for row in self.matrix:
            print("|", end="")
            for val in row:
                if val == 0:
                    print("   |", end="")
                else:
                    print(f" {val} |", end="")
            print("\n-------------")
        print("\n")

    def __str__(self):
        return f"Matrix: {self.matrix}\nNeighbors: {self.neighbors}\nZero Position: {self.zero_position}\nLast Move: {self.last_move}"
