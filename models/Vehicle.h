#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
using namespace std;

class Vehicle
{
private:
  string type;
  string model;
  string fuelType;
  string transmission;
  double price;
  int stock;

public:
  Vehicle() {}

  Vehicle(string type, string model, string fuelType, string transmission, double price, int stock)
  {
    this->type = type;
    this->model = model;
    this->fuelType = fuelType;
    this->transmission = transmission;
    this->price = price;
    this->stock = stock;
  }

  string getType()
  {
    return type;
  }

  string getModel()
  {
    return model;
  }

  string getFuelType()
  {
    return fuelType;
  }

  string getTransmission()
  {
    return transmission;
  }

  double getPrice()
  {
    return price;
  }

  int getStock()
  {
    return stock;
  }
};

#endif