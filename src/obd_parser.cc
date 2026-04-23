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

void OBDParser::initialize()
{
    m_mapLabel["SLOW"] = 0;
    m_mapLabel["NORMAL"] = 1;
    m_mapLabel["AGGRESSIVE"] = 2;
}

int64_t OBDParser::load(std::filesystem::path filePath)
{
    std::string row;
    std::ifstream dataset(filePath);
    size_t sizeStart = m_records.size();
    uint8_t fieldBackPos = (sizeof(OBDRecord) / sizeof(float)) - 1;

    if (!dataset.is_open())
        return -1;

    getline(dataset, row);

    while (dataset.good()) {
        getline(dataset, row);

        OBDRecord record;
        bool recordError = false;
        std::string rowCopy = row;

        memset(&record, 0, sizeof(record));
        
        if (row.empty())
            continue;

        if (row.front() == ',') {
            std::cerr << "[ERROR] " << row << std::endl;

            continue;
        }

        float *pFieldFront = reinterpret_cast<float *>(&record);
        float *pFieldBack = pFieldFront + fieldBackPos;
        float fieldFront = std::stod(row.substr(0, row.find(',')));
        memcpy(pFieldFront, &fieldFront, sizeof(float));

        for (float *pField = pFieldBack; pField != pFieldFront; pField--) {
            size_t pos = row.find_last_of(',');
            std::string str = row.substr(pos + 1);

            if (!str.empty()) {
                float field;

                if (pField == pFieldBack)
                    field = m_mapLabel[str];
                else
                    field = std::stod(str);

                memcpy(pField, &field, sizeof(float));
                
                row.erase(pos);
            } else {
                std::cerr << "[ERROR] " << rowCopy << std::endl;

                recordError = true;
                break;
            }
        }
        
        if (!recordError)
            m_records.push_back(record);
    }

    dataset.close();
    
    return m_records.size() - sizeStart;
}

OBDRecord OBDParser::getRecord(int64_t pos)
{
    if (pos >= m_records.size()) {
        std::out_of_range error("[ERROR] Неверный индекс");

        throw error;
    } else {
        OBDRecord record = m_records[pos];

        return record;
    }
}

std::map<std::string, float> OBDParser::getMapLabel()
{
    return m_mapLabel;
}