#include "DatabaseManager.h"
#include <iostream>
using namespace std;

DatabaseManager::DatabaseManager()
{
  db = nullptr;
}

DatabaseManager::~DatabaseManager()
{
  closeDatabase();
}

bool DatabaseManager::openDatabase(const string &dbPath)
{
  int result = sqlite3_open(dbPath.c_str(), &db);

  if (result != SQLITE_OK)
  {
    cout << "Error opening database: " << sqlite3_errmsg(db) << endl;
    return false;
  }

  cout << "Database connected successfully." << endl;
  return true;
}

void DatabaseManager::closeDatabase()
{
  if (db != nullptr)
  {
    sqlite3_close(db);
    db = nullptr;
  }
}

bool DatabaseManager::executeQuery(const string &query)
{
  char *errorMessage = nullptr;

  int result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);

  if (result != SQLITE_OK)
  {
    cout << "SQL Error: " << errorMessage << endl;
    sqlite3_free(errorMessage);
    return false;
  }

  return true;
}
sqlite3 *DatabaseManager::getDatabase()
{
  return db;
}

bool DatabaseManager::createTables()
{
  string createCustomersTable =
      "CREATE TABLE IF NOT EXISTS customers ("
      "customer_id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "name TEXT NOT NULL,"
      "email TEXT,"
      "phone TEXT UNIQUE NOT NULL,"
      "address TEXT,"
      "gender TEXT"
      ");";

  string createVehiclesTable =
      "CREATE TABLE IF NOT EXISTS vehicles ("
      "vehicle_id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "type TEXT NOT NULL,"
      "model TEXT NOT NULL,"
      "fuel_type TEXT,"
      "transmission TEXT,"
      "price REAL NOT NULL,"
      "stock INTEGER NOT NULL"
      ");";

  string createSalesTable =
      "CREATE TABLE IF NOT EXISTS sales ("
      "sale_id INTEGER PRIMARY KEY AUTOINCREMENT,"
      "customer_id INTEGER,"
      "vehicle_id INTEGER,"
      "quantity INTEGER NOT NULL,"
      "total_amount REAL NOT NULL,"
      "sale_date TEXT DEFAULT CURRENT_TIMESTAMP,"
      "FOREIGN KEY(customer_id) REFERENCES customers(customer_id),"
      "FOREIGN KEY(vehicle_id) REFERENCES vehicles(vehicle_id)"
      ");";

  return executeQuery(createCustomersTable) &&
         executeQuery(createVehiclesTable) &&
         executeQuery(createSalesTable);
}