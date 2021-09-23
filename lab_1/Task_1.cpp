#include <iostream>
#include <vector>
#include <fstream>

typedef unsigned int ui;

class Image{
public:
    Image(): data_{nullptr}, height_{0}, width_{0}, depth_{0}{ //нулевой указатель
    }

    Image(ui *raw_str, size_t height, size_t width, size_t depth): data_{nullptr}, height_{height}, width_{width}, depth_{depth}{
        allocate(height, width, depth);
        copy_from(raw_str, height, width, depth);
    }

    Image(size_t height, size_t width, size_t depth): data_{nullptr}, height_{height}, width_{width}, depth_{depth}{
        allocate(height, width, depth);
    }

    Image(Image const &other){
        height_ = other.height_;
        width_ = other.width_;
        depth_ = other.depth_;
        img_ = other.img_;
    }

    void print(){
        for (auto k = 0u; k < depth_; ++k){
            for (auto i = 0u; i < height_; ++i){
                for (auto j = 0u; j < width_; ++j){
                    std::cout << img_[i][j][k] << " ";
                }
            std::cout << '\n';
            }
        std::cout << '\n';
        }
    }

    void crop(ui xi, ui xj, ui yi, ui yj){
        for (int k = 0; k < xi; ++k)
            for (int i = 0; i < depth_; ++i)
                for (int j = 0; j < width_; ++j){
                    img_[k][j][i] = 0;
                }
        for (int k = xj; k < height_; ++k)
            for (int i = 0; i < depth_; ++i)
                for (int j = 0; j < width_; ++j){
                    img_[k][j][i] = 0;
                }
        for (int k = 0; k < yi; ++k)
            for (int i = 0; i < depth_; ++i)
                for (int j = 0; j < height_; ++j){
                    img_[j][k][i] = 0;
                }
        for (int k = yj; k < width_; ++k)
            for (int i = 0; i < depth_; ++i)
                for (int j = 0; j < height_; ++j){
                    img_[j][k][i] = 0;
        }
    }

    at(ui x, ui y, ui z){
        return img_[x][y][z];
    }

    void to_gs(){
        auto sum = 0;
        for (auto i = 0; i < height_; ++i)
            for (auto j = 0; j < width_; ++j){
                for (auto k = 0; k < depth_; ++k)
                    sum += img_[i][j][k];
                for (auto k = 0; k < depth_; ++k)
                    img_[i][j][k] = sum / 3;
            }
    }

    void clear() {
        data_ = nullptr;
        height_ = 0;
        width_ = 0;
        depth_ = 0;
        delete[] data_;
    }

    ~Image(){
        clear();
    }

private:
    void allocate(size_t h, size_t w, size_t d){
        std::vector<std::vector<std::vector<ui>>>img_new(h, std::vector<std::vector<ui>>(w, std::vector<ui>(d, 0)));
        img_ = img_new;
    }

    void copy_from(ui *data, size_t h, size_t w, size_t d){
        for (auto k = 0u; k < h; ++k)
            for (auto i = 0u; i < w; ++i)
                for (auto j = 0u; j < d; ++j) {
                    img_[k][i][j] = data[j + i*d + k*d*w];
                }
    }

    char *data_ = nullptr;
    std::vector<std::vector<std::vector<ui>>>img_;
    size_t height_ = 0, width_ = 0, depth_ = 0;
};

int main(int argc, char** argv){
    ui depth = 3;
    ui height = 8;
    ui width = 4;
    ui length = depth * height * width;
    ui* raw = new ui[length];

    for (ui i = 0; i < length; i++)
        raw[i] = i;

    Image img(raw, height, width, depth);
    img.print();
    return 0;
}
