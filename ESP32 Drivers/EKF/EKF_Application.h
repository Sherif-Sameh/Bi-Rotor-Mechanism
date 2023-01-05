/* This header file is what provides the definitions of the 4 functions needed by the EKF namely:
   - State Transition Function (STF)
   - Measurement Function (MF)
   - Jacobian of the STF
   - Jacobian of the MF
   In this file the initialization of the EKF is done by setting the 4 previously mentioned functions
   along with the covariance matrices and the initial conditions of the EKF. 
   
   Note: The parameters of the EKF config file should be set from menuconfig according to the application
   before proceeding with using the EKF. */

#ifndef EKF_Application
#define EKF_Application

#include "EKF.h"
#include <math.h>

/* Macro Definitions*/
#define SIN_TAYLOR_APPROX(X)    (X - (X*X*X)/6)     // Taylor approximation for Sin(x) about a = 0 using 
                                                        // the first 2 terms in the series.
#define COS_TAYLOR_APPROX(X)    (1 - (X*X)/2)       // Taylor approximation for Cos(x) about a = 0 using 
                                                        // the first 2 terms in the series.
#define GRAVITY_CONSTANT            9.81

/* IMU Configuration Parameters */
#define IMU_GYROSCOPE_FS_RANGE      GYRO_FS_RANGE_250DPS
#define IMU_ACCELEROMETER_FS_RANGE  ACCELEROMETER_FS_RANGE_2G
#define IMU_DLPF_SETTING            3
#define IMU_INTERNAL_CLK_SOURCE     GYRO_X
#define IMU_AD0                     0


/* Global Variables */
float sin_p = 0, cos_p = 1;                             /*!< Used to store the values of sin and cos of rho to
                                                        avoid revaluation at every use of them.*/                          
float sin_e = 0, cos_e = 1;                             /*!< Used to store the values of sin and cos of epsilon to
                                                        avoid revaluation at every use of them.*/

/* Function Definitions */
void stateTransitionFunction(const float *x, const float *u, float *g_k)
{
    sin_p = SIN_TAYLOR_APPROX(x[0]);
    cos_p = COS_TAYLOR_APPROX(x[0]);
    g_k[0] = u[0];
    g_k[1] = (u[1] * cos_p) - (u[2] * sin_p);  
}

void stateTransitionFunctionJacob(const float *x, const float * u, float *G_k)
{
    sin_p = SIN_TAYLOR_APPROX(x[0]);    /*!< Sin and Cos revaluated due to update of state vector. */
    cos_p = COS_TAYLOR_APPROX(x[0]);
    G_k[0] = 0;                                     G_k[1] = 0;
    G_k[2] = (-u[1] * sin_p) - (u[2] * cos_p);      G_k[3] = 0;
}

void measurementFunction(const float *x, float *h_k)
{
    sin_e = SIN_TAYLOR_APPROX(x[1]);    /*!< Only Sin and Cos epsilon need revaluation*/
    cos_e = COS_TAYLOR_APPROX(x[1]);
    h_k[0] = -(GRAVITY_CONSTANT * sin_e);
    h_k[1] = GRAVITY_CONSTANT * cos_e * sin_p;
    h_k[2] = GRAVITY_CONSTANT * cos_e * cos_p;
}

void measurementFunctionJacob(const float *x, float *H_k)
{
    /*!< Technically all 4 trigonometric terms should be updated here to have
    the best estimates for the last step in the EKF when correcting the error 
    covariance matrix based on the final corrected state estimates but this 
    is avoided to prioritize performance. */
    H_k[0] =  0;                                     H_k[1] = -(GRAVITY_CONSTANT * cos_e);
    H_k[2] =  GRAVITY_CONSTANT * cos_p * cos_e;      H_k[3] = -(GRAVITY_CONSTANT * sin_e * sin_p);
    H_k[4] = -(GRAVITY_CONSTANT * cos_e * sin_p);    H_k[5] = -(GRAVITY_CONSTANT * sin_e * cos_p);
}

EKF_STATUS_t calcInitialVectors() /*!< MUST be called after initExtendedKalmanFilter(). */
{
    float accReadings[3];
    float x[2];
    float errorCov[4] = {2.06e-5, 0                 /*!< Pho based on cov. of Ay while epsilon in cov. if Ax. */
                        ,0       ,1.7971e-5}; 
    
    getAccReadingsScaledSI(accReadings);
    x[1] = (float) asin((double) -(accReadings[0] / GRAVITY_CONSTANT));
    float cosE = (float) cos((double) x[1]);
    x[0] = (float) asin((double) accReadings[1] / (GRAVITY_CONSTANT * cosE));

    return setInitialValues(x, errorCov);
}

EKF_STATUS_t initExtendedKalmanFilter()     /*!< All initialization are done except for the intial vectors. */
{
    while(mpuInit(IMU_AD0) != ESP_OK);      /*!< Make sure I2C comm. is initialized successfully. */
    setClockSource(IMU_INTERNAL_CLK_SOURCE);
    setAccFS_Range(IMU_ACCELEROMETER_FS_RANGE);
    setGyroFS_Range(IMU_GYROSCOPE_FS_RANGE);
    setDLPF(IMU_DLPF_SETTING);

    nonLinearFunc *STF = stateTransitionFunction;
    nonLinearFunc2 *MF = measurementFunction;
    nonLinearFunc *STF_Jacob = stateTransitionFunctionJacob;
    nonLinearFunc2 *MF_Jacob = measurementFunctionJacob;
    float PN_Cov[4] = {4.11e-7, 0
                      ,0       ,6.1135e-7};
    float MN_Cov[9] = {1.7971e-4, 0,       0,
                       0,         2.06e-4, 0,
                       0,         0,       5.403e-4};

    if(setStateTransitionFunction(STF) == EKF_STF_NOT_DEFINED)
    {
        return EKF_STF_NOT_DEFINED;
    }
    
    if(setMeasurementFunction(MF) == EKF_MF_NOT_DEFINED)
    {
        return EKF_MF_NOT_DEFINED;
    }
    
    if(setStateTransitionFunctionJacob(STF_Jacob) == EKF_STF_JACOB_NOT_DEFINED)
    {
        return EKF_STF_JACOB_NOT_DEFINED;
    }

    if(setMeasurementFunctionJacob(MF_Jacob) == EKF_MF_JACOB_NOT_DEFINED)
    {
        return EKF_MF_JACOB_NOT_DEFINED;
    }
    
    if(setProcessNoiseCov(PN_Cov) == EKF_PN_COV_NOT_DEFINED)
    {
        return EKF_PN_COV_NOT_DEFINED;
    }
    
    if(setMeasurementNoiseCov(MN_Cov) == EKF_MN_COV_NOT_DEFINED)
    {
        return EKF_MN_COV_NOT_DEFINED;
    }

    return EKF_INIT_NOT_CALLED;
}

#endif