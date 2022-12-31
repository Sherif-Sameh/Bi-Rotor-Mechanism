%{

This script was written to capture both the output of both the gyroscopes
 and the accelerometers in different positions when stationary in order to
 determine their standard deviations and thus their variances for use in 
 the EKF.

Test Conditions:
- Accelerometer Scale Factor = 16384 LSB/g
- Gyroscope Scale Factor = 131 LSB/(dps)
- MPU6050 Sampling Rate = 1KHz
- Actual Sampling Rate used = 50Hz
- Accelerometer DLPF Bandwidth  = 44Hz (4.9ms delay)
- Gyroscope DLPF Bandwidth = 42Hz (4.8ms delay)

%}

%% Establishing Serial Communication with the ESP32

ESP32 = serialport("COM4",115200);
configureTerminator(ESP32,"LF");
flush(ESP32); % Flush serial buffer from any old data.
pause(4); % Wait for serial comm. to be established.

%% Capturing the 1st sample:

noOfSamples = 501;
Fs = 50; % 50Hz
Ts = 1/Fs;
t = (0:1:(noOfSamples-1))*Ts;

write(ESP32,1,'uint8'); % Gives the signal to start sampling the IMU
[gyroReadings,accReadings] = readFloats(noOfSamples,ESP32);
flush(ESP32);

%% Calculating Covariances for the 1st sample and storing the results:

gyroReadings = (gyroReadings ./ 131) .* (pi/180);
accReadings = (accReadings ./ 16384) .* (9.81);

tiledlayout(3,2);
nexttile
plot(t,gyroReadings(1,1:noOfSamples));
title('X-axis Gyro (LSBs) vs Time (sec)');
nexttile
plot(t,accReadings(1,1:noOfSamples));
title('X-axis Accelerometer (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings(2,1:noOfSamples));
title('Y-axis Gyro (LSBs) vs Time (sec)');
nexttile
plot(t,accReadings(2,1:noOfSamples));
title('Y-axis Accelerometer (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings(3,1:noOfSamples));
title('Z-axis Gyro (LSBs) vs Time (sec)');
nexttile
plot(t,accReadings(3,1:noOfSamples));
title('Z-axis Accelerometer (LSBs) vs Time (sec)');

gyroCov1 = cov(transpose(gyroReadings));
accCov1 = cov(transpose(accReadings));

%% Capturing the 2nd sample:

noOfSamples = 501;
Fs = 50; % 50Hz
Ts = 1/Fs;
t = (0:1:(noOfSamples-1))*Ts;

write(ESP32,1,'uint8'); % Gives the signal to start sampling the IMU
[gyroReadings,accReadings] = readFloats(noOfSamples,ESP32);
flush(ESP32);

%% Calculating Covariances for the 2nd sample and storing the results:

gyroReadings = (gyroReadings ./ 131) .* (pi/180);
accReadings = (accReadings ./ 16384) .* (9.81);

tiledlayout(3,2);
nexttile
plot(t,gyroReadings(1,1:noOfSamples));
title('X-axis Gyro (LSBs) vs Time (sec)');
nexttile
plot(t,accReadings(1,1:noOfSamples));
title('X-axis Accelerometer (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings(2,1:noOfSamples));
title('Y-axis Gyro (LSBs) vs Time (sec)');
nexttile
plot(t,accReadings(2,1:noOfSamples));
title('Y-axis Accelerometer (LSBs) vs Time (sec)');

nexttile
plot(t,gyroReadings(3,1:noOfSamples));
title('Z-axis Gyro (LSBs) vs Time (sec)');
nexttile
plot(t,accReadings(3,1:noOfSamples));
title('Z-axis Accelerometer (LSBs) vs Time (sec)');

gyroCov2 = cov(transpose(gyroReadings));
accCov2 = cov(transpose(accReadings));
