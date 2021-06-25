/*
 * test_csvReader.cpp
 *
 *  Created on: Jun 24, 2021
 *      Author: forma
 */

#include "readCSV.hpp"
#include <iostream>
#include <fstream>
int main()
{
  using namespace Utility;
  std::ifstream db("test.csv");
  ReadCSV reader;
  // Every record has at least 3 tokens
  reader.setMinTokens(3u);
  // I set to 4 the minumn number of records.
  // If you know the number of records this avoids useless memory reallocation
  reader.setMinRecords(4u);
  // I want verbosity
  reader.setVerbose(true);
  // I skip the first line
  reader.setSkippedLines(1);
  // Strip quoataion marks in text token, i.e. "anc" -> anc
  reader.stripQuotation(true);
  // Read!
  reader.read(db);
  // Write!
  reader.writeAllRecords(std::cout);
}


