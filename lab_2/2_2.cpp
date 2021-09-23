#include <iostream>

#include <vector>
#include <string>
#include <chrono>

class DemoStr{
public:
    DemoStr(): data_{nullptr}, length_{0}, capacity_{0}{ //нулевой указатель
    }

    DemoStr(DemoStr const& other): length_{other.length_}{
        allocate(other.length_);
        copy_from(other.data_, other.length_);
    }

    DemoStr(char const* raw_str, size_t length): length_{length}{
        allocate(length);
        copy_from(raw_str, length);
        reformate();
    }

    explicit DemoStr(int value) {
        length_ = get_value_length(value);
        capacity_ = length_;
        allocate(length_);
        auto it = &data_[length_ - 1];
        it = 0;
        while(value > 0){
            *it = static_cast<char>(value % 10) + '0'; // value % 10 + 0x30
            --it;
            value /= 10;
        }
    }

    void print() const {
        for (auto i = 0u; i < length_; ++i)
            std::cout << "\n";
    }

    //index < length_ - 1
    char at(size_t index) const{
        return data_[index];
    }

    bool equal(DemoStr const &other) const{
        if (other.length_ != length_)
            return false;
        for (auto i = 0u; i < length_; ++i)
            if (data_[i] != other.data_[i])
                return false;
        return true;
    }

    void read_from(std::istream &in){
        char tmp;
        while (in >> tmp) {
            if (length_ >= capacity_)
                reallocate((capacity_ == 0) ? 1 : capacity_ * 2);
            data_[length_] = tmp;
            ++length_;
        }
        reformate();
    }

    void shrink_to_fit(){
        capacity_ = length_;
    }

    auto size() const{
        return length_;
    }

    auto data() const{
        return data_;
    }

    void clear() {
        data_ = nullptr;
        length_ = 0;
        capacity_ = 0;
        delete[] data_;
    }

    ~DemoStr(){ // деструктор
        clear();
    }

protected:
    size_t get_length_string(DemoStr const &new_str){
        return new_str.length_;
    }

    void set_length(size_t new_length){
        length_ = new_length;
    }

    void reallocate(size_t n){
        auto old_data = data_;

        data_ = new char [n];
        capacity_ = n;

        copy_from(old_data, std::min(n, length_));

        delete [] old_data;
    }

    void copy(char const* add_str, char* new_str, size_t n){
        for (auto i = 0u; i < n; ++i)
            new_str[i] = add_str[i];
    }

private:
    void reformate(){
        for(auto i = 0u; i < length_; ++i)
            data_[i] = (i % 2 == 1) ? std::tolower(data_[i]):
                                      std::toupper(data_[i]);
    }

    size_t get_value_length(int value){
        auto len = 0ull;
        while (value > 0){
            value /= 10;
            ++len;
        }
        return len;
    }

    void allocate(size_t n){
        data_ = new char[n];
    }

    void copy_from(char const* data, size_t n){
        for (auto i = 0u; i < n; ++i)
            data_[i] = data[i];
    }

    friend std::ostream& operator << (std::ostream& out, DemoStr const& v);

    char *data_ = nullptr;
    size_t length_ = 0, capacity_ = 0;
};

class NewStr: public DemoStr{
public:
    using DemoStr::DemoStr;

    void append(DemoStr const &add_str){
        auto len_new = get_length_string(add_str);
        reallocate(len_new + size());
        copy(add_str.data(), data() + size(), len_new);
        set_length(len_new + size());
    }

    std::vector<NewStr> split(char simbol){
        auto length = size();
        std::vector<NewStr> words = create_vectors(length);
        int k = 0;
        NewStr new_string(&(data()[0]), 1);
        new_string.clear();
        for (auto i = 0u; i < length; ++i){
            if (data()[i] != simbol){
                DemoStr new_data(&(data()[i]), 1);
                new_string.append(new_data);
            }
            else{
                words[k] = new_string;
                std::cout << words[k] << " " << '\n';
                k++;
                new_string.clear();
            }
        }
        words[k] = new_string;
        std::cout << words[k] << " " << '\n';
        return words;
    }

    std::vector<NewStr> create_vectors(int length){
        std::vector<NewStr> vectors(length);
        return vectors;
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

std::ostream& operator << (std::ostream& out, DemoStr const& v){
    for (auto i = 0ul; i < v.length_; ++i)
        out << v.data_[i];
    out << "\n";
    return out;
}

int main(int argc, char** argv){
    TimeMeasure timer;

    timer.start();

    char demo[] = "Hello class 123";
    NewStr from_str(demo, sizeof(demo) - 1);
    std::cout << from_str << '\n';

    char add_string[] = "!!!";
    NewStr add_str(add_string, sizeof(add_string) - 1);
    from_str.append(add_str);
    std::cout << from_str << '\n';

    char simbol = ' ';
    std::vector<NewStr> new_vector = from_str.split(simbol);

    timer.stop();

    std::cout << "Program run time: " << timer.elapsed() << " ms." << std::endl;
    return 0;
}

