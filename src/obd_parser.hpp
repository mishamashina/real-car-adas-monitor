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
    int64_t load(std::filesystem::path filePath);
    OBDRecord getRecord(int64_t pos);

private:
    OBDParser();

    std::vector<OBDRecord> m_records;
};