import csv

with open("JiaoTan_15min_JL9.csv", "r") as inFile, open("JiaoTan_1hour_JL9.csv", "wb") as outFile:
    reader = csv.reader(inFile)
    writer = csv.writer(outFile)

    count = 0
    skipped = 0
    open_price = 0.0
    high_price = 0.0
    low_price = 0.0
    close_price = 0.0
    volume = 0

    for line in reader:
        skipped += 1
        if skipped <= 13:
            continue

        count += 1
        if count == 5:
            count = 1

        if count == 1:
            time = line[0]
            open_price = float(line[1])
            high_price = float(line[2])
            low_price = float(line[3])
            volume = int(line[5])
            continue

        if float(line[2]) > high_price:
            high_price = float(line[2])
        if float(line[3]) < low_price:
            low_price = float(line[3])

        volume += int(line[5])

        if count == 4:
            close_price = float(line[4])
            writer.writerow([time, open_price, high_price, low_price, close_price, volume])
            