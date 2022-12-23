%{
 This script was written to capture the accelerometer's raw response when
 stationary and to use the knowledge of the known gravity vector to 
 calibrate its outputs against biasing errors and senstivity scale
 factor errors.

Test Conditions:
- Accelerometer Scale Factor = 16384 LSB/g
- MPU6050 Sampling Rate = 1KHz
- Actual Sampling Rate used = 100Hz
- DLPF Bandwidth = 44Hz (4.9ms delay)

%}

%% Establishing Serial Communication with the Arduino Uno

arduino = serialport("COM3",115200);
configureTerminator(arduino,"CR/LF");
flush(arduino); % Flush serial buffer from any old data.
pause(4); % Wait for serial comm. to be established.

%% First set of data: gravity aligned with the Z-axis

noOfSamples = 501;
Fs = 100; % 100Hz
Ts = 1/Fs;
t = (0:1:(noOfSamples-1))*Ts;

write(arduino,1,'uint8'); % Gives the signal to start sampling the IMU
[~,accReadings1] = readData(noOfSamples,arduino);
flush(arduino);

%% Verfiying the first set of data and computing the mean for each accelerometer.

tiledlayout(3,1);
nexttile
plot(t,accReadings1(1,1:noOfSamples));
title('X-axis Accelerometer Readings (LSBs) vs Time (sec)');

nexttile
plot(t,accReadings1(2,1:noOfSamples));
title('Y-axis Accelerometer Readings (LSBs) vs Time (sec)');

nexttile
plot(t,accReadings1(3,1:noOfSamples));
title('Z-axis Accelerometer Readings (LSBs) vs Time (sec)');

accReadings1Mean = mean(accReadings1,2);
accReadings1Expected = [0;0;16384];

%% Second set of data: gravity at 45deg angle between X and Y axes.

write(arduino,1,'uint8'); % Gives the signal to start sampling the IMU
[~,accReadings2] = readData(noOfSamples,arduino);
flush(arduino);

%% Verfiying the second set of data and computing the mean for each accelerometer.

tiledlayout(3,1);
nexttile
plot(t,accReadings2(1,1:noOfSamples));
title('X-axis Accelerometer Readings (LSBs) vs Time (sec)');

nexttile
plot(t,accReadings2(2,1:noOfSamples));
title('Y-axis Accelerometer Readings (LSBs) vs Time (sec)');

nexttile
plot(t,accReadings2(3,1:noOfSamples));
title('Z-axis Accelerometer Readings (LSBs) vs Time (sec)');

accReadings2Mean = mean(accReadings2,2);
accReadings2Expected = [16384*sin(pi/4);-16384*cos(pi/4);0];

%% Computing the bias factor and the SF correction factor from the recorded data:

accReadingsChange = accReadings2Mean - accReadings1Mean;
accReadingsExpectedChange = accReadings2Expected - accReadings1Expected;
S_cf = diag((accReadingsExpectedChange ./ accReadingsChange));
b = (accReadings1Mean - inv(S_cf)*accReadings1Expected);

%% Applying the corrections to the old data:

accReadings1Calibrated = S_cf*(double(accReadings1) - b);

tiledlayout(3,1);
nexttile
plot(t,accReadings1Calibrated(1,1:noOfSamples));
title('X-axis Calibrated Accelerometer Readings (LSBs) vs Time (sec)');

nexttile
plot(t,accReadings1Calibrated(2,1:noOfSamples));
title('Y-axis Calibrated Accelerometer Readings (LSBs) vs Time (sec)');

nexttile
plot(t,accReadings1Calibrated(3,1:noOfSamples));
title('Z-axis Calibrated Accelerometer Readings (LSBs) vs Time (sec)');

%% Continued for second set:

accReadings2Calibrated = S_cf*(double(accReadings2) - b);

tiledlayout(3,1);
nexttile
plot(t,accReadings2Calibrated(1,1:noOfSamples));
title('X-axis Calibrated Accelerometer Readings (LSBs) vs Time (sec)');

nexttile
plot(t,accReadings2Calibrated(2,1:noOfSamples));
title('Y-axis Calibrated Accelerometer Readings (LSBs) vs Time (sec)');

nexttile
plot(t,accReadings2Calibrated(3,1:noOfSamples));
title('Z-axis Calibrated Accelerometer Readings (LSBs) vs Time (sec)');

%% Testing the calibration for random orientations:

write(arduino,1,'uint8');
[~,accReadings3] = readData(noOfSamples,arduino);
flush(arduino);
accReadings3Calibrated = S_cf*(double(accReadings3) - b);
accReadings3Magnitude = zeros(1,noOfSamples);

for i=1:noOfSamples
    accReadings3Magnitude(1,i) = sqrt(accReadings3Calibrated(1,i)^2 + accReadings3Calibrated(2,i)^2 + accReadings3Calibrated(3,i)^2);
end

plot(t,accReadings3Magnitude);
xlabel('time (sec)');
ylabel('acceleration (LSBs)');
title('Magnitude of acceleration readings vs Time');

%% End of MATLAB Script

clear arduino