#include "obd_parser.hpp"

#include <iostream>

int main()
{
    OBDParser parser = OBDParser::create();

    int64_t recordsLoaded = parser.load("../data/dataset.csv");

    if (recordsLoaded != -1)
        std::cout << "[INFO] " << "Загружено записей: " << recordsLoaded << std::endl;
    else
        std::cout << "[ERROR] " << "Неверный путь" << std::endl;

    for (uint16_t i = 0; i != 5; i++) {
        OBDRecord record = parser.getRecord(i);

        std::cout << "[INFO] " << "Запись № " << i << std::endl;
        printf("%f %f %f %f %f \n", record.rpm, record.speed, record.throttlePos, record.coolantTemp, record.fuelLevel);
    }

    return 0;
}