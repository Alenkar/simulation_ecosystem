#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <typeinfo>

#include <food.h>

using namespace std;


cv::Mat draw(Food class_food, cv::Mat &output, std::vector<FoodObj> &vector_food, int radius){
#pragma omp parallel
    {
    for (int i = 0; i < vector_food.size(); i++){
        cv::circle(output, vector_food.at(i).pos, radius, class_food.age_color(vector_food.at(i).age), -1);
    }
    }
    cv::imshow("Output", output);
    cv::waitKey(1);
    return output;
}


int main() {
    int width = 500;
    int height = 500;

    Food class_food = Food();

    cv::Mat output(height, width, CV_8UC3);
    int **id_matrix = new int * [height];
        for (int i = 0; i < height; i++) {
            id_matrix[i] = new int [width];
        }

    std::vector<FoodObj> vector_food;

    class_food.create_food(height, width, vector_food, id_matrix);

    int radius = class_food.d / 2;

    draw(class_food, output, vector_food, radius);

    int k = 0;
    while (true){
//    for (int k = 0; k < 25; k++){
        cout << "K: " << k << endl;
        cout << "Size: " << vector_food.size() << endl;
        cout << "Draw" << endl;
        draw(class_food, output, vector_food, radius);

        k++;

        /// Remove old food
        vector_food.erase(remove_if(vector_food.begin(), vector_food.end(), [](FoodObj f){ return f.age > 5;}), vector_food.end());


        /// Food increase OpenCL

        int food_size = vector_food.size();
        for(int i = 0; i < food_size; i++){
            FoodObj &food = vector_food.at(i);

            if (!class_food.check_safety(vector_food, food)){
                food.age += 1;
            }

            if (food.age >= 4){
                food.grow_time = 0;
                continue;
            } else {
                food.grow_time += 1;
            }

            if (food.grow_time == 2){
                food.grow_time = 0;
                random_shuffle(class_food.direction.begin(), class_food.direction.end());

                for (auto direction : class_food.direction){
                    FoodObj new_food;
//                    new_food.age = 0;
                    new_food.pos.x = food.pos.x + direction.x;
                    new_food.pos.y = food.pos.y + direction.y;

                    if(new_food.pos.x >= 0 && new_food.pos.y >= 0 &&
                            new_food.pos.x <= width && new_food.pos.y <= height){
                        if (!class_food.find_in(vector_food, new_food)){
                            vector_food.push_back(new_food);
                            break;
                        }
                    }
                }
            }
        }
    }


    /// Create bio
    /// Moving bio to food
    /// Growth bio of eat or decrease

    return 0;
}


