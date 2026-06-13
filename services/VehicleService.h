#ifndef VEHICLE_SERVICE_H
#define VEHICLE_SERVICE_H

#include "../database/DatabaseManager.h"
#include "../models/Vehicle.h"

class VehicleService
{
private:
    DatabaseManager &dbManager;

public:
    VehicleService(DatabaseManager &dbManager);

    void addVehicle();
    void viewVehicles();
    void updateVehicle();
    void deleteVehicle();
    void searchVehicle();
};

#endif