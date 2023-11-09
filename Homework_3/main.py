import itertools

PLAYER_WON = 1
COMPUTER_WON = 0
DRAW = -1
CONTINUE = 2

PLAYER_TURN = True
COMPUTER_TURN = False


class State:
#    winning_combinations = {i for i in set(itertools.combinations(range(1, 10), 3)) if sum(i) == 15}

    def deep_copy(self):
        new_state = State()
        new_state.available = self.available.copy()
        new_state.turn = self.turn
        new_state.set_player = self.set_player.copy()
        new_state.set_computer = self.set_computer.copy()
        return new_state

    def __init__(self):
        self.available = {i for i in range(1, 10)}
        self.turn = PLAYER_TURN
        self.set_player = set()
        self.set_computer = set()

    def is_final(self):
        for combo in itertools.combinations(self.set_player, 3):
            if sum(combo) == 15:
                return PLAYER_WON 

        for combo in itertools.combinations(self.set_computer, 3):
            if sum(combo) == 15: 
                return COMPUTER_WON

#        if (len(self.set_player) >= 2 and len(self.set_computer) >= 2) and ((15 - sum(self.set_computer)) not in self.available) and ((15 - sum(self.set_player)) not in self.available):
#            return DRAW

        if len(self.available) == 0:
            return DRAW

        return CONTINUE

    def make_move(self, number):

        if 0 >= number or number > 9:
            return False
        if number in self.available:
            self.available.remove(number)
            if self.turn == PLAYER_TURN:
                self.set_player.add(number)
            else:
                self.set_computer.add(number)
            self.turn = not self.turn
            return True
        # something went wrong and we need to redo the move
        return False


def player_turn(state):
    print("PLAYER TURN:")
    print(f"Select a number from the available numbers: {state.available}")
    while True:
        player_number = int(input("Waiting for number: "))
        if state.make_move(player_number):
            break
        print("You have selected an invalid number, please select another one!")


def winning(result):
    if result == PLAYER_WON:
        print("Player won!")
    elif result == COMPUTER_WON:
        print("Computer won!")
    elif result == DRAW:
        print("It's a draw!")
    else:
        print("Invalid result.")

def calculate_winning_potential(state, set):
    score = 0
    for combination in itertools.combinations(set, 2):
        if 15 - sum(combination) in state.available:
            score += 1
    return score
def heuristic(state, game_state):
    if game_state == COMPUTER_WON:
        return float("inf")
    if game_state == PLAYER_WON:
        return float("-inf")

    score_player = calculate_winning_potential(state, state.set_player)
    score_computer = calculate_winning_potential(state, state.set_computer)
    return score_computer - score_player

def minmax(state, depth, type):
    game_state = state.is_final()
    if depth == 0 or game_state == COMPUTER_WON or game_state == PLAYER_WON:
        return heuristic(state, game_state), None
    # MAXIMIZATION
    result = 0
    move = None
    if type == COMPUTER_TURN:
        result = float('-inf')
        for number in state.available:
            new_state = state.deep_copy()
            new_state.make_move(number)
            value, _ = minmax(new_state, depth - 1, not type)
            if result < value:
                result = value
                move = number
    #MIN
    if type == PLAYER_TURN:
        result = float('inf')
        for number in state.available:
            new_state = state.deep_copy()
            new_state.make_move(number)
            value, _ = minmax(new_state, depth - 1, not type)
            if result > value:
                result = value
                move = number

    if move == None:
        for i in state.available:
            move = i
            break

    return result, move

def computer_turn(state, depth, type):
    _, move = minmax(state, depth, type)
    state.make_move(move)


def number_scrabble():
    state = State()
    print("---GAME STARTED---")
    while True:
        player_turn(state)
        ending_condition = state.is_final()
        if ending_condition != CONTINUE:
            print("Player:", state.set_player)
            print("Computer:", state.set_computer)
            winning(ending_condition)
            break
        computer_turn(state, 2, COMPUTER_TURN)
        ending_condition = state.is_final()
        if ending_condition != CONTINUE:
            print("Player:", state.set_player)
            print("Computer:", state.set_computer)
            winning(ending_condition)
            break
        print("Player:", state.set_player)
        print("Computer:", state.set_computer)

def main():
    number_scrabble()


if __name__ == '__main__':
    main()
