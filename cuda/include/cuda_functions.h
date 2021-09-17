#ifndef CUDA_FUNK_H
#define CUDA_FUNK_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <calculations.h>
#include <cuda_runtime.h>
#include <curand.h>
#include <cuda.h>
#include <cstring>
#include <stdio.h>

#include <food.h>
#include <settings.h>

#include <array>
#include <curand.h>
#include <curand_kernel.h>

#include <thrust/random/linear_congruential_engine.h>
#include <thrust/random/uniform_int_distribution.h>

using namespace std;


class CudaFunctions{
public:
    CudaFunctions();
    void test();
    void calculate_food(array2D *array_food);


};

#endif
