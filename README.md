# Automobile Showroom DBMS

A console-based Automobile Showroom Database Management System built using C++ and SQLite. The project manages vehicles, customers, and sales records with proper database storage and input validation.

## Features

### Vehicle Management
- Add new vehicles
- View vehicle inventory
- Update vehicle price and stock
- Delete vehicles
- Search vehicles by model, type, or fuel type

### Customer Management
- Add new customers
- View customer records
- Update customer details
- Delete customers
- Search customers by name, email, or phone number

### Sales Management
- Record vehicle sales
- Validate customer and vehicle IDs before sale
- Check available vehicle stock
- Automatically reduce vehicle stock after sale
- View sales records with customer and vehicle details

### Input Validation
- Prevents invalid numeric input
- Validates vehicle type, fuel type, and transmission
- Validates customer phone number, email, name, and gender
- Prevents duplicate customer phone numbers

## Tech Stack

- C++
- SQLite
- Object-Oriented Programming
- File-based database storage

## Project Structure

```text
AutomobileShowroomDBMS/
│
├── main.cpp
├── sqlite3.c
├── sqlite3.h
├── README.md
├── .gitignore
│
├── database/
│   ├── DatabaseManager.h
│   └── DatabaseManager.cpp
│
├── models/
│   ├── Vehicle.h
│   └── Customer.h
│
├── services/
│   ├── VehicleService.h
│   ├── VehicleService.cpp
│   ├── CustomerService.h
│   ├── CustomerService.cpp
│   ├── SalesService.h
│   └── SalesService.cpp
│
├── utils/
│   └── InputValidator.h
│
└── data/
    └── showroom.db
Database Tables

The project uses three main tables:

Vehicles

Stores vehicle inventory details such as type, model, fuel type, transmission, price, and stock.

Customers

Stores customer details such as name, email, phone number, address, and gender.

Sales

Stores sales transactions and connects customers with purchased vehicles.

How to Compile and Run

First compile SQLite:

gcc -c sqlite3.c -o sqlite3.o

Then compile the project:

g++ main.cpp database\DatabaseManager.cpp services\VehicleService.cpp services\CustomerService.cpp services\SalesService.cpp sqlite3.o -o showroom

Run the program:

showroom.exe
Menu Options
--- Vehicle Management ---
1. Add Vehicle
2. View Vehicles
3. Update Vehicle
4. Delete Vehicle
5. Search Vehicle

--- Customer Management ---
6. Add Customer
7. View Customers
8. Update Customer
9. Delete Customer
10. Search Customer

--- Sales Management ---
11. Record Sale
12. View Sales

13. Exit
Example Workflow
Add a vehicle with price and stock.
Add a customer with valid contact details.
Record a sale using customer ID and vehicle ID.
The system checks stock availability.
After the sale, vehicle stock is reduced automatically.
Sales records can be viewed from the menu.
Learning Outcomes

This project demonstrates:

Object-oriented programming in C++
SQLite database integration
CRUD operations
Table relationships using customer, vehicle, and sales records
Input validation
Modular project structure
Basic inventory and transaction management
Future Improvements
Generate bill/receipt after each sale
Add login system for admin users
Improve foreign key enforcement
Add better formatted reports
Export sales records to file
Add GUI in future