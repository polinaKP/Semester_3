#include <random>
#include <vector>
#include <chrono>

#include <iostream>

class RandomGenerator{
public:
    static std::vector<int> create_sequence(size_t n, int min, int max){
        //std::mt19937_64 engine(42); //Даёт одни и те же данные
        std::mt19937_64 engine(std::random_device{}()); //Реально случайные (но на современных ПК и ОС)
        std::uniform_int_distribution<int> distr(min, max);
        std::vector<int> v(n);
        for (auto i = 0u; i < n; ++i)
            //v[i] = distr(engine);
            v[i] = std::random_device{}();
    return v;
    }
private:

};

class TimeMeasure{
public:
    void start(){
        start_time_ = std::chrono::high_resolution_clock::now();
    }
    void stop(){
        stop_time_ = std::chrono::high_resolution_clock::now();
    }
    size_t elapsed() const{
        auto elapsed_time_ = stop_time_ - start_time_;
        return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time_).count();
    }
private:
    std::chrono::high_resolution_clock::time_point start_time_, stop_time_;
};

int main(int arg, char** argv){
    TimeMeasure timer;

    timer.start();
    for (auto i = 0; i < 10000; ++i){
        auto random_vec = RandomGenerator::create_sequence(100, 1, 6);
    }
    /*for (auto const& e : random_vec);
        std::cout << e << " ";
    std::cout << std::endl;*/
    timer.stop();

    std::cout << "Eta (x10000): " << timer.elapsed() << std::endl;
    return 0;
}
