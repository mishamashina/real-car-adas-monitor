#include "obd_parser.hpp"

#include <iostream>

int main()
{
    OBDParser parser = OBDParser::create();

    int64_t recordsLoaded = parser.load("../data/obd_data.csv");

    if (recordsLoaded != -1) {
        std::cout << "[INFO] " << "Загружено записей: " << recordsLoaded << std::endl;
    } else {
        std::cout << "[ERROR] " << "Неверный путь" << std::endl;

        return -1;
    }
    
    for (uint16_t i = 0; i != 5; i++) {
        OBDRecord record = parser.getRecord(i);

        std::cout << "[INFO] " << "Запись № " << i << std::endl;
        printf(
            "%f %f %f %f %f %f %f \n", 
            record.speed, 
            record.rpm, 
            record.throttlePos, 
            record.coolantTemp, 
            record.fuelLevel,
            record.intakeTemp,
            record.label
        );
    }

    return 0;
}