#include <array>
#include <filesystem>
#include <stdint.h>

#include <toml++/toml.hpp>

struct OrtApi;
struct OrtStatus;
struct OrtEnv;
struct OrtSession;
struct OrtMemoryInfo;
struct OrtValue;

struct ClassificationResult 
{
    float label;
    float confidence;
    std::array<float, 3> scores;  
};

class ONNXClassifier
{
public:
    ~ONNXClassifier();
    static ONNXClassifier create();
    ClassificationResult getClassificationResult(std::array<float, 6> &features);
    
private:
    ONNXClassifier();
    void initialize();
    void checkPath(const std::filesystem::path &path);
    std::array<float, 3> softmax(std::array<float, 3> scores);

    const OrtApi *m_ortApi = nullptr;
    OrtStatus *m_ortStatus = nullptr;
    OrtEnv *m_ortEnv = nullptr;
    OrtSession *m_ortSession = nullptr;
    OrtMemoryInfo *m_ortMemoryInfo = nullptr;

    toml::table m_toml;
    std::filesystem::path m_tomlPath = "../models/normalization_params.toml";
    std::filesystem::path m_onnxLibPath = "/usr/local/lib64/libonnxruntime.so";
    std::filesystem::path m_onnxModelPath = "../models/driver_classifier.onnx";

    toml::array m_featuresMean;
    toml::array m_featuresStd;
    const std::array<int64_t, 1> m_featuresShape = {6};
    const std::array<const char *, 1> m_inputNames = {"features"};
    const std::array<const char *, 1> m_outputNames = {"class_scores"};
};