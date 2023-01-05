/**
 * @file EKF.h
 * @author Sherif Sameh
 * @brief Performs the EKF algorithm by combining readings from a 6-axis IMU to provide
 * a good estimate of orientation.
 * This component was added into the system to perform the Extended Kalman Filter (EKF) algorithm
 * based on sensor readings from an IMU where the gyroscopes are used for predicting the state
 * estimates while the accelerometers' readings are used in the correction step to validate
 * the predictions made in the previous step. To keep it as flexible as possible the definitions
 * of the state transition and measurement functions along with their Jacobians was omitted from
 * this file, instead they are defined in a seperate file and passed using pointers to the driver
 * for it to call them during the algorithm.
 * @version 1.0
 * @date 2023-01-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef ExtendedKalmanFilter
#define ExtendedKalmanFilter

#include "MPU_Basic.h"
#include "MPU_Advanced.h"
#include "Functions.h"                         /* The CControl Library is a lightweight library that supports many
                                                linear algebra matrix operations as well as some control system features
                                                and is used in this project mainly to have a C implmentation of matrix 
                                                operations that can run easily on a uC with limited H/W resources. */

/* Symbolic Constants */
#define EKF_SAMPLING_TIME_MS        CONFIG_EKF_SAMPLING_TIME_MS
#define EKF_NUM_OF_STATES           CONFIG_EKF_NUM_OF_STATES
#define EKF_NUM_OF_OBSERVATIONS     CONFIG_EKF_NUM_OF_OBSERVATIONS
#define EKF_NUM_OF_FLAGS            7

/**
 * @def EKF_SAMPLING_TIME_MS
 * This parameter is used to set the sampling rate that is used during the calculation of the EKF but does in way
 * enforce that sampling rate, that is left to the application to ensure that this rate is followed. Set through the
 * menuconfig.
 */

/**
 * @def EKF_NUM_OF_STATES
 * This parameter describes the number of states that are estimated through the EKF. Set through the menuconfig.
 */

/**
 * @def EKF_NUM_OF_OBSERVATIONS
 * This parameter describes the number of available observations to the EKF that it will use to correct the predictions
 * of the model.
 */

/**
 * @def EKF_NUM_OF_FLAGS 
 * This parameter represents the number of flags that must be set before declaring that the EKF has been initialized
 * successfully and the algorithm can begin.
 */


/* Typedefs */
typedef void (nonLinearFunc)(const float *, const float *, float *);   
typedef void (nonLinearFunc2)(const float *, float *);                 

/**
 * @typedef nonLinearFunc
 * Alias for defining a function that does not return anything and takes 3 pointers to float arrays as input. */
 */

/**
 * @typedef nonLinearFunc2 
 * Alias for defining a function that does not return anything and takes 2 pointers to float arrays as input. */
 */


/* Enum defintions */

/**
 * This enum is used to describe the current status of the EKF and is used as a checklist of processes that
 * should be done to initialize the EKF before starting the algorithm.
 */
typedef enum {
    EKF_INIT_SUCCESS,               /**< The EKF has been fully initialized succesfully */
    EKF_STF_NOT_DEFINED,            /**< The state transition function (STF) is not set */
    EKF_MF_NOT_DEFINED,             /**< The measurement function (MF) is not set */
    EKF_STF_JACOB_NOT_DEFINED,      /**< The Jacobian of the STF has is not set */
    EKF_MF_JACOB_NOT_DEFINED,       /**< The Jacobian of the MF has is not set */
    EKF_PN_COV_NOT_DEFINED,         /**< The process noise covariance has not been set */
    EKF_MN_COV_NOT_DEFINED,         /**< The measurement noise covariance has not been set */
    EKF_INIT_NOT_CALLED             /**< The initial states and error covariance have not been set */
} EKF_STATUS_t;


/* Function Declarations */
/*!< Setters and getters for the parameters of the EKF. */

/**
 * @brief Sets and stores the pointer to the state transition function.
 * @param STF A function pointer that points to the STF.
 * @return EKF_STATUS_t Will not return EKF_STF_NOT_DEFINED if successful. 
 */
EKF_STATUS_t setStateTransitionFunction(nonLinearFunc *STF);

/**
 * @brief Returns the stored pointer to the STF.
 * @return nonLinearFunc* Pointer to the STF, equals NULL if the STF has not been defined.
 */
nonLinearFunc *getStateTransitionFunction();

/**
 * @brief Sets and stores the pointer to the measurment function.
 * @param MF A function pointer that points to the MF.
 * @return EKF_STATUS_t Will not return EKF_MF_NOT_DEFINED if successful.
 */
EKF_STATUS_t setMeasurementFunction(nonLinearFunc2 *MF);

/**
 * @brief Returns the stored pointer to the MF.
 * @return nonLinearFunc2* Pointer to the MF, equals NULL if the MF has not been defined.
 */
nonLinearFunc2 *getMeasurementFunction();

/**
 * @brief Sets and stores the pointer to the Jacobian of the STF.
 * @param STF_Jacob A function pointer that points to the Jacobian of the STF.
 * @return EKF_STATUS_t Will not return EKF_STF_JACOB_NOT_DEFINED if successful. 
 */
EKF_STATUS_t setStateTransitionFunctionJacob(nonLinearFunc *STF_Jacob);

/**
 * @brief Returns the stored pointer to the Jacobian of the STF.
 * @return nonLinearFunc* Pointer to the Jacobian of the STF, equals NULL if the 
 * Jacobian has not been defined. 
 */
nonLinearFunc *getStateTransitionFunctionJacob();

/**
 * @brief Sets and stores the pointer to the Jacobian of the MF.
 * @param MF_Jacob A function pointer that points to the Jacobian of the MF.
 * @return EKF_STATUS_t Will not return EKF_MF_JACOB_NOT_DEFINED if successful. 
 */
EKF_STATUS_t setMeasurementFunctionJacob(nonLinearFunc2 *MF_Jacob);

/**
 * @brief Returns the stored pointer to the Jacobian of the MF.
 * @return nonLinearFunc2* Pointer to the Jacobian of the MF, equals NULL if the 
 * Jacobian has not been defined.
 */
nonLinearFunc2 *getMeasurementFunctionJacob();

/**
 * @brief Sets and stores the value of the Process Noise Covariance. 
 * @param PNCov A pointer to an array of floats that represents the process noise
 * covariance matrix.
 * @return EKF_STATUS_t Will not return EKF_PN_COV_NOT_DEFINED if successful. 
 */
EKF_STATUS_t setProcessNoiseCov(float *PNCov);

/**
 * @brief Will copy the value of the PN covariance matrix into the array given.
 * @param PNCov A pointer to the array that the PN covariance will be copied into.
 */
void getProcessNoiseCov(float *PNCov);

/**
 * @brief Set the Measurement Noise Covariance.
 * @param MNCov A pointer to an array of floats that represents the measurement
 * noise covariance matrix.
 * @return EKF_STATUS_t Will not return EKF_MN_COV_NOT_DEFINED if successful. 
 */
EKF_STATUS_t setMeasurementNoiseCov(float *MNCov);

/**
 * @brief Will copy the value of the MN covariance matrix into the array given.
 * @param MNCov A pointer to the array that the MN covariance will be copied into.
 */
void getMeasurementNoiseCov(float *MNCov);

/**
 * @brief This function is used to set the initial state vector and error
 * covariance matrix for the EKF.
 * @param initialStateVector A pointer to the start of the initial state vector.
 * @param initialErrorCov A pointer to the start of the initial error covariance matrix.
 * @return EKF_STATUS_t Will not return EKF_INIT_NOT_CALLED if successful.
 */
EKF_STATUS_t setInitialValues(float *initialStateVector, float *initialErrorCov);


/*!< Functions responsible for executing the EKF algorithm. */
#ifdef CONFIG_EKF_RETURN_STF_RESULTS

/**
 * @brief Run the prediction step only of the EKF.
 * @param gyroReadings The scaled gyroscope readings in rad/s.
 * @param g_k The result of the state transition function for this iteration.
 * @attention EKF_RETURN_STF_RESULTS must be set in the menuconfig for this function to be active.
 */
void kalmanPredictionStep(float *gyroReadings, float *g_k);

/**
 * @brief Runs a full iteration of the EKF to calculate the updated state vector estimate.
 * @param updatedStateVector A pointer to an array of floats that will store the updated
 * state vector estimate.
 * @param STF_Result The result of the state transition function for this iteration.
 * @attention EKF_RETURN_STF_RESULTS must be set in the menuconfig for this function to be active.
 */
void getUpdatedStateEstimate(float *updatedStateVector, float *STF_Result);
#else

/**
 * @brief Run the prediction step only of the EKF.
 * @param gyroReadings The scaled gyroscope readings in rad/s.
 */
void kalmanPredictionStep(float *gyroReadings);

/**
 * @brief Runs a full iteration of the EKF to calculate the updated state vector estimate.
 * @param updatedStateVector A pointer to an array of floats that will store the updated
 * state vector estimate.
 */
void getUpdatedStateEstimate(float *updatedStateVector);
#endif

/**
 * @brief Run the correction step only of the EKF.
 * @param accReadings The scaled accelerometer readings in m/s^2.
 */
void kalmanCorrectionStep(float *accReadings);

#endif