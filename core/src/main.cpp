#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <typeinfo>
#include <chrono>

#include <calculations.h>
#include <cuda_functions.h>

#include <food.h>
#include <settings.h>

using namespace std;


cv::Scalar age_color(int age){
    if (5 <= age && age <= 6){
        return cv::Scalar(0, 255, 0);
    } else if (3 <= age && age <= 4){
        return cv::Scalar(0, 255, 255);
    } else if (1 <= age && age <= 2) {
        return cv::Scalar(0, 0, 255);
    } else if (0 == age){
        return cv::Scalar(0, 0, 0);
    }
}


cv::Mat draw(cv::Mat image, int array_food[C][W][H], int radius){
    auto t1 = chrono::high_resolution_clock::now();
#pragma omp parallel for
    for (int i=0;i<W;i++){
        for(int j=0;j<H;j++){
            if(array_food[0][i][j] == 1){
                int size = (int)(array_food[5][i][j] * (radius / 6.0));
                cv::circle(image, cv::Point(i*radius, j*radius), size, age_color(array_food[5][i][j]), -1);
            }
        }
    }

    auto t2 = chrono::high_resolution_clock::now();
    double d = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
    printf("Seconds draw: %f\n", d / 1000000);
    return image;
}


int main() {
    CudaFunctions cuda_funk;

    int width = 1000;
    int height = 1000;
    int radius = width / W;
    cout << radius << endl;

    Food class_food = Food(height, width);
    cv::Mat output(height, width, CV_8UC3);

    int array_food[C][W][H] = {0};
    class_food.create_food(W, H, array_food);

    /// Init draw
    output = draw(output, array_food, radius);
    cv::imshow("Output", output);
    cv::waitKey(0);

    int k = 0;
    while (true){
        auto t1 = chrono::high_resolution_clock::now();
        cout << "K: " << k << endl;
        cout << "Draw" << endl;

        output = draw(output, array_food, radius);
        cv::imshow("Output", output);
        int key = cv::waitKey(1);
        if (key == 27){
            break;
        }
        k++;

        /// Food increase CUDA
        auto t11 = chrono::high_resolution_clock::now();
        cuda_funk.calculate_food(array_food);
        auto t22 = chrono::high_resolution_clock::now();
        double delta = chrono::duration_cast<chrono::microseconds>(t22 - t11).count();
        printf("Calculate: %f FPS: %f\n", delta / 1000000, 1/ (delta / 1000000));


        auto t2 = chrono::high_resolution_clock::now();
        double delta2 = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
        printf("All Seconds: %f FPS: %f\n", delta2 / 1000000, 1 / (delta2 / 1000000));
        cout << "next" << endl << endl;
    }

    return 0;
}


