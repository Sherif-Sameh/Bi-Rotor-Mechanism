/*!< This file includes all function definitions for functions declared in the file named EKF.h */

#include "EKF.h"

/* Function Declarations for Static functions. */

/*!< Functions Responsible for ensuring correct initialization of EKF before starting the algorithm.*/
static inline void setFlag(uint8_t bitNumber);
static void updateCurrentStatus();

/*!< Basic functions for manipulating matrices that are not provided by the CControl library. */
static inline void addMatrices(float *A, float *B, float *C, uint16_t row_a, uint16_t column_a, uint16_t row_b, uint16_t column_b);
static inline void subMatrices(float *A, float *B, float *C, uint16_t row_a, uint16_t column_a, uint16_t row_b, uint16_t column_b);
static inline void createIdentityMatrix(float *I, uint16_t n);

/* Global Variables */
static nonLinearFunc *stateTransitionFunction           = NULL;  /*!< This function is called to propagate the states estimates
                                                                    at every iteration. */
static nonLinearFunc2 *measurementFunction              = NULL;  /*!< This function is called to calculate the predicted observations 
                                                                    at every iteration to be compared to the measured observations. */
static nonLinearFunc *stateTransitionFunctionJacob      = NULL;  /*!< This functions is called to calculate the jacobian of the STF
                                                                    at every iteration*/
static nonLinearFunc2 *measurementFunctionJacob         = NULL;  /*!< This functions is called to calculate the jacobian of the MF
                                                                    at every iteration*/

static float stateVector[EKF_NUM_OF_STATES];
static float errorCovariance[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];
static float processNoiseCovariance[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];
static float measurementNoiseCovariance[EKF_NUM_OF_OBSERVATIONS * EKF_NUM_OF_OBSERVATIONS];
static float identityMatrix[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];
static uint8_t EKF_flags = 0;
static EKF_STATUS_t currentStatus = EKF_STF_NOT_DEFINED;
                                               
static inline void setFlag(uint8_t bitNumber)
{
    EKF_flags |= (1 << bitNumber);
}

static void updateCurrentStatus()
{
    for(uint8_t i = 1; i < EKF_NUM_OF_FLAGS + 1; i++)   /*!< Will choose the first missing flag if multiple are missing. */
    {
        if(!(EKF_flags & (1 << i)))
        {
            currentStatus = i;
            return;
        }
    }
    currentStatus = EKF_INIT_SUCCESS;
}

EKF_STATUS_t setStateTransitionFunction(nonLinearFunc *STF)
{
    if(STF != NULL) // Protect against null pointers
    {
        stateTransitionFunction = STF;
        setFlag(EKF_STF_NOT_DEFINED);
        updateCurrentStatus();
    }
    return currentStatus;
}

nonLinearFunc *getStateTransitionFunction()
{
    return stateTransitionFunction;
}

EKF_STATUS_t setMeasurementFunction(nonLinearFunc2 *MF)
{
    if(MF != NULL) // Protect against null pointers
    {
        measurementFunction = MF;
        setFlag(EKF_MF_NOT_DEFINED);
        updateCurrentStatus();
    }
    return currentStatus;
}

nonLinearFunc2 *getMeasurementFunction()
{
    return measurementFunction;
}

EKF_STATUS_t setStateTransitionFunctionJacob(nonLinearFunc *STF_Jacob)
{
    if(STF_Jacob != NULL) // Protect against null pointers
    {
        stateTransitionFunctionJacob = STF_Jacob;
        setFlag(EKF_STF_JACOB_NOT_DEFINED);
        updateCurrentStatus();
    }
    return currentStatus;
}

nonLinearFunc *getStateTransitionFunctionJacob()
{
    return stateTransitionFunctionJacob;
}

EKF_STATUS_t setMeasurementFunctionJacob(nonLinearFunc2 *MF_Jacob)
{
    if(MF_Jacob != NULL) // Protect against null pointers
    {
        measurementFunctionJacob = MF_Jacob;
        setFlag(EKF_MF_JACOB_NOT_DEFINED);
        updateCurrentStatus();
    }
    return currentStatus;
}

nonLinearFunc2 *getMeasurementFunctionJacob()
{
    return measurementFunctionJacob;
}

EKF_STATUS_t setProcessNoiseCov(float *PNCov)
{
    if(PNCov != NULL)
    {
        memcpy(processNoiseCovariance, PNCov, EKF_NUM_OF_STATES*EKF_NUM_OF_STATES*sizeof(float));
        setFlag(EKF_PN_COV_NOT_DEFINED);
        updateCurrentStatus();
    }
    return currentStatus;
}

void getProcessNoiseCov(float *PNCov)
{
    if(processNoiseCovariance != NULL)
    {
        memcpy(PNCov, processNoiseCovariance, EKF_NUM_OF_STATES*EKF_NUM_OF_STATES*sizeof(float));
    }
}

EKF_STATUS_t setMeasurementNoiseCov(float *MNCov)
{
    if(MNCov != NULL)
    {
        memcpy(measurementNoiseCovariance, MNCov, EKF_NUM_OF_OBSERVATIONS*EKF_NUM_OF_OBSERVATIONS*sizeof(float));
        setFlag(EKF_MN_COV_NOT_DEFINED);
        updateCurrentStatus();
    }
    return currentStatus;
}

void getMeasurementNoiseCov(float *MNCov)
{
    if(measurementNoiseCovariance != NULL)
    {
        memcpy(MNCov, measurementNoiseCovariance, EKF_NUM_OF_OBSERVATIONS*EKF_NUM_OF_OBSERVATIONS*sizeof(float));
    }
}

EKF_STATUS_t setInitialValues(float *initialStateVector, float *initialErrorCov)
{
    if((initialStateVector != NULL) && (initialErrorCov !=NULL))
    {
        createIdentityMatrix(identityMatrix, EKF_NUM_OF_STATES);
        memcpy(stateVector, initialStateVector, EKF_NUM_OF_STATES*sizeof(float));
        memcpy(errorCovariance, initialErrorCov, EKF_NUM_OF_STATES*EKF_NUM_OF_STATES*sizeof(float));
        setFlag(EKF_INIT_NOT_CALLED);
        updateCurrentStatus();
    }
    return currentStatus;
}

static inline void addMatrices(float *A, float *B, float *C, uint16_t row_a, uint16_t column_a, uint16_t row_b, uint16_t column_b)
{
    if((row_a != row_b) || (column_a != column_b))
    {
        return;
    }
    for(uint16_t i = 0; i < row_a * column_a; i++)
    {
        C[i] = A[i] + B[i];
    }
}

static inline void subMatrices(float *A, float *B, float *C, uint16_t row_a, uint16_t column_a, uint16_t row_b, uint16_t column_b)
{
    if ((row_a != row_b) || (column_a != column_b))
    {
        return;
    }
    for(uint16_t i = 0; i < row_a * column_a; i++)
    {
        C[i] = A[i] - B[i];
    }
}

static inline void createIdentityMatrix(float *I, uint16_t n)
{
    memset(I, 0, n * n * sizeof(float));
    for(uint16_t i = 0; i < n; i++)
    {
        I[i + i * n] = 1;
    }
}

#ifdef CONFIG_EKF_RETURN_STF_RESULTS
void kalmanPredictionStep(float *gyroReadings, float *g_k)
{
    float G_k[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];               /*!< Vector to save result of the jacobian of the STF. */
    float G_kTranspose[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];
    float Tg_k[EKF_NUM_OF_STATES];
    float PGTranspose_k[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];
    float T_Sampling = ((float)EKF_SAMPLING_TIME_MS)/1000;

    stateTransitionFunction((const float *)stateVector, (const float *)gyroReadings, g_k);          /*!< Propagating the state vector*/
    mul(g_k, &T_Sampling, Tg_k, EKF_NUM_OF_STATES, 1, 1);
    addMatrices(stateVector, Tg_k, stateVector, EKF_NUM_OF_STATES, 1, EKF_NUM_OF_STATES, 1);
    
    stateTransitionFunctionJacob((const float *)stateVector, (const float *)gyroReadings, G_k);     /*!< Propagting the error covariance*/
    memcpy(G_kTranspose, G_k, EKF_NUM_OF_STATES * EKF_NUM_OF_STATES * sizeof(float));
    tran(G_kTranspose, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
    mul(errorCovariance, G_kTranspose, PGTranspose_k, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
    mul(G_k, PGTranspose_k, errorCovariance, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
    addMatrices(errorCovariance, processNoiseCovariance, errorCovariance, EKF_NUM_OF_STATES,
                EKF_NUM_OF_STATES, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
}

#else
void kalmanPredictionStep(float *gyroReadings)
{
    float g_k[EKF_NUM_OF_STATES];
    float G_k[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];               /*!< Vector to save result of the jacobian of the STF. */
    float G_kTranspose[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];
    float Tg_k[EKF_NUM_OF_STATES];
    float PGTranspose_k[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];
    float T_Sampling = ((float)EKF_SAMPLING_TIME_MS)/1000;

    stateTransitionFunction((const float *)stateVector, (const float *)gyroReadings, g_k);          /*!< Propagating the state vector*/
    mul(g_k, &T_Sampling, Tg_k, EKF_NUM_OF_STATES, 1, 1);
    addMatrices(stateVector, Tg_k, stateVector, EKF_NUM_OF_STATES, 1, EKF_NUM_OF_STATES, 1);
    
    stateTransitionFunctionJacob((const float *)stateVector, (const float *)gyroReadings, G_k);     /*!< Propagting the error covariance*/
    memcpy(G_kTranspose, G_k, EKF_NUM_OF_STATES * EKF_NUM_OF_STATES * sizeof(float));
    tran(G_kTranspose, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
    mul(errorCovariance, G_kTranspose, PGTranspose_k, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
    mul(G_k, PGTranspose_k, errorCovariance, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
    addMatrices(errorCovariance, processNoiseCovariance, errorCovariance, EKF_NUM_OF_STATES,
                EKF_NUM_OF_STATES, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
}
#endif

void kalmanCorrectionStep(float *accReadings)
{
    float h_k[EKF_NUM_OF_OBSERVATIONS];
    float H_k[EKF_NUM_OF_OBSERVATIONS * EKF_NUM_OF_STATES];
    float H_kTranspose[EKF_NUM_OF_OBSERVATIONS * EKF_NUM_OF_STATES];
    float PHTranspose_k[EKF_NUM_OF_OBSERVATIONS * EKF_NUM_OF_STATES];
    float S_k[EKF_NUM_OF_OBSERVATIONS * EKF_NUM_OF_OBSERVATIONS];
    float HTransposeS_k[EKF_NUM_OF_STATES * EKF_NUM_OF_OBSERVATIONS];
    float K_k[EKF_NUM_OF_STATES * EKF_NUM_OF_OBSERVATIONS];
    float measurementResidual[EKF_NUM_OF_OBSERVATIONS];
    float correctionVector[EKF_NUM_OF_STATES];
    float KH_k[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];
    float priorErrorCovariance[EKF_NUM_OF_STATES * EKF_NUM_OF_STATES];

    /*!< Calculating S_k = inv(H_k * P_k * HTranspose_k + Q_k) for later using it 
        to calculate the Kalman gain matrix K. */
    measurementFunctionJacob((const float *)stateVector, H_k);
    memcpy(H_kTranspose, H_k, EKF_NUM_OF_OBSERVATIONS * EKF_NUM_OF_STATES * sizeof(float));
    tran(H_kTranspose, EKF_NUM_OF_OBSERVATIONS, EKF_NUM_OF_STATES);
    mul(errorCovariance, H_kTranspose, PHTranspose_k, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES, EKF_NUM_OF_OBSERVATIONS);
    mul(H_k, PHTranspose_k, S_k, EKF_NUM_OF_OBSERVATIONS, EKF_NUM_OF_STATES, EKF_NUM_OF_OBSERVATIONS);
    addMatrices(S_k, measurementNoiseCovariance, S_k, EKF_NUM_OF_OBSERVATIONS, EKF_NUM_OF_OBSERVATIONS,
                EKF_NUM_OF_OBSERVATIONS, EKF_NUM_OF_OBSERVATIONS);
    inv(S_k, EKF_NUM_OF_OBSERVATIONS);

    /*!< Calculating the Kalman gain matrix K = P_k * HTranspose_k * S_k. */
    mul(H_kTranspose, S_k, HTransposeS_k, EKF_NUM_OF_STATES, EKF_NUM_OF_OBSERVATIONS, EKF_NUM_OF_OBSERVATIONS);
    mul(errorCovariance, HTransposeS_k, K_k, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES, EKF_NUM_OF_OBSERVATIONS);

    /*!< Correcting the previous predicted state estimate based on the sensor observations.
        X_k = X_k' + K_k * (Actual observations - Predicted Observations) */
    measurementFunction((const float *)stateVector, h_k);
    subMatrices(accReadings, h_k, measurementResidual, EKF_NUM_OF_OBSERVATIONS, 1, EKF_NUM_OF_OBSERVATIONS, 1);
    mul(K_k, measurementResidual, correctionVector, EKF_NUM_OF_STATES, EKF_NUM_OF_OBSERVATIONS, 1);
    addMatrices(stateVector, correctionVector, stateVector, EKF_NUM_OF_STATES, 1, EKF_NUM_OF_STATES, 1);

    /*!< Correcting the error covariance matrix P_k = (I - K_k * H_k) * P_k' */
    mul(K_k, H_k, KH_k, EKF_NUM_OF_STATES, EKF_NUM_OF_OBSERVATIONS, EKF_NUM_OF_STATES);
    subMatrices(identityMatrix, KH_k, KH_k, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES,
                EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
    memcpy(priorErrorCovariance, errorCovariance, EKF_NUM_OF_STATES * EKF_NUM_OF_STATES * sizeof(float));
    mul(KH_k, priorErrorCovariance, errorCovariance, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES, EKF_NUM_OF_STATES);
}

#ifdef CONFIG_EKF_RETURN_STF_RESULTS
void getUpdatedStateEstimate(float *updatedStateVector, float *STF_Result)
{
    float gyroReadings[3];
    float accReadings[3];
    getGyroReadingsScaledSI(gyroReadings);
    getAccReadingsScaledSI(accReadings);
    kalmanPredictionStep(gyroReadings, STF_Result);
    kalmanCorrectionStep(accReadings);
    memcpy(updatedStateVector, stateVector, EKF_NUM_OF_STATES * sizeof(float));
}

#else
void getUpdatedStateEstimate(float *updatedStateVector)
{
    float gyroReadings[3];
    float accReadings[3];
    getGyroReadingsScaledSI(gyroReadings);
    getAccReadingsScaledSI(accReadings);
    kalmanPredictionStep(gyroReadings);
    kalmanCorrectionStep(accReadings);
    memcpy(updatedStateVector, stateVector, EKF_NUM_OF_STATES * sizeof(float));
}
#endif