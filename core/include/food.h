#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <typeinfo>

using namespace std;


struct FoodObj{
    int id = 0;
    int type = 0;
    cv::Point position = {0, 0};
    int age = 0;
    int safety_points = 0;
    int growth_points = 0;
    int reproduction_points = 0;
    int health_points;

    std::mt19937 rng;
    std::uniform_int_distribution<int> rnd_w;
    std::uniform_int_distribution<int> rnd_h;
    std::uniform_int_distribution<int> rnd_type;
};

class Food{
public:
    Food();
    void create_food(int height, int width, vector<FoodObj> &vector_food, int** id_matrix);
    bool find_in(vector<FoodObj> v, FoodObj f);
    bool check_safety(vector<FoodObj> vector_food, FoodObj food);
    cv::Scalar age_color(int age);
    std::vector<cv::Point> direction;

    int d;
    int count_init_food;
};
