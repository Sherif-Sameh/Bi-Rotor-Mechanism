function [gyroReadings,accReadings] = readData(noOfSamples,arduino)
%Read Gyroscope and Accelerometer readings from serial port
%   The values are first read as a low and a high byte since the arduino
%   sends them byte by byte then are reconstructed to a 16 bit signed
%   integer and returned.

gyroReadingsHighByte = zeros(3, noOfSamples);
gyroReadingsHighByte = int8(gyroReadingsHighByte);
gyroReadingsLowByte = zeros(3, noOfSamples);
gyroReadingsLowByte = uint8(gyroReadingsLowByte);

accReadingsHighByte = zeros(3, noOfSamples);
accReadingsHighByte =int8(accReadingsHighByte);
accReadingsLowByte = zeros(3, noOfSamples);
accReadingsLowByte = uint8(accReadingsLowByte);

count = 1; i = 1;
while(count <= noOfSamples)
    if(arduino.NumBytesAvailable > 3)
        gyroReadingsHighByte(i, count) = read(arduino,1,"int8");
        gyroReadingsLowByte(i, count) = read(arduino,1,"uint8");
        accReadingsHighByte(i, count) = read(arduino,1,"int8");
        accReadingsLowByte(i, count) = read(arduino,1,"uint8");
        i = i + 1;
        if(i > 3)
            i = 1;
            count = count + 1;
        end
    end
end

[gyroReadings] = reconstructNumber(gyroReadingsHighByte, gyroReadingsLowByte);
[accReadings] = reconstructNumber(accReadingsHighByte, accReadingsLowByte);
end

