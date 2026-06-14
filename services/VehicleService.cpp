#include "VehicleService.h"
#include "../utils/InputValidator.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

VehicleService::VehicleService(DatabaseManager &dbManager) : dbManager(dbManager) {}

string toLowerCase(string text)
{
  transform(text.begin(), text.end(), text.begin(), ::tolower);
  return text;
}

bool hasAlphabet(const string &text)
{
  for (char ch : text)
  {
    if (isalpha(ch))
      return true;
  }
  return false;
}

bool vehicleExists(sqlite3 *db, int vehicleId)
{
  const char *query = "SELECT vehicle_id FROM vehicles WHERE vehicle_id = ?;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_int(stmt, 1, vehicleId);

  bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
  sqlite3_finalize(stmt);
  return exists;
}

bool vehicleHasSales(sqlite3 *db, int vehicleId)
{
  const char *query = "SELECT sale_id FROM sales WHERE vehicle_id = ?;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_int(stmt, 1, vehicleId);

  bool hasSales = (sqlite3_step(stmt) == SQLITE_ROW);
  sqlite3_finalize(stmt);
  return hasSales;
}

string getValidVehicleType()
{
  string type;
  while (true)
  {
    type = InputValidator::getString("\nEnter vehicle type (Car/Bike/Heavy Vehicle): ");
    string lowerType = toLowerCase(type);

    if (lowerType == "car")
      return "Car";
    if (lowerType == "bike")
      return "Bike";
    if (lowerType == "heavy vehicle" || lowerType == "heavyvehicle")
      return "Heavy Vehicle";

    cout << "Invalid vehicle type. Please enter Car, Bike, or Heavy Vehicle.\n";
  }
}

string getValidFuelType()
{
  string fuelType;
  while (true)
  {
    fuelType = InputValidator::getString("Enter fuel type (Petrol/Diesel/CNG/EV): ");
    string lowerFuel = toLowerCase(fuelType);

    if (lowerFuel == "petrol")
      return "Petrol";
    if (lowerFuel == "diesel")
      return "Diesel";
    if (lowerFuel == "cng")
      return "CNG";
    if (lowerFuel == "ev" || lowerFuel == "electric")
      return "EV";

    cout << "Invalid fuel type. Please enter Petrol, Diesel, CNG, or EV.\n";
  }
}

string getValidTransmission()
{
  string transmission;
  while (true)
  {
    transmission = InputValidator::getString("Enter transmission type (Manual/Automatic): ");
    string lowerTransmission = toLowerCase(transmission);

    if (lowerTransmission == "manual")
      return "Manual";
    if (lowerTransmission == "automatic")
      return "Automatic";

    cout << "Invalid transmission type. Please enter Manual or Automatic.\n";
  }
}

string getValidModelName()
{
  string model;
  while (true)
  {
    model = InputValidator::getString("Enter model name: ");
    if (hasAlphabet(model))
      return model;
    cout << "Invalid model name. Model name should contain letters, not only numbers.\n";
  }
}

double getValidPrice()
{
  double price;
  while (true)
  {
    price = InputValidator::getDouble("Enter price: ");
    if (price > 0)
      return price;
    cout << "Price must be greater than 0.\n";
  }
}

int getValidStock()
{
  int stock;
  while (true)
  {
    stock = InputValidator::getInt("Enter stock quantity: ");
    if (stock >= 0)
      return stock;
    cout << "Stock cannot be negative.\n";
  }
}

void VehicleService::addVehicle()
{
  string type = getValidVehicleType();
  string model = getValidModelName();
  string fuelType = getValidFuelType();
  string transmission = getValidTransmission();
  double price = getValidPrice();
  int stock = getValidStock();

  const char *query =
      "INSERT INTO vehicles (type, model, fuel_type, transmission, price, stock) "
      "VALUES (?, ?, ?, ?, ?, ?);";

  sqlite3_stmt *stmt;
  sqlite3 *db = dbManager.getDatabase();

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "\nFailed to add vehicle.\n";
    return;
  }

  sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, model.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, fuelType.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, transmission.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_double(stmt, 5, price);
  sqlite3_bind_int(stmt, 6, stock);

  if (sqlite3_step(stmt) == SQLITE_DONE)
    cout << "\nVehicle added successfully.\n";
  else
    cout << "\nFailed to add vehicle.\n";

  sqlite3_finalize(stmt);
}

void VehicleService::viewVehicles()
{
  const char *query =
      "SELECT vehicle_id, type, model, fuel_type, transmission, price, stock FROM vehicles;";

  sqlite3_stmt *stmt;
  sqlite3 *db = dbManager.getDatabase();

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "Failed to fetch vehicles.\n";
    return;
  }

  cout << "\nVehicle Inventory\n";
  cout << "---------------------------------------------------------------------------------------------\n";
  cout << left << setw(5) << "ID"
       << setw(15) << "Type"
       << setw(25) << "Model"
       << setw(15) << "Fuel"
       << setw(15) << "Transmission"
       << setw(12) << "Price"
       << setw(8) << "Stock" << endl;
  cout << "---------------------------------------------------------------------------------------------\n";

  bool found = false;
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    found = true;
    cout << left << setw(5) << sqlite3_column_int(stmt, 0)
         << setw(15) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))
         << setw(25) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2))
         << setw(15) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3))
         << setw(15) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4))
         << setw(12) << fixed << setprecision(2) << sqlite3_column_double(stmt, 5)
         << setw(8) << sqlite3_column_int(stmt, 6)
         << endl;
  }

  if (!found)
    cout << "No vehicles found in inventory.\n";

  sqlite3_finalize(stmt);
}

void VehicleService::updateVehicle()
{
  sqlite3 *db = dbManager.getDatabase();

  int vehicleId = InputValidator::getInt("\nEnter vehicle ID to update: ");

  if (!vehicleExists(db, vehicleId))
  {
    cout << "Vehicle not found.\n";
    return;
  }

  double newPrice = getValidPrice();
  int newStock = getValidStock();

  const char *query =
      "UPDATE vehicles SET price = ?, stock = ? WHERE vehicle_id = ?;";

  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "\nFailed to update vehicle.\n";
    return;
  }

  sqlite3_bind_double(stmt, 1, newPrice);
  sqlite3_bind_int(stmt, 2, newStock);
  sqlite3_bind_int(stmt, 3, vehicleId);

  if (sqlite3_step(stmt) == SQLITE_DONE)
    cout << "\nVehicle updated successfully.\n";
  else
    cout << "\nFailed to update vehicle.\n";

  sqlite3_finalize(stmt);
}

void VehicleService::deleteVehicle()
{
  sqlite3 *db = dbManager.getDatabase();

  int vehicleId = InputValidator::getInt("\nEnter vehicle ID to delete: ");

  if (!vehicleExists(db, vehicleId))
  {
    cout << "Vehicle not found.\n";
    return;
  }

  if (vehicleHasSales(db, vehicleId))
  {
    cout << "Cannot delete vehicle because this vehicle has sales records.\n";
    return;
  }

  const char *query = "DELETE FROM vehicles WHERE vehicle_id = ?;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "\nFailed to delete vehicle.\n";
    return;
  }

  sqlite3_bind_int(stmt, 1, vehicleId);

  if (sqlite3_step(stmt) == SQLITE_DONE)
    cout << "\nVehicle deleted successfully.\n";
  else
    cout << "\nFailed to delete vehicle.\n";

  sqlite3_finalize(stmt);
}

void VehicleService::searchVehicle()
{
  string keyword = InputValidator::getString("\nEnter vehicle model/type/fuel to search: ");
  string pattern = "%" + keyword + "%";

  const char *query =
      "SELECT vehicle_id, type, model, fuel_type, transmission, price, stock FROM vehicles "
      "WHERE type LIKE ? OR model LIKE ? OR fuel_type LIKE ?;";

  sqlite3_stmt *stmt;
  sqlite3 *db = dbManager.getDatabase();

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "Failed to search vehicles.\n";
    return;
  }

  sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, pattern.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, pattern.c_str(), -1, SQLITE_TRANSIENT);

  cout << "\nSearch Results\n";
  cout << "---------------------------------------------------------------------------------------------\n";
  cout << left << setw(5) << "ID"
       << setw(15) << "Type"
       << setw(25) << "Model"
       << setw(15) << "Fuel"
       << setw(15) << "Transmission"
       << setw(12) << "Price"
       << setw(8) << "Stock" << endl;
  cout << "---------------------------------------------------------------------------------------------\n";

  bool found = false;
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    found = true;
    cout << left << setw(5) << sqlite3_column_int(stmt, 0)
         << setw(15) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))
         << setw(25) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2))
         << setw(15) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3))
         << setw(15) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4))
         << setw(12) << fixed << setprecision(2) << sqlite3_column_double(stmt, 5)
         << setw(8) << sqlite3_column_int(stmt, 6)
         << endl;
  }

  if (!found)
    cout << "No matching vehicles found.\n";

  sqlite3_finalize(stmt);
}
