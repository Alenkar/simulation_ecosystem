#include <food.h>


Food::Food(int height, int width){
    cout << "Init" << endl;

    count_init_food = 50;
    d = 5;
    direction = {{-d, -d}, {0, -d}, {d, -d},
                        {-d, 0}, {0, 0}, {d, 0},
                        {-d, d}, {0, d}, {d, d}};

    std::random_device rd;
    std::mt19937 rng(rd());
    rnd_w = std::uniform_int_distribution<int>(0, width);
    rnd_h = std::uniform_int_distribution<int>(0, height);
    rnd_type = std::uniform_int_distribution<int>(0, 3);

}


void Food::create_food(int height, int width, array2D *array, int count){
    /// Create food
    int counter = 0;
    do{
        int pos_x = rnd_w(rng);
        int pos_y = rnd_h(rng);

        if(pos_x >= 0 && pos_y >= 0 && pos_x <= width && pos_y <= height){
            if (array[0][pos_x][pos_y] == 0){
                /// rand_type
                array[0][pos_x][pos_y] = 1;
                array[1][pos_x][pos_y] = 1;
                array[5][pos_x][pos_y] = 1;
                counter++;
            }
        }
    }while(counter != count_init_food);
}

void Food::create_bact(int height, int width, array2D *array, int count){
    /// Create bacteries
    int counter = 0;
    do{
        int pos_x = rnd_w(rng);
        int pos_y = rnd_h(rng);

        if(pos_x >= 0 && pos_y >= 0 && pos_x <= width && pos_y <= height){
            if (array[0][pos_x][pos_y] == 0){
                /// rand_type
                array[0][pos_x][pos_y] = 2;
                array[1][pos_x][pos_y] = 1;
                array[5][pos_x][pos_y] = 5;
                counter++;
            }
        }
    }while(counter != count);
}


bool Food::find_in(vector<FoodObj> v, FoodObj f){
    for(auto vi : v){
        if (vi.position == f.position){
            return true;
        }
    }
    return false;
}


bool Food::check_safety(vector<FoodObj> vector_food, FoodObj food){
    int c = 0;
    for (auto dir : direction){
        FoodObj around_food = food;
        around_food.position += dir;
        if(find_in(vector_food, around_food)){
            c++;
        }
    }
    if (c > 6){
        return true;
    } else {
        return false;
    }
}


cv::Scalar Food::age_color(int age){
    if (0 <= age && age < 2){
        return cv::Scalar(0, 255, 0);
    } else if (2 <= age && age < 4){
        return cv::Scalar(0, 255, 255);
    } else if (4 <= age && age < 6) {
        return cv::Scalar(0, 0, 255);
    } else if (6 <= age){
        return cv::Scalar(255, 0, 0);
    }
}


void Food::step()
{

}

