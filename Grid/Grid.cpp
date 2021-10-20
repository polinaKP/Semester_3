#include <iostream>

template<typename t_data>
class Grid {
private:
    t_data *memory_;
    size_t x_size_, y_size_;

    size_t get_index(size_t x, size_t y) const{
        return x * x_size_ + y;
    };

    size_t get_length(){
        return x_size_ * y_size_;
    }

    void clear(){
        for (size_t i = 0; i < get_length(); ++i)
            memory_[i] = 0;
    };

public:
    Grid(size_t x_size, size_t y_size): x_size_{x_size}, y_size_{y_size}{
        memory_ = new t_data[x_size_ * y_size_];
    };

    t_data operator()(size_t x_idx, size_t y_idx) const{
        return memory_[get_index(x_idx, y_idx)];
    }

    t_data& operator()(size_t x_idx, size_t y_idx){
        return memory_[x_idx * x_size_ + y_idx];
    }

    size_t get_xsize() const{
        return x_size_;
    }

    size_t get_ysize() const{
        return y_size_;
    }

    Grid& operator=(t_data value){
        for (int i = 0; i < get_length(); ++i)
            memory_[i] = value;
        return *this;
    }

    void copy(t_data const* enter_memory, size_t n){
        clear();
        size_t min = min(get_length(), n);
        for (size_t i = 0; i < min; ++i)
            memory_[i] = enter_memory[i];
    }

    bool is_subrid(size_t x_idx, size_t y_idx) const;

    friend std::ostream& operator<<(std::ostream &out, Grid const &grid){
        for (size_t i = 0; i < grid.get_ysize(); ++i){
            for (size_t j = 0; j < grid.get_xsize(); ++j){
                out << grid.memory_[grid.get_index(i, j)] << " ";
            }
            out << std::endl;
        }
        return out;
    }

    friend std::istream& operator>>(std::istream &in, Grid &grid){
        grid.clear();
        size_t index = 0;
        size_t value;
        while(index < grid.get_length()){
            in >> value;
            grid.memory_[index] = value;
            ++ index;
        }
        return in;
    }

    ~Grid(){
        delete[] memory_;
    }
};

int main(int argc, const char * argv[]) {
    Grid<int> grid_1(4, 2);
    std::cout << grid_1(1, 0) << std::endl;
    return 0;
}
