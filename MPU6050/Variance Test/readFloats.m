function [gyro,acc] = readFloats(noOfSamples,arduino)
%readFloats
%   When called this function will read serially the sent data in bytes and
%   reconstruct the numbers back into floats.

gyro = single(zeros(3,noOfSamples));
acc = single(zeros(3,noOfSamples));

gyroByteArray = uint8(zeros(12,noOfSamples));
accByteArray = uint8(zeros(12,noOfSamples));

% Read theta and omega as byte arrays:
for sampleCount = 1:noOfSamples
    while( arduino.NumBytesAvailable() < 12)
        % Just wait for Gyro readings, do nothing.
    end
    gyroByteArray(1:12, sampleCount) = read(arduino, 12, "uint8");
    while( arduino.NumBytesAvailable() < 12)
        % Just wait for Accelerometer readings, do nothing.
    end
    accByteArray(1:12, sampleCount) = read(arduino, 12, "uint8");
end

% Convert to Floats

for i = 1:noOfSamples
    gyro(1,i) = typecast(gyroByteArray(1:4,i), 'single');
    gyro(2,i) = typecast(gyroByteArray(5:8,i), 'single');
    gyro(3,i) = typecast(gyroByteArray(9:12,i), 'single');
    acc(1,i) = typecast(accByteArray(1:4,i), 'single');
    acc(2,i) = typecast(accByteArray(5:8,i), 'single');
    acc(3,i) = typecast(accByteArray(9:12,i), 'single');
end

