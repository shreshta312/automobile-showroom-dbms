#include "SalesService.h"
#include "../utils/InputValidator.h"

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

SalesService::SalesService(DatabaseManager &dbManager) : dbManager(dbManager) {}

bool SalesService_customerExists(sqlite3 *db, int customerId)
{
  const char *query = "SELECT customer_id FROM customers WHERE customer_id = ?;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_int(stmt, 1, customerId);
  bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
  sqlite3_finalize(stmt);
  return exists;
}

bool SalesService_getVehicleDetails(sqlite3 *db, int vehicleId, double &price, int &stock)
{
  const char *query = "SELECT price, stock FROM vehicles WHERE vehicle_id = ?;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_int(stmt, 1, vehicleId);

  bool found = false;
  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    price = sqlite3_column_double(stmt, 0);
    stock = sqlite3_column_int(stmt, 1);
    found = true;
  }

  sqlite3_finalize(stmt);
  return found;
}

void SalesService::recordSale()
{
  sqlite3 *db = dbManager.getDatabase();

  int customerId = InputValidator::getInt("\nEnter customer ID: ");
  int vehicleId = InputValidator::getInt("Enter vehicle ID: ");
  int quantity = InputValidator::getInt("Enter quantity: ");

  if (quantity <= 0)
  {
    cout << "Quantity must be greater than 0.\n";
    return;
  }

  if (!SalesService_customerExists(db, customerId))
  {
    cout << "Customer not found.\n";
    return;
  }

  double price;
  int stock;

  if (!SalesService_getVehicleDetails(db, vehicleId, price, stock))
  {
    cout << "Vehicle not found.\n";
    return;
  }

  if (quantity > stock)
  {
    cout << "Not enough stock available.\n";
    cout << "Available stock: " << stock << endl;
    return;
  }

  double totalAmount = price * quantity;

  // Use a transaction so both queries succeed or fail together
  if (sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) != SQLITE_OK)
  {
    cout << "\nFailed to start transaction.\n";
    return;
  }

  // Insert sale record
  const char *insertQuery =
      "INSERT INTO sales (customer_id, vehicle_id, quantity, total_amount) VALUES (?, ?, ?, ?);";

  sqlite3_stmt *insertStmt;
  bool success = false;

  if (sqlite3_prepare_v2(db, insertQuery, -1, &insertStmt, nullptr) == SQLITE_OK)
  {
    sqlite3_bind_int(insertStmt, 1, customerId);
    sqlite3_bind_int(insertStmt, 2, vehicleId);
    sqlite3_bind_int(insertStmt, 3, quantity);
    sqlite3_bind_double(insertStmt, 4, totalAmount);

    if (sqlite3_step(insertStmt) == SQLITE_DONE)
    {
      // Update stock
      const char *updateQuery =
          "UPDATE vehicles SET stock = stock - ? WHERE vehicle_id = ?;";

      sqlite3_stmt *updateStmt;

      if (sqlite3_prepare_v2(db, updateQuery, -1, &updateStmt, nullptr) == SQLITE_OK)
      {
        sqlite3_bind_int(updateStmt, 1, quantity);
        sqlite3_bind_int(updateStmt, 2, vehicleId);

        if (sqlite3_step(updateStmt) == SQLITE_DONE)
          success = true;

        sqlite3_finalize(updateStmt);
      }
    }

    sqlite3_finalize(insertStmt);
  }

  if (success)
  {
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    cout << "\nSale recorded successfully.\n";
    cout << "Total amount: " << fixed << setprecision(2) << totalAmount << endl;
  }
  else
  {
    sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
    cout << "\nFailed to record sale.\n";
  }
}

void SalesService::viewSales()
{
  const char *query =
      "SELECT "
      "sales.sale_id, "
      "customers.name, "
      "vehicles.model, "
      "sales.quantity, "
      "sales.total_amount, "
      "sales.sale_date "
      "FROM sales "
      "JOIN customers ON sales.customer_id = customers.customer_id "
      "JOIN vehicles ON sales.vehicle_id = vehicles.vehicle_id;";

  sqlite3_stmt *stmt;
  sqlite3 *db = dbManager.getDatabase();

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "Failed to fetch sales.\n";
    return;
  }

  cout << "\nSales Records\n";
  cout << "-------------------------------------------------------------------------------------------------\n";
  cout << left << setw(8) << "Sale ID"
       << setw(20) << "Customer"
       << setw(25) << "Vehicle"
       << setw(10) << "Qty"
       << setw(15) << "Amount"
       << setw(25) << "Date" << endl;
  cout << "-------------------------------------------------------------------------------------------------\n";

  bool found = false;
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    found = true;
    cout << left << setw(8) << sqlite3_column_int(stmt, 0)
         << setw(20) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))
         << setw(25) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2))
         << setw(10) << sqlite3_column_int(stmt, 3)
         << setw(15) << fixed << setprecision(2) << sqlite3_column_double(stmt, 4)
         << setw(25) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5))
         << endl;
  }

  if (!found)
    cout << "No sales records found.\n";

  sqlite3_finalize(stmt);
}
