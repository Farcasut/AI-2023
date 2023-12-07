import numpy as np
import random

ACTIONS = ['up', 'down', 'left', 'right']

def choose_action(state, epsilon, Q):
    if np.random.random() < epsilon:
        return random.choice(ACTIONS)
    else:
        return ACTIONS[np.argmax(Q[state])]

def get_next_state(state, action, wind_strength, rows, cols):
    i, j = state
    match action:
        case 'up':
            i = max(i - 1 - wind_strength[j], 0)
        case 'down':
            i = min(i + 1 - wind_strength[j], rows - 1)
        case 'left':
            i = max(i - wind_strength[j], 0)
            j = max(j - 1, 0)
        case 'right':
            i = max(i - wind_strength[j], 0)
            j = min(j + 1, cols - 1)
        case _:
            pass
    i = min(max(i, 0), rows - 1)
    return (i, j)

def train(episodes = 5000, alpha = 0.1, gamma = 0.99, epsilon = 0.1, start = (3,0), goal = (3,7), rows = 7, cols = 10, wind_strength = np.array([0, 0, 0, 1, 1, 1, 2, 2, 1, 0])):
    Q = np.zeros((rows, cols, len(ACTIONS)))
    
    for episode in range(episodes):
        state = start
        done = False
        while not done:
            action = choose_action(state, epsilon, Q)
            action_index = ACTIONS.index(action)
            next_state = get_next_state(state, action, wind_strength, rows, cols)

            if next_state == goal:
                done = True
            reward = -1
            Q[state][action_index] = Q[state][action_index] + alpha * (reward + gamma * np.max(Q[next_state]) - Q[state][action_index])
            state = next_state

    return Q

def main():
    rows = 7
    cols = 10
    alpha = 0.1
    gamma = 0.99
    epsilon = 0.1
    episodes = 5000
    Q = train(episodes, alpha, gamma, epsilon)

    optimal_policy = np.array([[ACTIONS[np.argmax(Q[i, j])] for j in range(cols)] for i in range(rows)])
    print(optimal_policy)

if __name__ == '__main__':
    main()
