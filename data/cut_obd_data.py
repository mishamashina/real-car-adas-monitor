import csv

class Headers:
    rpm = "RPM"
    speed = "SPEED"
    throttlePos = "THROTTLE_POS"
    coolantTemp = "COOLANT_TEMP"
    fuelLevel = "FUEL_LEVEL"

class Fields:
    rpm = list()
    speed = list()
    throttlePos = list()
    coolantTemp = list()
    fuelLevel = list()

headers = Headers()
fields = Fields()

rowSkipCount = 99999
rowAppendCount = 5000

with open("dataset_origin.csv", newline='') as datasetOrigin:
    reader = csv.DictReader(datasetOrigin)
    rowSkipedCount = 0
    rowAppendedCount = 0
    
    for row in reader:
        if rowSkipedCount < rowSkipCount:
            rowSkipedCount += 1

            continue
            
        if rowAppendedCount < rowAppendCount:
            fields.rpm.append(row[headers.rpm])
            fields.speed.append(row[headers.speed])
            fields.throttlePos.append(row[headers.throttlePos])
            fields.coolantTemp.append(row[headers.coolantTemp])
            fields.fuelLevel.append(row[headers.fuelLevel])
            
            rowAppendedCount += 1
        else:
            break
    
    datasetOrigin.close()

with open("dataset.csv", "w", newline='') as dataset:
    fieldNames = [
        headers.rpm,
        headers.speed,
        headers.throttlePos,
        headers.coolantTemp,
        headers.fuelLevel
    ]

    writer = csv.DictWriter(dataset, fieldNames, lineterminator="\n")
    writer.writeheader()
    
    for i in range(0, rowAppendCount):
        writer.writerow({
            headers.rpm: fields.rpm[i],
            headers.speed: fields.speed[i],
            headers.throttlePos: fields.throttlePos[i],
            headers.coolantTemp: fields.coolantTemp[i],
            headers.fuelLevel: fields.fuelLevel[i],
        })

    dataset.close()