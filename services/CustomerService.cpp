#include "CustomerService.h"
#include "../utils/InputValidator.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

CustomerService::CustomerService(DatabaseManager &dbManager) : dbManager(dbManager) {}

string CustomerService_toLowerCase(string text)
{
  transform(text.begin(), text.end(), text.begin(), ::tolower);
  return text;
}

bool CustomerService_containsDigit(const string &text)
{
  for (char ch : text)
    if (isdigit(ch))
      return true;
  return false;
}

bool CustomerService_isValidPhone(const string &phone)
{
  if (phone.length() != 10)
    return false;
  for (char ch : phone)
    if (!isdigit(ch))
      return false;
  return true;
}

bool CustomerService_isValidEmail(const string &email)
{
  return email.find('@') != string::npos && email.find('.') != string::npos;
}

bool CustomerService_customerExists(sqlite3 *db, int customerId)
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

bool CustomerService_customerHasSales(sqlite3 *db, int customerId)
{
  const char *query = "SELECT sale_id FROM sales WHERE customer_id = ?;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
    return false;

  sqlite3_bind_int(stmt, 1, customerId);
  bool hasSales = (sqlite3_step(stmt) == SQLITE_ROW);
  sqlite3_finalize(stmt);
  return hasSales;
}

string CustomerService_getValidCustomerName()
{
  string name;
  while (true)
  {
    name = InputValidator::getString("\nEnter customer name: ");
    if (!CustomerService_containsDigit(name))
      return name;
    cout << "Invalid name. Name should not contain numbers.\n";
  }
}

string CustomerService_getValidPhoneNumber()
{
  string phone;
  while (true)
  {
    phone = InputValidator::getString("Enter phone number: ");
    if (CustomerService_isValidPhone(phone))
      return phone;
    cout << "Invalid phone number. Please enter a 10-digit number.\n";
  }
}

string CustomerService_getValidEmailAddress()
{
  string email;
  while (true)
  {
    email = InputValidator::getString("Enter email: ");
    if (CustomerService_isValidEmail(email))
      return email;
    cout << "Invalid email. Please enter a valid email address.\n";
  }
}

string CustomerService_getValidGender()
{
  string gender;
  while (true)
  {
    gender = InputValidator::getString("Enter gender (M/F/Other): ");
    string lowerGender = CustomerService_toLowerCase(gender);

    if (lowerGender == "m" || lowerGender == "male")
      return "Male";
    if (lowerGender == "f" || lowerGender == "female")
      return "Female";
    if (lowerGender == "other")
      return "Other";

    cout << "Invalid gender. Please enter M, F, or Other.\n";
  }
}

void CustomerService::addCustomer()
{
  string name = CustomerService_getValidCustomerName();
  string email = CustomerService_getValidEmailAddress();
  string phone = CustomerService_getValidPhoneNumber();
  string address = InputValidator::getString("Enter address: ");
  string gender = CustomerService_getValidGender();

  const char *query =
      "INSERT INTO customers (name, email, phone, address, gender) VALUES (?, ?, ?, ?, ?);";

  sqlite3_stmt *stmt;
  sqlite3 *db = dbManager.getDatabase();

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "\nFailed to add customer.\n";
    return;
  }

  sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, phone.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, address.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 5, gender.c_str(), -1, SQLITE_TRANSIENT);

  if (sqlite3_step(stmt) == SQLITE_DONE)
    cout << "\nCustomer added successfully.\n";
  else
    cout << "\nFailed to add customer. Phone number may already exist.\n";

  sqlite3_finalize(stmt);
}

void CustomerService::viewCustomers()
{
  const char *query =
      "SELECT customer_id, name, email, phone, address, gender FROM customers;";

  sqlite3_stmt *stmt;
  sqlite3 *db = dbManager.getDatabase();

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "Failed to fetch customers.\n";
    return;
  }

  cout << "\nCustomer Records\n";
  cout << "-------------------------------------------------------------------------------------------------------------\n";
  cout << left << setw(5) << "ID"
       << setw(20) << "Name"
       << setw(30) << "Email"
       << setw(15) << "Phone"
       << setw(25) << "Address"
       << setw(10) << "Gender" << endl;
  cout << "-------------------------------------------------------------------------------------------------------------\n";

  bool found = false;
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    found = true;
    cout << left << setw(5) << sqlite3_column_int(stmt, 0)
         << setw(20) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))
         << setw(30) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2))
         << setw(15) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3))
         << setw(25) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4))
         << setw(10) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5))
         << endl;
  }

  if (!found)
    cout << "No customers found.\n";

  sqlite3_finalize(stmt);
}

void CustomerService::updateCustomer()
{
  sqlite3 *db = dbManager.getDatabase();

  int customerId = InputValidator::getInt("\nEnter customer ID to update: ");

  if (!CustomerService_customerExists(db, customerId))
  {
    cout << "Customer not found.\n";
    return;
  }

  string email = CustomerService_getValidEmailAddress();
  string phone = CustomerService_getValidPhoneNumber();
  string address = InputValidator::getString("Enter new address: ");
  string gender = CustomerService_getValidGender();

  const char *query =
      "UPDATE customers SET email = ?, phone = ?, address = ?, gender = ? "
      "WHERE customer_id = ?;";

  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "\nFailed to update customer.\n";
    return;
  }

  sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, phone.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, address.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 4, gender.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_int(stmt, 5, customerId);

  if (sqlite3_step(stmt) == SQLITE_DONE)
    cout << "\nCustomer updated successfully.\n";
  else
    cout << "\nFailed to update customer. Phone number may already exist.\n";

  sqlite3_finalize(stmt);
}

void CustomerService::deleteCustomer()
{
  sqlite3 *db = dbManager.getDatabase();

  int customerId = InputValidator::getInt("\nEnter customer ID to delete: ");

  if (!CustomerService_customerExists(db, customerId))
  {
    cout << "Customer not found.\n";
    return;
  }

  if (CustomerService_customerHasSales(db, customerId))
  {
    cout << "Cannot delete customer because this customer has sales records.\n";
    return;
  }

  const char *query = "DELETE FROM customers WHERE customer_id = ?;";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "\nFailed to delete customer.\n";
    return;
  }

  sqlite3_bind_int(stmt, 1, customerId);

  if (sqlite3_step(stmt) == SQLITE_DONE)
    cout << "\nCustomer deleted successfully.\n";
  else
    cout << "\nFailed to delete customer.\n";

  sqlite3_finalize(stmt);
}

void CustomerService::searchCustomer()
{
  string keyword = InputValidator::getString("\nEnter customer name/email/phone to search: ");
  string pattern = "%" + keyword + "%";

  const char *query =
      "SELECT customer_id, name, email, phone, address, gender FROM customers "
      "WHERE name LIKE ? OR email LIKE ? OR phone LIKE ?;";

  sqlite3_stmt *stmt;
  sqlite3 *db = dbManager.getDatabase();

  if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK)
  {
    cout << "Failed to search customers.\n";
    return;
  }

  sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, pattern.c_str(), -1, SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 3, pattern.c_str(), -1, SQLITE_TRANSIENT);

  cout << "\nSearch Results\n";
  cout << "-------------------------------------------------------------------------------------------------------------\n";
  cout << left << setw(5) << "ID"
       << setw(20) << "Name"
       << setw(30) << "Email"
       << setw(15) << "Phone"
       << setw(25) << "Address"
       << setw(10) << "Gender" << endl;
  cout << "-------------------------------------------------------------------------------------------------------------\n";

  bool found = false;
  while (sqlite3_step(stmt) == SQLITE_ROW)
  {
    found = true;
    cout << left << setw(5) << sqlite3_column_int(stmt, 0)
         << setw(20) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1))
         << setw(30) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2))
         << setw(15) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3))
         << setw(25) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4))
         << setw(10) << reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5))
         << endl;
  }

  if (!found)
    cout << "No matching customers found.\n";

  sqlite3_finalize(stmt);
}
