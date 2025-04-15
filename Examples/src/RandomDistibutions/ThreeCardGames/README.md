# A simple representation of the three card game using random distributions

The game consists in showing three cards to the player. Only one card is a winning card. The player selects one card and the dealer shows one of the other two cards corresponding to a loser card. The player can then either stick with their original choice or switch to the other card. The goal is to determine the probability of winning based on the player's strategy and the chosen strategy's effectiveness.

It is well known that the best strategy is to always switch. This is because the probability of winning by switching is 2/3, while the probability of winning by sticking with the original choice is only 1/3. This counterintuitive result is often referred to as the Monty Hall problem.
The code simulates the game and calculates the probabilities of winning based on the player's strategy. It uses random distributions to generate the cards and determine the winning card. The simulation is run multiple times to get a more accurate estimate of the probabilities.

The code used C++ random distributions to shuffle the cards at each game. 

## What do I learn here?
- How to use random distributions to simulate a game
- A use of `std::views::iota` and `std::ranges::find_if`
- A verification of the Monty Hall problem

