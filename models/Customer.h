#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
using namespace std;

class Customer
{
private:
  string name;
  string email;
  string phone;
  string address;
  string gender;

public:
  Customer() {}

  Customer(string name, string email, string phone, string address, string gender)
  {
    this->name = name;
    this->email = email;
    this->phone = phone;
    this->address = address;
    this->gender = gender;
  }

  string getName()
  {
    return name;
  }

  string getEmail()
  {
    return email;
  }

  string getPhone()
  {
    return phone;
  }

  string getAddress()
  {
    return address;
  }

  string getGender()
  {
    return gender;
  }
};

#endif