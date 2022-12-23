%{ 
 This script was written to capture the raw response of the 3 Gyroscopes in
 the MPU6050 and to calculate the proper bias factors that correct the
 constant biases present in the sensor's readings.

Test Conditions:
- Gyroscope Scale Factor = 131 LSB/dps
- MPU6050 Sampling Rate = 1KHz
- Actual Sampling Rate used = 100Hz
- DLPF Bandwidth = 42Hz (4.8ms delay)

%}

%% Establishing Serial Communication with the Arduino

arduino = serialport("COM3",115200);
configureTerminator(arduino,"CR/LF");
flush(arduino); % Flush serial buffer from any old data.
pause(4); % Wait for serial comm. to be established.

%% First set of data:

noOfSamples = 501;
Fs = 100; % 100Hz
Ts = 1/Fs;
t = (0:1:(noOfSamples-1))*Ts;

write(arduino,1,"uint8"); % Gives the signal to start sampling the IMU.
[gyroReadings1,~] = readData(noOfSamples,arduino);
flush(arduino);

%% Plot the first set and compute its mean:

tiledlayout(3,1);
nexttile
plot(t,gyroReadings1(1,1:noOfSamples));
title('X-axis Gyroscope Readings (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings1(2,1:noOfSamples));
title('Y-axis Gyroscope Readings (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings1(3,1:noOfSamples));
title('Z-axis Gyroscope Readings (LSBs) vs Time (sec)');

gyroReadings1Mean = mean(gyroReadings1,2);

%% Second set of data:

write(arduino,1,"uint8"); % Gives the signal to start sampling the IMU.
[gyroReadings2,~] = readData(noOfSamples,arduino);
flush(arduino);

%% Plot the first set and compute its mean:

tiledlayout(3,1);
nexttile
plot(t,gyroReadings2(1,1:noOfSamples));
title('X-axis Gyroscope Readings (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings2(2,1:noOfSamples));
title('Y-axis Gyroscope Readings (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings2(3,1:noOfSamples));
title('Z-axis Gyroscope Readings (LSBs) vs Time (sec)');

gyroReadings2Mean = mean(gyroReadings2,2);

%% Third and final set of data:

write(arduino,1,"uint8"); % Gives the signal to start sampling the IMU.
[gyroReadings3,~] = readData(noOfSamples,arduino);
flush(arduino);

%% Plot the first set and compute its mean:

tiledlayout(3,1);
nexttile
plot(t,gyroReadings3(1,1:noOfSamples));
title('X-axis Gyroscope Readings (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings3(2,1:noOfSamples));
title('Y-axis Gyroscope Readings (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings3(3,1:noOfSamples));
title('Z-axis Gyroscope Readings (LSBs) vs Time (sec)');

gyroReadings3Mean = mean(gyroReadings3,2);

%% Calculating and applying the new bias factors

b = (gyroReadings1Mean + gyroReadings2Mean + gyroReadings3Mean)/3;

gyroReadings1Calibrated = double(gyroReadings1) - b;
gyroReadings2Calibrated = double(gyroReadings2) - b;
gyroReadings3Calibrated = double(gyroReadings3) - b;

gyroReadings1Magnitude = zeros(1,noOfSamples);
gyroReadings2Magnitude = zeros(1,noOfSamples);
gyroReadings3Magnitude = zeros(1,noOfSamples);

for i = 1:noOfSamples
    gyroReadings1Magnitude(i) = sqrt(gyroReadings1Calibrated(1,i)^2 + ...
    gyroReadings1Calibrated(2,i)^2 + gyroReadings1Calibrated(3,i)^2);

    gyroReadings2Magnitude(i) = sqrt(gyroReadings2Calibrated(1,i)^2 + ...
    gyroReadings2Calibrated(2,i)^2 + gyroReadings2Calibrated(3,i)^2);

    gyroReadings3Magnitude(i) = sqrt(gyroReadings3Calibrated(1,i)^2 + ...
    gyroReadings3Calibrated(2,i)^2 + gyroReadings3Calibrated(3,i)^2);
end

%% Validating the calibration through magn. of the gyro readings vector

gyroReadings1MagnitudeMean = mean(gyroReadings1Magnitude);
gyroReadings2MagnitudeMean = mean(gyroReadings2Magnitude);
gyroReadings3MagnitudeMean = mean(gyroReadings3Magnitude);

tiledlayout(3,1);
nexttile
plot(t,gyroReadings1Magnitude(1,1:noOfSamples));
title(["Magnitude of Gyroscpe Readings from 1st set (LSBs) vs Time (sec)", ...
    strcat("Mean value = ",num2str(gyroReadings1MagnitudeMean))]);

nexttile
plot(t,gyroReadings2Magnitude(1,1:noOfSamples));
title(["Magnitude of Gyroscpe Readings from 2nd set (LSBs) vs Time (sec)", ...
    strcat("Mean value = ",num2str(gyroReadings2MagnitudeMean))]);

nexttile
plot(t,gyroReadings3Magnitude(1,1:noOfSamples));
title(["Magnitude of Gyroscpe Readings from 3rd set (LSBs) vs Time (sec)", ...
    strcat("Mean value = ",num2str(gyroReadings3MagnitudeMean))]);
%% End of MATLAB Script

clear arduino