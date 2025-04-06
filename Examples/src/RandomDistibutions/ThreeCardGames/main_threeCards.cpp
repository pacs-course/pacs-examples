#include "threeCard.hpp"
#include <iostream>
int
main()
{
  std::mt19937                       m_Generator{std::random_device{}()};
  std::uniform_int_distribution<int> dist(0, 2);
  ThreeCards                         game;
  unsigned                           countWins = 0;
  unsigned                           numTries;
  std::cout << "Enter the coiche (0=Keep, 1=Change): ";
  int decision;
  std::cin >> decision;
  if(decision != 0 && decision != 1)
    {
      std::cout << "Invalid choice. Please enter 0 or 1." << std::endl;
      return 1;
    }
  std::cout << "Enter the number of tries: ";
  std::cin >> numTries;
  if(numTries == 0)
    {
      std::cout << "Invalid number of tries. Please enter a positive integer."
                << std::endl;
      return 1;
    }
  for(unsigned i = 0; i < numTries; ++i)
    {
      game.drawCards();
      auto choice = dist(m_Generator);
      game.setChoiche(static_cast<ThreeCards::Choice>(choice));
      game.tellALoserCard();
      auto result = game.decision(static_cast<ThreeCards::Decision>(
        decision)); // or Change based on your logic
      if(result == ThreeCards::Result::Win)
        {
          ++countWins;
        }
    }
  std::cout << "Total Wins: " << countWins << std::endl;
  std::cout << "Win Rate: " << static_cast<double>(countWins) / numTries * 100
            << "%" << std::endl;
  std::cout << "Total Games: " << numTries << std::endl;
  return 0;
}