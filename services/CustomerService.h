#ifndef CUSTOMER_SERVICE_H
#define CUSTOMER_SERVICE_H

#include "../database/DatabaseManager.h"
#include "../models/Customer.h"

class CustomerService
{
private:
  DatabaseManager &dbManager;

public:
  CustomerService(DatabaseManager &dbManager);

  void addCustomer();
  void viewCustomers();
  void updateCustomer();
  void deleteCustomer();
  void searchCustomer();
};

#endif