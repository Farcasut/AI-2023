import random
from queue import Queue
from state import State

visited_states = []


def dfs(**kwgs):
    cnt = 0
    current_depth = 0
    stack = [(kwgs['state'], current_depth)]
    max_depth = -1
    if kwgs['max_depth'] is not None:
        max_depth = kwgs['max_depth']
    while stack:
        current_state, current_depth = stack.pop()
        if current_depth == max_depth:
            return
        if current_state.is_final():
            print(f"Found final state: {current_state.matrix}\nNo of iterations: {cnt}")
            exit(-1)

        current_state.display_puzzle()
        visited_states.append(current_state.matrix)
        for i in current_state.neighbors:
            new_state = current_state.transition(i)
            new_state.display_puzzle()
            cnt += 1
            if new_state.matrix not in visited_states:
                visited_states.append(new_state.matrix)
                stack.append((new_state, current_depth + 1))


def idfs(state, max_depth):
    for i in range(max_depth):
        dfs(state=state, max_depth=i + 1)


def bfs(**kwgs):
    queue = Queue()
    queue.put(kwgs['state'])

    while not queue.empty():
        current_state = queue.get()

        if current_state.is_final():
            print(f"Found final state: {current_state.matrix}")
            exit(-1)

        current_state.display_puzzle()
        visited_states.append(current_state.matrix)

        for i in current_state.neighbors:
            new_state = current_state.transition(i)
            new_state.display_puzzle()
            if new_state.matrix not in visited_states:
                visited_states.append(new_state.matrix)
                queue.put(new_state)


def main():
    state_matrix = [[8, 6, 7], [2, 5, 4], [4, 0, 3]]
    state = State(state_matrix, (-1, -1), (-1, -1))
    # idfs(state=state, max_depth=100000)
    bfs(state=state)
if __name__ == "__main__":
    main()
