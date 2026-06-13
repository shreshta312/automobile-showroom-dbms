#include <iostream>

#include "database/DatabaseManager.h"
#include "services/VehicleService.h"
#include "services/CustomerService.h"
#include "services/SalesService.h"
#include "utils/InputValidator.h"

using namespace std;

int main()
{
  DatabaseManager db;

  if (!db.openDatabase("data/showroom.db"))
  {
    cout << "Failed to connect to database.\n";
    return 1;
  }

  if (!db.createTables())
  {
    cout << "Failed to create tables.\n";
    return 1;
  }

  VehicleService vehicleService(db);
  CustomerService customerService(db);
  SalesService salesService(db);

  int choice;

  do
  {
    cout << "\n========== Automobile Showroom DBMS ==========\n";

    cout << "\n--- Vehicle Management ---\n";
    cout << "1. Add Vehicle\n";
    cout << "2. View Vehicles\n";
    cout << "3. Update Vehicle\n";
    cout << "4. Delete Vehicle\n";
    cout << "5. Search Vehicle\n";

    cout << "\n--- Customer Management ---\n";
    cout << "6. Add Customer\n";
    cout << "7. View Customers\n";
    cout << "8. Update Customer\n";
    cout << "9. Delete Customer\n";
    cout << "10. Search Customer\n";

    cout << "\n--- Sales Management ---\n";
    cout << "11. Record Sale\n";
    cout << "12. View Sales\n";

    cout << "\n13. Exit\n";

    choice = InputValidator::getInt("Enter your choice: ");

    switch (choice)
    {
    case 1:
      vehicleService.addVehicle();
      break;

    case 2:
      vehicleService.viewVehicles();
      break;

    case 3:
      vehicleService.updateVehicle();
      break;

    case 4:
      vehicleService.deleteVehicle();
      break;

    case 5:
      vehicleService.searchVehicle();
      break;

    case 6:
      customerService.addCustomer();
      break;

    case 7:
      customerService.viewCustomers();
      break;

    case 8:
      customerService.updateCustomer();
      break;

    case 9:
      customerService.deleteCustomer();
      break;

    case 10:
      customerService.searchCustomer();
      break;

    case 11:
      salesService.recordSale();
      break;

    case 12:
      salesService.viewSales();
      break;

    case 13:
      cout << "Exiting program.\n";
      break;

    default:
      cout << "Invalid choice. Try again.\n";
    }

  } while (choice != 13);

  return 0;
}