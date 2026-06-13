#ifndef SALES_SERVICE_H
#define SALES_SERVICE_H

#include "../database/DatabaseManager.h"

class SalesService
{
private:
  DatabaseManager &dbManager;

public:
  SalesService(DatabaseManager &dbManager);

  void recordSale();
  void viewSales();
  void updateVehicle();
};

#endif