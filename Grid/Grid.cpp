#include <iostream>

struct capacity_overflow{};

template<typename t_data>
class Grid {
private:
    Grid *memory_;
    size_t x_size_, y_size_, length_;
    t_data value_;

    size_t get_index(size_t x, size_t y) const{
        return y * x_size_ + x;
    };

    void clear(){
        value_ = get_average();
        size_t length = length_;
        for (size_t i = 0; i < length; i++){
            Grid& element = memory_[i];
            if(element.is_subgrid()) element.clear();
            else{
                delete[] element.memory_;
                element.memory_ = nullptr;
            }
        }
    };

public:
    Grid(size_t x_size, size_t y_size, t_data value): x_size_{x_size}, y_size_{y_size}, value_{value}{
        length_ = x_size_ * y_size_;
        if (length_ == 0) memory_ = nullptr;
        else memory_ = new Grid[length_];
    };

    Grid(): Grid<t_data>(0, 0, 0) {}

    Grid(size_t x_size, size_t y_size): Grid<t_data>(x_size, y_size, 0) {}

    t_data operator()(size_t x_idx, size_t y_idx) const{
        if (x_idx > get_xsize() || y_idx > get_ysize()){
            throw capacity_overflow();
        }
        size_t index = get_index(x_idx, y_idx);
        Grid& element = memory_[index];
        if (element.is_subrid()) return element.get_average();
        else return element.value_;
    }

    t_data get_average() const{
        if (length_ == 0) return 0;
        else{
            t_data sum = 0;
            for (size_t i = 0; i < length_; ++ i){
                t_data number;
                const Grid& element = memory_[i];
                if (element.is_subgrid()) number = element.get_average();
                else number = element.value_;
                sum += number;
            }
            return sum / length_;
        }
    }

    size_t get_xsize() const{
        return x_size_;
    }

    size_t get_ysize() const{
        return y_size_;
    }

    size_t get_value() const{
        return value_;
    }

    Grid& operator=(t_data value){
        for (int i = 0; i < length_; ++i)
            memory_[i] = value;
        return *this;
    }

    void copy(const Grid& copy_grid){
        clear();
        delete[] memory_;
        length_ = copy_grid.length_;
        x_size_ = copy_grid.x_size_;
        y_size_ = copy_grid.y_size_;
        memory_ = new Grid[length_];
        for (unsigned int i = 0; i < length_; i++){
                Grid& element = memory_[i];
                Grid& copy_element = copy_grid.memory_[i];
                if(copy_element.is_subgrid()){
                    element.copy(copy_element);
                }
                else{
                    element.length_ = 0;
                    element.x_size_ = 0;
                    element.y_size_ = 0;
                    element.value_ = copy_element.value_;
                }
            }
    }

    bool is_subgrid() const{
        if(memory_ == nullptr) return false;
        else return true;
    }

    Grid& make_subgrid(size_t x_idx, size_t y_idx, size_t x_sub_size, size_t y_sub_size){
        t_data value = (*this)(x_idx, y_idx);
        size_t index = get_index(x_idx, y_idx);
        Grid& new_grid = memory_[index];
        new_grid.x_size_ = x_sub_size;
        new_grid.y_size_ = y_sub_size;
        size_t length = x_sub_size * y_sub_size;
        new_grid.length_ = length;
        new_grid.memory_ = new Grid[length];
        new_grid = value;
        return *this;
    }

    Grid& collapse_subgrid(size_t x_idx, size_t y_idx, size_t x_sub_size, size_t y_sub_size){
        t_data value = (*this)(x_idx, y_idx);
        size_t index = get_index(x_idx, y_idx);
        Grid& element = memory_[index];
        element.clear();
        element.length_ = 0;
        element.x_size_ = 0;
        element.y_size_ = 0;
        delete[] element.memory_;
        element.memory_ = nullptr;
        element.value_ = value;
        return *this;
    }

    Grid& get_subgrid(size_t x_idx, size_t y_idx){
        size_t index = get_index(x_idx, y_idx);
        Grid& subgrid = memory_[index];
        return subgrid;
    }

    Grid const& get_subgrid(size_t x_idx, size_t y_idx) const{
        size_t index = get_index(x_idx, y_idx);
        const Grid& subgrid = memory_[index];
        return subgrid;
    }


    friend std::ostream& operator<<(std::ostream &out, Grid const &grid){
        for (size_t i = 0; i < grid.get_ysize(); ++i){
            for (size_t j = 0; j < grid.get_xsize(); ++j){
                out << grid.memory_[grid.get_index(i, j)] << " ";
            }
            out << std::endl;
        }
        return out;
    }

    /*friend std::istream& operator>>(std::istream &in, Grid<int> &grid){
        grid.clear();
        size_t index = 0;
        size_t value;
        while(in.good() && index < grid.length_){
            in >> value;
            grid.memory_[index] = value;
            ++ index;
        }
        return in;
    }*/

    ~Grid(){
        delete[] memory_;
    }
};

template <typename T>
bool is_equal(const T value_1, const T value_2){
    return (value_1 == value_2);
}

template <>
bool is_equal(const double value_1, const double value_2)
{
    const double error = 0.00005;
    return (std::abs(value_2 - value_1) < error);
}

class Check{
public:

    static bool test_get_average_check(const Grid<int> grid, const size_t correct_result){
        const size_t result = grid.get_average();
        if (result == correct_result) return true;
        else return false;
    }

    static bool test_get_average_check(const Grid<double> grid, const size_t correct_result){
        const size_t result = grid.get_average();
        if (is_equal(result, correct_result)) return true;
        else return false;
    }

};

void test_get_average(Check checking){
    const Grid<int> data_empty;
    const Grid<int> data_one_element(1, 1);
    const Grid<double> data_double(1, 2);

    if (checking.test_get_average_check(data_empty, 0.0) &&
        checking.test_get_average_check(data_one_element, 0.0) &&
        checking.test_get_average_check(data_double, 0.0))
        std::cout << "There are no problems with Get_average!" << std::endl;
    else std::cout << "There are problems with Get_average!" << std::endl;
}

int main(int argc, const char * argv[]) {
    Check my_checker;
    test_get_average(my_checker);

    return 0;
}
