#include <cuda_functions.h>


using namespace std;

__global__ void calculate(int * array_food){
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    array_food[idx] = array_food[idx] + 1;
}


// 0 Position and type obj
// 1 age
// 2 safety points
// 3 grow points
// 4 repr points
// 5 health points


__device__ int direction[8][2] = {{-1, -1}, {0, -1}, {1, -1},
                                  {-1, 0}, {1, 0},
                                  {-1, 1}, {0, 1}, {1, 1}};


__device__ void check_safety(int array_food[][W][H], int idx, int idy, int c_max, bool &safe){
    int c = 0;
    safe = false;
    for (auto route : direction){
        int pos_x = idx + route[0];
        int pos_y = idy + route[1];
        if(pos_x >= 0 && pos_y >= 0 && pos_x < W && pos_y < H){
            if (array_food[0][pos_x][pos_y] == 1 && array_food[5][pos_x][pos_y] >= 3){
                c++;
            }
        } else {
            c++;
        }
    }
    if (c >= c_max){
        safe = true;
    }
}


dim3 dimBlock(W, H);
dim3 dimGrid(1, 1);



__global__ void calculate_obj(int array_food[C][W][H]){
//__global__ void calculate_obj(int ***gpu_vector){
    int idx = blockIdx.x*blockDim.x + threadIdx.x;
    int idy = blockIdx.y*blockDim.y + threadIdx.y;

    bool safe = false;

    if (array_food[0][idx][idy] == 1){
        /// Remove
        if(array_food[1][idx][idy] == 50){//array_food[5][idx][idy] == 0){
            array_food[0][idx][idy] = 0;
            array_food[1][idx][idy] = 0;
            array_food[2][idx][idy] = 0;
            array_food[3][idx][idy] = 0;
            array_food[4][idx][idy] = 0;
        } else {
            array_food[1][idx][idy] += 1;
            check_safety(array_food, idx, idy, 8, safe);
            /// Check non blocked
            if (safe == false){


                /// Check safety
                check_safety(array_food, idx, idy, 6, safe);

                /// Increase health and size
                if (array_food[1][idx][idy] % 5 == 0){
//                    array_food[1][idx][idy] = 1;  /// ToDo remove
                    if (array_food[5][idx][idy] < 6){
                        array_food[5][idx][idy] += 1;
                    }
                }

                /// Grow inc/dec
                if (array_food[5][idx][idy] >= 3){
                    array_food[3][idx][idy] += 1;
                } else {
                    array_food[3][idx][idy] = 0;
                }

                ///Grow direction
                if (array_food[3][idx][idy] == 3){
                    array_food[3][idx][idy] = 0;
    //                random_shuffle(direction, direction+9);

                    for (auto route : direction){
                        int pos_x = idx + route[0];
                        int pos_y = idy + route[1];

                        if(pos_x >= 0 && pos_y >= 0 && pos_x <= W && pos_y <= H){
                            if (array_food[0][pos_x][pos_y] == 0){
                                /// Create
                                array_food[0][pos_x][pos_y] = 1;
                                array_food[1][pos_x][pos_y] = 1;
                                array_food[5][pos_x][pos_y] = 1;
                            }
                        }
                    }
                }
            } else {
                array_food[5][idx][idy] = 6;
            }
        }
    }
}

CudaFunctions::CudaFunctions(){
    int count;
    cudaGetDeviceCount(&count);
    printf("Device: %d\n", count);
    cudaDeviceProp dev_prop;
    for(int i = 0; i < count; i++){
        cudaGetDeviceProperties(&dev_prop, i);
        printf("ID: %d Name: %s\n", i, dev_prop.name);
    }

    int dev;
    memset(&dev_prop, 0, sizeof(cudaDeviceProp));

    dev_prop.major = 1;
    dev_prop.minor = 3;

    cudaGetDevice(&dev);
    printf("ID current: %d\n", dev);
    cudaChooseDevice(&dev, &dev_prop);
    printf("ID nearest: %d\n\n", dev);
    cudaSetDevice(dev);
}


void CudaFunctions::test(){
    cout << "test" << endl;
}

#include <chrono>

void CudaFunctions::calculate_food(array2D *array_food){
    array2D *gpu_vector;

    cudaMalloc(&gpu_vector, C*W*H*C*sizeof(int));
    cudaMemcpy(gpu_vector, array_food, C*W*H*sizeof(int), cudaMemcpyHostToDevice);

    calculate_obj<<<dimBlock, dimGrid>>>(gpu_vector);
    cudaDeviceSynchronize();

    cudaMemcpy(array_food, gpu_vector, C*W*H*sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(gpu_vector);
    cudaDeviceSynchronize();
}
