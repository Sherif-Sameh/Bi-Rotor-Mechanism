/* This component was added into the system to perform the Extended Kalman Filter (EKF) algorithm
    based on sensor readings from an IMU where the gyroscopes are used for predicting the state
    estimates while the accelerometers' readings are used in the correction step to validate
    the predictions made in the previous step. To keep it as flexible as possible the definitions
    of the state transition and measurement functions along with their jacobians was omitted from
    this file, instead they are defined in a seperate file and passed using pointers to the driver
    for it to call them during the algorithm. */

#ifndef ExtendedKalmanFilter
#define ExtendedKalmanFilter

#include "MPU_Basic.h"
#include "MPU_Advanced.h"
#include "Functions.h"                         /*!< The CControl Library is a lightweight library that supports many
                                                linear algebra matrix operations as well as some control system features
                                                and is used in this project mainly to have a C implmentation of matrix 
                                                operations that can run easily on a uC with limited H/W resources. */

/* Macros Definitions */
#define EKF_SAMPLING_TIME_MS        CONFIG_EKF_SAMPLING_TIME_MS
#define EKF_NUM_OF_STATES           CONFIG_EKF_NUM_OF_STATES
#define EKF_NUM_OF_OBSERVATIONS     CONFIG_EKF_NUM_OF_OBSERVATIONS
#define EKF_NUM_OF_FLAGS            7

/* Typedefs */
typedef void (nonLinearFunc)(const float *, const float *, float *);   /* Alias for defining a function that does not return anything
                                                                        and takes 3 pointers to float arrays as input. */
typedef void (nonLinearFunc2)(const float *, float *);                 /* Alias for defining a function that does not return anything
                                                                        and takes 2 pointers to float arrays as input. */

typedef enum {
    EKF_INIT_SUCCESS,
    EKF_STF_NOT_DEFINED,
    EKF_MF_NOT_DEFINED,
    EKF_STF_JACOB_NOT_DEFINED,
    EKF_MF_JACOB_NOT_DEFINED,
    EKF_PN_COV_NOT_DEFINED,
    EKF_MN_COV_NOT_DEFINED,
    EKF_INIT_NOT_CALLED
} EKF_STATUS_t;

/* Function Declarations */

/*!< Setters and getters for the parameters of the EKF. */
EKF_STATUS_t setStateTransitionFunction(nonLinearFunc *STF);
nonLinearFunc *getStateTransitionFunction();
EKF_STATUS_t setMeasurementFunction(nonLinearFunc2 *MF);
nonLinearFunc2 *getMeasurementFunction();
EKF_STATUS_t setStateTransitionFunctionJacob(nonLinearFunc *STF_Jacob);
nonLinearFunc *getStateTransitionFunctionJacob();
EKF_STATUS_t setMeasurementFunctionJacob(nonLinearFunc2 *MF_Jacob);
nonLinearFunc2 *getMeasurementFunctionJacob();
EKF_STATUS_t setProcessNoiseCov(float *PNCov);
void getProcessNoiseCov(float *PNCov);
EKF_STATUS_t setMeasurementNoiseCov(float *MNCov);
void getMeasurementNoiseCov(float *MNCov);
EKF_STATUS_t setInitialValues(float *initialStateVector, float *initialErrorCov);

/*!< Functions responsible for executing the EKF algorithm. */
#ifdef CONFIG_EKF_RETURN_STF_RESULTS
void kalmanPredictionStep(float *gyroReadings, float *g_k);
void getUpdatedStateEstimate(float *updatedStateVector, float *STF_Result);
#else
void kalmanPredictionStep(float *gyroReadings);
void getUpdatedStateEstimate(float *updatedStateVector);
#endif
void kalmanCorrectionStep(float *accReadings);

#endif