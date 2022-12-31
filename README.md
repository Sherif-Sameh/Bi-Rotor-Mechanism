# Bi-Rotor-Mechanism

This project is concerned with the stabilization and control of an externally excited Bi-Rotor mechanism through the use of a closed loop state feedback controller. The mechanism has 2 degrees of freedom, namely pitch and elevation, which are actuated through 2 1400Kv brushless DC motors situated on the mechanism that generate their thrust through the use of 8045 propellers. Also, an external excitation is forced through the use of a DC motor with a slider crank mechanism to consistently provide disturbance into the system and force the controller to react to these disturbances and the observer to try and filter these disturbances. The main sensing element in the system is a 6-axis IMU (MPU6050) which is used to measure the angular rates through its gyroscopes then by combining the readings from the accelerometers and gyroscopes through the use of a Kalman Filter we generate an estimate of the missing states to have the full state vector that is fed into the controller to decide on the best control signal for the motors. The motors are driven using an AVR microcontroller running FreeRTOS to generate the needed pwm signals that are sent to the ESCs that will control the speed of the motors. An ESP32 development board is added into the system to alleviate the computational burden of a Kalman Filter or an Extended Kalman Filter (EKF) off of the weaker 8 bit AVR microcontroller. Additionally the wireless communication features provided by the ESP32, namely through WiFi, are used to communicate with the system wirelessly by sending the reference states of the system to the controller from a Laptop and to monitor the states of the system remotely by displaying them in real-time in MATLAB.
