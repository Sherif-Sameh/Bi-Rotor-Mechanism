#include "StateSpaceController_SL.h"

float x[4];

const uint16_t row_x = 4;
const uint16_t col_x = 1;

float u[2];

const uint16_t row_u = 2;
const uint16_t col_u = 1;

float dt[1] = {0.02};

const uint16_t row_dt = 1;
const uint16_t col_dt = 1; 

float A[16] = {0, 1, 0, 0, 
               0, 0, 0, 0,
               0, 0, 0, 1,
               0, 0, 0, 0};

const uint16_t row_a = 4;
const uint16_t col_a = 4; 

float B[8] = {0, 0,  
              13.6992, -13.6992,
              0, 0, 
              7.14637, 7.14637};

const uint16_t row_b = 4;
const uint16_t col_b = 2;

float C[8] = {0, 1, 0, 0, 
              0, 0, 0, 1};

const uint16_t row_c = 2;
const uint16_t col_c = 4;

float D[8] = {0, 0, 
              0, 0, 
              0, 0, 
              0, 0}; 

const uint16_t row_d = 4;
const uint16_t col_d = 2;

float K[8] = {6.16805, 0.948945, 11.8238, 1.81908,
              -6.16805, -0.948945, 11.8238, 1.81908};

const uint16_t row_k = 2;
const uint16_t col_k = 4;

float r[2] = {0,
              0};

const uint16_t row_r = 2;
const uint16_t col_r = 1;

void setReferenceAngles(float *ref)
{
  r[0] = ref[0];
  r[1] = ref[1];
}

void updateController(float *observedStates, float *actValues){
    float tP[2] = {0,0};
    float x[4] = {observedStates[0],
                  observedStates[1],
                  observedStates[2],
                  observedStates[3]};

    mul(K, x, tP, row_k, col_k, col_x);
    sub(r, tP, u, row_r, col_r, row_u, col_u);

    actValues[0] = u[0];
    actValues[1] = u[1];
}

void add(float *A, float *B, float *C, uint16_t row_a, uint16_t column_a, uint16_t row_b, uint16_t column_b)
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

void sub(float *A, float *B, float *C, uint16_t row_a, uint16_t column_a, uint16_t row_b, uint16_t column_b)
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

void mul(float A[], float B[], float C[], uint16_t row_a, uint16_t column_a, uint16_t column_b) {

  // Data matrix
  float* data_a;
  float* data_b;

  for (uint16_t i = 0; i < row_a; i++) {
    // Then we go through every column of b
    for (uint16_t j = 0; j < column_b; j++) {
      data_a = &A[i * column_a];
      data_b = &B[j];

      *C = 0; // Reset
      // And we multiply rows from a with columns of b
      for (uint16_t k = 0; k < column_a; k++) {
        *C += *data_a * *data_b;
        data_a++;
        data_b += column_b;
      }
      C++; // ;)
    }
  }
}
