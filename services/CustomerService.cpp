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
  {
    if (isdigit(ch))
      return true;
  }
  return false;
}

bool CustomerService_isValidPhone(const string &phone)
{
  if (phone.length() != 10)
    return false;

  for (char ch : phone)
  {
    if (!isdigit(ch))
      return false;
  }

  return true;
}

bool CustomerService_isValidEmail(const string &email)
{
  return email.find('@') != string::npos && email.find('.') != string::npos;
}

bool CustomerService_customerExists(sqlite3 *db, int customerId)
{
  string query = "SELECT customer_id FROM customers WHERE customer_id = " + to_string(customerId) + ";";

  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

  if (result != SQLITE_OK)
    return false;

  bool exists = false;

  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    exists = true;
  }

  sqlite3_finalize(stmt);
  return exists;
}

bool CustomerService_customerHasSales(sqlite3 *db, int customerId)
{
  string query = "SELECT sale_id FROM sales WHERE customer_id = " + to_string(customerId) + ";";

  sqlite3_stmt *stmt;
  int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

  if (result != SQLITE_OK)
    return false;

  bool hasSales = false;

  if (sqlite3_step(stmt) == SQLITE_ROW)
  {
    hasSales = true;
  }

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
    {
      return name;
    }

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
    {
      return phone;
    }

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
    {
      return email;
    }

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
    {
      return "Male";
    }

    if (lowerGender == "f" || lowerGender == "female")
    {
      return "Female";
    }

    if (lowerGender == "other")
    {
      return "Other";
    }

    cout << "Invalid gender. Please enter M, F, or Other.\n";
  }
}

void CustomerService::addCustomer()
{
  string name, email, phone, address, gender;

  name = CustomerService_getValidCustomerName();
  email = CustomerService_getValidEmailAddress();
  phone = CustomerService_getValidPhoneNumber();
  address = InputValidator::getString("Enter address: ");
  gender = CustomerService_getValidGender();

  string query =
      "INSERT INTO customers (name, email, phone, address, gender) VALUES ('" +
      name + "', '" +
      email + "', '" +
      phone + "', '" +
      address + "', '" +
      gender + "');";

  if (dbManager.executeQuery(query))
  {
    cout << "\nCustomer added successfully.\n";
  }
  else
  {
    cout << "\nFailed to add customer. Phone number may already exist.\n";
  }
}

void CustomerService::viewCustomers()
{
  string query = "SELECT customer_id, name, email, phone, address, gender FROM customers;";

  sqlite3_stmt *stmt;
  sqlite3 *db = dbManager.getDatabase();

  int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

  if (result != SQLITE_OK)
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
  {
    cout << "No customers found.\n";
  }

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

  string query =
      "UPDATE customers SET email = '" + email +
      "', phone = '" + phone +
      "', address = '" + address +
      "', gender = '" + gender +
      "' WHERE customer_id = " + to_string(customerId) + ";";

  if (dbManager.executeQuery(query))
  {
    cout << "\nCustomer updated successfully.\n";
  }
  else
  {
    cout << "\nFailed to update customer. Phone number may already exist.\n";
  }
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

  string query = "DELETE FROM customers WHERE customer_id = " + to_string(customerId) + ";";

  if (dbManager.executeQuery(query))
  {
    cout << "\nCustomer deleted successfully.\n";
  }
  else
  {
    cout << "\nFailed to delete customer.\n";
  }
}

void CustomerService::searchCustomer()
{
  string keyword = InputValidator::getString("\nEnter customer name/email/phone to search: ");

  string query =
      "SELECT customer_id, name, email, phone, address, gender FROM customers "
      "WHERE name LIKE '%" +
      keyword + "%' "
                "OR email LIKE '%" +
      keyword + "%' "
                "OR phone LIKE '%" +
      keyword + "%';";

  sqlite3_stmt *stmt;
  sqlite3 *db = dbManager.getDatabase();

  int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

  if (result != SQLITE_OK)
  {
    cout << "Failed to search customers.\n";
    return;
  }

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
  {
    cout << "No matching customers found.\n";
  }

  sqlite3_finalize(stmt);
}