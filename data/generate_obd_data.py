import csv

class Headers:
    speed = "SPEED"
    rpm = "RPM"
    throttlePos = "THROTTLE_POS"
    coolantTemp = "COOLANT_TEMP"
    fuelLevel = "FUEL_LEVEL"
    intakeTemp = "INTAKE_TEMP"
    label = "LABEL"

class Fields:
    speed = list()
    rpm = list()
    throttlePos = list()
    coolantTemp = list()
    fuelLevel = list()
    intakeTemp = list()
    label = list()

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
            fields.speed.append(row[headers.speed])
            fields.rpm.append(row[headers.rpm])
            fields.throttlePos.append(row[headers.throttlePos])
            fields.coolantTemp.append(row[headers.coolantTemp])
            fields.fuelLevel.append(row[headers.fuelLevel])
            fields.intakeTemp.append(row[headers.intakeTemp])

            acceleratorPos = float(row["ACCELERATOR_POS_D"])
            
            if acceleratorPos < 25:
                fields.label.append("GENTLE")
            elif acceleratorPos < 50:
                fields.label.append("NORMAL")
            else:
                fields.label.append("VIOLENT")

            rowAppendedCount += 1
        else:
            break
    
    datasetOrigin.close()

with open("obd_data.csv", "w", newline='') as dataset:
    fieldNames = [
        headers.speed,
        headers.rpm,
        headers.throttlePos,
        headers.coolantTemp,
        headers.fuelLevel,
        headers.intakeTemp,
        headers.label
    ]

    writer = csv.DictWriter(dataset, fieldNames, lineterminator="\n")
    writer.writeheader()
    
    for i in range(0, rowAppendCount):
        writer.writerow({
            headers.speed: fields.speed[i],
            headers.rpm: fields.rpm[i],
            headers.throttlePos: fields.throttlePos[i],
            headers.coolantTemp: fields.coolantTemp[i],
            headers.fuelLevel: fields.fuelLevel[i],
            headers.intakeTemp: fields.intakeTemp[i],
            headers.label: fields.label[i],
        })

    dataset.close()