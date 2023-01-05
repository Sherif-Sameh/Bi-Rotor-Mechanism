#ifndef StateSpaceController
#define StateSpaceController

#include <stdint.h>

void updateController(float *observedStates, float *actValues);
void setReferenceAngles(float *ref);
void add(float *A, float *B, float *C, uint16_t row_a, uint16_t column_a, uint16_t row_b, uint16_t column_b);
void sub(float *A, float *B, float *C, uint16_t row_a, uint16_t column_a, uint16_t row_b, uint16_t column_b);
void mul(float A[], float B[], float C[], uint16_t row_a, uint16_t column_a, uint16_t column_b);

#endif
