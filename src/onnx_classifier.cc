#include "onnx_classifier.hpp"

#include <algorithm>
#include <dlfcn.h>

#include <onnxruntime/onnxruntime_c_api.h>

ONNXClassifier::ONNXClassifier()
{
}

ONNXClassifier::~ONNXClassifier()
{
    m_ortApi->ReleaseMemoryInfo(m_ortMemoryInfo);
    m_ortApi->ReleaseSession(m_ortSession);
    m_ortApi->ReleaseEnv(m_ortEnv);
}

ONNXClassifier ONNXClassifier::create()
{
    ONNXClassifier onnxClassifier;

    onnxClassifier.initialize();

    return onnxClassifier;
}

void ONNXClassifier::initialize()
{
    m_toml = toml::parse_file(m_tomlPath.string());
    m_featuresMean = *(m_toml["mean"].as_array());
    m_featuresStd = *(m_toml["std"].as_array());

    checkPath(m_onnxLibPath);
    checkPath(m_onnxModelPath);

    void *handle = dlopen(m_onnxLibPath.c_str(), RTLD_LAZY);

    typedef const OrtApiBase* (*OrtGetApiBaseF)(void);
    OrtGetApiBaseF OrtGetApiBase = (OrtGetApiBaseF)dlsym(handle, "OrtGetApiBase");
    m_ortApi = OrtGetApiBase()->GetApi(ORT_API_VERSION);

    m_ortStatus = m_ortApi->CreateEnv(
        ORT_LOGGING_LEVEL_WARNING, 
        "0", 
        &m_ortEnv
    );
    m_ortStatus = m_ortApi->CreateSession(
        m_ortEnv,
        m_onnxModelPath.c_str(),
        nullptr,
        &m_ortSession
    );
    m_ortStatus = m_ortApi->CreateMemoryInfo(
        "Cpu",
        OrtArenaAllocator,
        0,
        OrtMemTypeDefault,
        &m_ortMemoryInfo
    );

    dlclose(handle);
}

ClassificationResult ONNXClassifier::getClassificationResult(std::array<float, 6> &features)
{
    OrtValue *input = nullptr;
    OrtValue *output = nullptr;
    float *scores = nullptr;
    
    for (size_t i = 0; i != features.size(); i++) {
        float mean = m_featuresMean[i].as_floating_point()->get();
        float std = m_featuresStd[i].as_floating_point()->get();

        features[i] = (features[i] - mean) / std;
    }

    m_ortStatus = m_ortApi->CreateTensorWithDataAsOrtValue(
        m_ortMemoryInfo,
        features.data(),
        sizeof(features.front()) * features.size(),
        m_featuresShape.data(),
        m_featuresShape.size(),
        ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
        &input
    );

    const std::array<const OrtValue *, 1> inputs = {input};

    m_ortStatus = m_ortApi->Run(
        m_ortSession,
        nullptr,
        m_inputNames.data(),
        inputs.data(),
        m_inputNames.size(),
        m_outputNames.data(),
        m_outputNames.size(),
        &output
    );
    m_ortStatus = m_ortApi->GetTensorMutableData(
        output,
        (void **)&scores
    );
    
    m_ortApi->ReleaseValue(input);
    m_ortApi->ReleaseValue(output);

    ClassificationResult classifRes;

    memset(&classifRes, 0, sizeof(classifRes));

    classifRes.scores = softmax({scores[0], scores[1], scores[2]});
    float *scoreMax = std::max_element(classifRes.scores.begin(), classifRes.scores.end());
    classifRes.confidence = *scoreMax;

    if (scoreMax == classifRes.scores.begin())
        classifRes.label = 0;
    else if (scoreMax == classifRes.scores.end())
        classifRes.label = 2;
    else
        classifRes.label = 1;

    return classifRes;
}

void ONNXClassifier::checkPath(const std::filesystem::path &path)
{
    if (!std::filesystem::exists(path)) {
        std::runtime_error error(
            std::string("[ERROR] Неверный путь ") + path.string()
        );

        throw error;
    }
}

std::array<float, 3> ONNXClassifier::softmax(std::array<float, 3> scores)
{
    float sum = 0;
    float scoreMax = *(std::max_element(scores.begin(), scores.end()));

    for (float &score : scores) {
        score = std::exp(score - scoreMax);
        sum += score;
    }

    for (float &score : scores) {
        score /= sum;
    }

    return scores;
}