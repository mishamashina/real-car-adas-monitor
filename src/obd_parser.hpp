#include <filesystem>
#include <vector>

#pragma pack(push, 1)

struct OBDRecord
{
    double rpm;
    double speed;
    double throttlePos;
    double coolantTemp;
    double fuelLevel;
};

#pragma pack(pop)

class OBDParser
{
public:
    ~OBDParser();
    static OBDParser create();
    OBDRecord getRecord(int64_t pos);

private:
    OBDParser();
    void initialize();
    int64_t load();

    std::vector<OBDRecord> m_records;
    uint8_t m_fieldBackPos;
    std::filesystem::path m_filePath = "../data/dataset.csv";
};