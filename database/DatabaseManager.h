#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include "../sqlite3.h"
#include <string>
using namespace std;

class DatabaseManager
{
private:
  sqlite3 *db;

public:
  DatabaseManager();
  ~DatabaseManager();

  bool openDatabase(const string &dbPath);
  void closeDatabase();

  bool executeQuery(const string &query);
  sqlite3 *getDatabase();

  bool createTables();
};

#endif