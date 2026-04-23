#include "obd_parser.hpp"
#include "onnx_classifier.hpp"

#include <iostream>

int main()
{
    OBDParser parser = OBDParser::create();
    int64_t recordsLoaded = parser.load("../data/obd_data.csv");
    
    if (recordsLoaded != -1) {
        std::cout << "[INFO] " << "Загружено записей: " << recordsLoaded << std::endl;
    } else {
        std::cout << "[ERROR] " << "Неверный путь к датасету" << std::endl;

        return -1;
    }

    uint16_t recordCount = 5;
    
    for (uint16_t i = 0; i != recordCount; i++) {
        OBDRecord record = parser.getRecord(i);

        std::cout << "Запись № " << i << std::endl;
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

    ONNXClassifier onnxClassifier = ONNXClassifier::create();
    uint16_t predictionCount = 0;
    float accuracy;

    std::cout << "Запись № - Истинная метка – Предсказание – Уверенность" << std::endl;

    recordCount = 20;

    for (uint16_t i = 0; i != recordCount; i++) {
        OBDRecord record = parser.getRecord(i);

        std::array<float, 6> features = {
            record.speed, 
            record.rpm, 
            record.throttlePos, 
            record.coolantTemp, 
            record.fuelLevel,
            record.intakeTemp
        };

        ClassificationResult classifRes = onnxClassifier.getClassificationResult(features);
        float labelTruth = record.label;
        float labelPrediction = classifRes.label;

        printf(
            "%u %f %f %f \n",
            i,
            labelTruth,
            labelPrediction,
            classifRes.confidence
        );

        if (labelTruth == labelPrediction)
            predictionCount += 1;
    }

    accuracy = ((float)predictionCount / (float)recordCount) * 100;

    std::cout << "Точность = " << accuracy << "%" << std::endl;

    return 0;
}