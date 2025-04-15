#pragma once
#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <ranges>
/*!
 * Class representing a three-card game with various choices and outcomes.
 * This class allows players to make decisions and track the results of the
 * game. It also includes methods for drawing cards and determining the outcome
 * of the game. The game logic is based on a simple decision-making process.
 *
 */
class ThreeCards
{
public:
  //! Enum to represent the choices available in the game
  enum Choice
  {
    Left = 0,
    Middle = 1,
    Right = 2
  };
  //! Enum to represent the possible outcomes of the game
  enum Result
  {
    Lose = 0,
    Win = 1
  };
  //! Enum to represent the decision made by the player
  enum Decision
  {
    Keep = 0,
    Change = 1
  };
  /*!
   * Default sonstructor for the ThreeCards class.
   * Initializes the random number generator and shuffles the deck of cards.
   * The deck consists of 3 cards, with one winning card and two losing cards.
   * The cards are represented as an array of Result enums.
   */
  ThreeCards() : m_Generator{std::random_device{}()}
  {
    // Initialize the deck with 3 cards
    std::shuffle(m_currentGame.begin(), m_currentGame.end(), m_Generator);
  }
  /*!
  @brief prints the current cards
  */
  void
  printCurrentGame()
  {
    for(auto card : m_currentGame)
      {
        std::cout << card << " ";
      }
    std::cout << std::endl;
  }
  /*!
  @brief Draws cards and returns the current game state
  */
  std::array<Result, 3>
  drawCards()
  {
    std::shuffle(m_currentGame.begin(), m_currentGame.end(), m_Generator);
    return m_currentGame;
  }
  /*!
  @brief Sets the player's choice of card
  @param choice The player's choice (0 = left, 1 = middle, 2 = right)
  */
  void
  setChoiche(Choice choice)
  {
    if(choice < 0 || choice > 2)
      {
        std::cerr << "Invalid choice. Please choose between 0 and 2."
                  << std::endl;
        return;
      }
    this->choice = choice;
  }
  /*!
  @brief Tells the player which card is a loser
  @return The losing card (0 = left, 1 = middle, 2 = right)
  */
  //! Tells the player which card is a loser
  Choice
  tellALoserCard()
  {
    auto pos = std::ranges::find_if(std::views::iota(0, 3), [this](int i) {
      return m_currentGame[i] == 0 && i != choice;
    });
    if(pos == std::views::iota(0, 3).end())
      {
        std::cerr << "No loser card found." << std::endl;
        return static_cast<Choice>(0);
      }
    this->m_losingCard = static_cast<Choice>(*pos);
    return this->m_losingCard;
  }

  /*!
  @brief Makes a decision based on the player's choice and the losing card
  @param decision The player's decision (0 = keep, 1 = change)
  @return The result of the game (0 = lose, 1 = win)
  */
  Result
  decision(Decision decision) const
  {
    if(decision == Decision::Keep)
      {
        return m_currentGame[choice];
      }
    else if(decision == Decision::Change)
      {
        auto pos = std::ranges::find_if(std::views::iota(0, 3), [this](int i) {
          return i != choice && i != this->m_losingCard;
        });
        return m_currentGame[*pos];
      }
    else
      {
        std::cerr << "Invalid decision. Please choose between 0 and 1."
                  << std::endl;
        return Result::Lose;
      }
  }

private:
  std::array<Result, 3> m_currentGame = {Result::Lose, Result::Lose,
                                         Result::Win};
  mutable std::mt19937  m_Generator;
  Choice                choice;       // 0 = left 1 = middle 2 = right
  Choice                m_losingCard; // 0 = left 1 = middle 2 = right
  // 0 = lose 1 = win
};