#include "obd_parser.hpp"

#include <iostream>
#include <fstream>
#include <cstring>

OBDParser::OBDParser() 
{
}

OBDParser::~OBDParser()
{
}

OBDParser OBDParser::create()
{
    OBDParser obdParser;

    obdParser.initialize();

    return obdParser;
}

OBDRecord OBDParser::getRecord(int64_t pos)
{
    OBDRecord record;

    try {
        record = m_records.at(pos);
    } catch(const std::out_of_range& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }

    return record;
}

void OBDParser::initialize()
{
    m_fieldBackPos = (sizeof(OBDRecord) / sizeof(double)) - 1;

    int64_t recordsLoaded = load();

    if (recordsLoaded != - 1)
        std::cout << "[INFO] " << "Загружено записей: " << recordsLoaded << std::endl;
}

int64_t OBDParser::load()
{
    std::ifstream dataset(m_filePath);
    std::string row;

    if (!dataset.is_open()) {
        std::cerr << "[ERROR] " << m_filePath << std::endl;

        return -1;
    }

    getline(dataset, row);

    while (dataset.good()) {
        getline(dataset, row);

        OBDRecord record;
        bool recordError = false;
        
        if (row.front() == ',' || row.empty()) {
            std::cerr << "[ERROR] " << row << std::endl;

            continue;
        }

        double *pFieldFront = reinterpret_cast<double *>(&record);
        double *pFieldBack = pFieldFront + m_fieldBackPos;
        double fieldFront = std::stod(row.substr(0, row.find(',')));
        memcpy(pFieldFront, &fieldFront, sizeof(double));

        for (double *pField = pFieldBack; pField != pFieldFront; pField--) {
            size_t pos = row.find_last_of(',');
            std::string str = row.substr(pos + 1);

            if (!str.empty()) {
                double field = std::stod(str);
                memcpy(pField, &field, sizeof(double));
                
                row.erase(pos);
            } else {
                std::cerr << "[ERROR] " << row << std::endl;

                recordError = true;
                break;
            }
        }
        
        if (!recordError)
            m_records.push_back(record);
    }

    dataset.close();

    return m_records.size();
}