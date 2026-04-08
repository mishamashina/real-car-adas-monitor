#include <filesystem>
#include <vector>
#include <map>

#pragma pack(push, 1)

struct OBDRecord
{
    double speed;
    double rpm;
    double throttlePos;
    double coolantTemp;
    double fuelLevel;
    double intakeTemp;
    double label;
};

#pragma pack(pop)

class OBDParser
{
public:
    ~OBDParser();
    static OBDParser create();
    int64_t load(std::filesystem::path filePath);
    OBDRecord getRecord(int64_t pos);
    std::map<std::string, double> getMapLabel();

private:
    OBDParser();
    void initialize();

    std::vector<OBDRecord> m_records;
    std::map<std::string, double> m_mapLabel;
};