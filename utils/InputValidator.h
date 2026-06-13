#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <iostream>
#include <limits>
#include <string>
using namespace std;

class InputValidator
{
public:
  static int getInt(string prompt)
  {
    int value;

    while (true)
    {
      cout << prompt;
      cin >> value;

      if (!cin.fail())
      {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
      }

      cout << "Invalid input. Please enter a valid integer.\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }

  static double getDouble(string prompt)
  {
    double value;

    while (true)
    {
      cout << prompt;
      cin >> value;

      if (!cin.fail())
      {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return value;
      }

      cout << "Invalid input. Please enter a valid number.\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
  }

  static string getString(string prompt)
  {
    string value;

    while (true)
    {
      cout << prompt;
      getline(cin, value);

      if (!value.empty())
      {
        return value;
      }

      cout << "Input cannot be empty. Please try again.\n";
    }
  }
};

#endif