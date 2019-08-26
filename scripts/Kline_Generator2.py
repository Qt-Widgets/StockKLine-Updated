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

    first_write = True

    for line in reader:
        skipped += 1
        if skipped <= 2:
            continue

        time = line[0][11:] # eg. 10:45

        if time == '09:30' or time == '10:45' or time == '13:45' or time == '14:45' or time == '21:00' or time == '22:00' or time == '23:00':
            if first_write == False:
                writer.writerow([datetime, open_price, high_price, low_price, close_price, volume])
            first_write = False

            datetime = line[0]
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
        close_price = float(line[4])
            