# Automobile Showroom DBMS

A console-based Automobile Showroom Database Management System built using C++ and SQLite. The project manages vehicles, customers, and sales records with proper database storage, input validation, and SQL injection protection via prepared statements.

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
- Automatically reduce vehicle stock after sale using atomic transactions
- View sales records with customer and vehicle details

### Input Validation
- Prevents invalid numeric input
- Validates vehicle type, fuel type, and transmission
- Validates customer phone number, email, name, and gender
- Prevents duplicate customer phone numbers

### Security
- All database queries use SQLite prepared statements to prevent SQL injection attacks
- Sale recording uses atomic transactions — if the stock update fails, the sale record is rolled back automatically

## Tech Stack

- C++
- SQLite (amalgamation)
- Object-Oriented Programming
- File-based database storage

## Project Structure

```text
AutomobileShowroomDBMS/
│
├── main.cpp
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
```

## Database Tables

The project uses three main tables:

**Vehicles** — stores vehicle inventory details such as type, model, fuel type, transmission, price, and stock.

**Customers** — stores customer details such as name, email, phone number, address, and gender.

**Sales** — stores sales transactions and connects customers with purchased vehicles via foreign keys.

## Dependencies

This project requires the SQLite amalgamation files (`sqlite3.c` and `sqlite3.h`). Download them from:
[https://sqlite.org/download.html](https://sqlite.org/download.html) → *sqlite-amalgamation-*.zip*

Place `sqlite3.c` and `sqlite3.h` in the project root before compiling.

## How to Compile and Run

**Step 1 — Compile SQLite (Windows):**
```bash
gcc -c sqlite3.c -o sqlite3.o
```

**Step 2 — Compile the project:**

Windows:
```bash
g++ main.cpp database\DatabaseManager.cpp services\VehicleService.cpp services\CustomerService.cpp services\SalesService.cpp sqlite3.o -o showroom
```

Linux/Mac:
```bash
g++ main.cpp database/DatabaseManager.cpp services/VehicleService.cpp services/CustomerService.cpp services/SalesService.cpp sqlite3.o -o showroom
```

**Step 3 — Run:**

Windows:
```bash
showroom.exe
```

Linux/Mac:
```bash
./showroom
```

## Menu Options

```
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
```

## Example Workflow

1. Add a vehicle with price and stock.
2. Add a customer with valid contact details.
3. Record a sale using customer ID and vehicle ID.
4. The system checks stock availability.
5. After the sale, vehicle stock is reduced automatically within a transaction.
6. Sales records can be viewed from the menu.

## Learning Outcomes

This project demonstrates:

- Object-oriented programming in C++
- SQLite database integration
- CRUD operations
- Table relationships using customer, vehicle, and sales records
- Input validation
- Modular project structure
- SQL injection prevention using prepared statements
- Atomic database transactions

## Future Improvements

- Generate bill/receipt after each sale
- Add login system for admin users
- Add better formatted reports
- Export sales records to CSV
- Add GUI frontend
