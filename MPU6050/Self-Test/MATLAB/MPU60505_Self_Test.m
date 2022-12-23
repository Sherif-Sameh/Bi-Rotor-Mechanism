%% Establishing Serial communication with the Arduino

% Choose serial port, baud rate and read and write termination characters
% respectively then flush the input buffer of any old data and wait for the
% arduino to establish a connection.

arduino = serialport("COM3", 115200);
configureTerminator(arduino,"CR/LF");
flush(arduino);
pause(4);

%% Read Factory Trim for both gyro and accelerometer

write(arduino,3,"uint8"); % Update the state of the arduino
[gyroFactoryTrim, accFactoryTrim]  = readData(1,arduino);
flush(arduino);

%% Sampling with Self-Test Disabled

noOfSamples = 1000; % Total number of samples
write(arduino,1,"uint8"); % Update the state of the arduino

% Gyroscope and Accelerometer Readings with Self-Test disabled
[gyroReadings, accReadings] = readData(noOfSamples,arduino);
flush(arduino);

%% Sampling with Self-Test Enabled

write(arduino,2,"uint8"); % Enable Self-Test and read new data.
[gyroReadingsST, accReadingsST] = readData(noOfSamples,arduino);
flush(arduino);
clear arduino;

%% Plotting Gyro data with Self-Test Disabled

t = 0:0.01:9.99;
tiledlayout(2,2);
nexttile
plot(t,gyroReadings(1,1:noOfSamples));
title('Gyro X vs Time (sec)');

nexttile
plot(t,gyroReadings(2,1:noOfSamples));
title('Gyro Y vs Time (sec)');

nexttile([1 2])
plot(t,gyroReadings(3,1:noOfSamples));
title('Gyro Z vs Time (sec)');

%% Plotting Accelerometer data with Self-Test Disabled

tiledlayout(2,2);
nexttile
plot(t,accReadings(1,1:noOfSamples));
title('Accelerometer X vs Time (sec)');

nexttile
plot(t,accReadings(2,1:noOfSamples));
title('Accelerometer Y vs Time (sec)');

nexttile([1 2])
plot(t,accReadings(3,1:noOfSamples));
title('Accelerometer Z vs Time (sec)');

%% Plotting Gyro data with Self-Test Enabled

t = 0:0.01:9.99;
tiledlayout(2,2);
nexttile
plot(t,gyroReadingsST(1,1:noOfSamples));
title('Gyro X vs Time (sec)');

nexttile
plot(t,gyroReadingsST(2,1:noOfSamples));
title('Gyro Y vs Time (sec)');

nexttile([1 2])
plot(t,gyroReadingsST(3,1:noOfSamples));
title('Gyro Z vs Time (sec)');

%% Plotting Accelerometer data with Self-Test Enabled

tiledlayout(2,2);
nexttile
plot(t,accReadingsST(1,1:noOfSamples));
title('Accelerometer X vs Time (sec)');

nexttile
plot(t,accReadingsST(2,1:noOfSamples));
title('Accelerometer Y vs Time (sec)');

nexttile([1 2])
plot(t,accReadingsST(3,1:noOfSamples));
title('Accelerometer Z vs Time (sec)');

%% Change from factory trim for both gyro and accelerometer

% Change from FT is calculated using the formula given in the datasheet
% Change = ((Response with ST Enabled - Response with ST Disabled) - FT)/FT
gyroFactoryTrim = double(gyroFactoryTrim);
gyroChangeFT = (double(gyroReadingsST - gyroReadings) - gyroFactoryTrim)./ gyroFactoryTrim;

accFactoryTrim = double(accFactoryTrim);
accChangeFT = (double(accReadingsST - accReadings) - accFactoryTrim)./ accFactoryTrim;

tiledlayout(3,3);
nexttile
plot(t,gyroChangeFT(1,1:noOfSamples).*100);
title('Gyro X Change % vs Time (sec)');

nexttile
plot(t,gyroChangeFT(2,1:noOfSamples).*100);
title('Gyro Y Change % vs Time (sec)');

nexttile
plot(t,gyroChangeFT(3,1:noOfSamples).*100);
title('Gyro Z Change % vs Time (sec)');

nexttile
plot(t,accChangeFT(1,1:noOfSamples).*100);
title('Accelerometer X Change % vs Time (sec)');

nexttile
plot(t,accChangeFT(2,1:noOfSamples).*100);
title('Accelerometer Y Change % vs Time (sec)');

nexttile
plot(t,accChangeFT(3,1:noOfSamples).*100);
title('Accelerometer Z Change % vs Time (sec)');

%% Mean of the % change from FT

meanGyroChangeFT = mean(gyroChangeFT,2)*100;
meanAccChangeFT = mean(accChangeFT,2)*100;
clear arduino