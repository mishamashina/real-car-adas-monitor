#include <filesystem>
#include <vector>
#include <map>

#pragma pack(push, 1)

struct OBDRecord
{
    float speed;
    float rpm;
    float throttlePos;
    float coolantTemp;
    float fuelLevel;
    float intakeTemp;
    float label;
};

#pragma pack(pop)

class OBDParser
{
public:
    ~OBDParser();
    static OBDParser create();
    int64_t load(std::filesystem::path filePath);
    OBDRecord getRecord(int64_t pos);
    std::map<std::string, float> getMapLabel();

private:
    OBDParser();
    void initialize();

    std::vector<OBDRecord> m_records;
    std::map<std::string, float> m_mapLabel;
};