#include <iostream>
#include <vector>

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

template <typename T, typename iter_t>
iter_t search(const iter_t begin, const iter_t end, const T value){
    iter_t index = begin;
    for (index; index != end; index = std::next(index)){
        T value_new = *index;
        if (is_equal(value, value_new)){
            return index;
        }
    }
    return index;
};

template <typename T>
T partition(const T begin, const T end){
    const unsigned int size = std::distance(begin, end);
    T start_index = begin;
    std::advance(start_index, size / 2);
    auto start_value = *start_index;
    T begin_to_end = begin;
    T end_to_begin = end;
    while (true){
        auto value_1 = *begin_to_end;
        while (value_1 < start_value){
            begin_to_end++;
            value_1 = *begin_to_end;
        }
        auto value_2 = *end_to_begin;
        while (value_2 > start_value){
            end_to_begin--;
            value_2 = *end_to_begin;
        }
        if (begin_to_end >= end_to_begin){
            return end_to_begin;
        }
        auto swap = *begin_to_end;
        *begin_to_end = *end_to_begin;
        *end_to_begin = swap;
        begin_to_end++;
        end_to_begin--;
    }
}

template <typename T>
void sort(const T begin, const T end)
{
    if (begin < end){
        const T part = partition(begin, end - 1);
        sort(begin, part);
        const T part_next = part + 1;
        sort(part_next, end);
    }
}

class Check{
public:
    template <typename T, typename iter_t>
    static bool test_search_check(const T value, const iter_t begin, const iter_t end, const iter_t correct_result){
        const iter_t result = search(begin, end, value);
        if (result == correct_result)
            return true;
        else
            return false;
    }

    template <typename T, typename iter_t>
    static bool test_sort_check(const iter_t begin, const iter_t end, const std::vector<T>& correct_result){
        std::vector<T> result(end - begin);
        sort(begin, end);
        iter_t index = begin;
        unsigned int position = 0;
        while (index != end){
            result[position] = *index;
            index++;
            position++;
        }
        return (result == correct_result);
    }
};

void test_search(Check checking){
    std::vector<unsigned char> data_empty;
    if (checking.test_search_check(0, data_empty.begin(), data_empty.end(), data_empty.end()))
        std::cout << "There are no problems with Search in empty vector!" << std::endl;
    else std::cout << "There are problems with Search in empty vector!" << std::endl;

    std::vector<char> data_char{0, 1};
    if (checking.test_search_check(0, data_char.begin(), data_char.end(), data_char.begin()) &&
        checking.test_search_check(1, data_char.begin(), data_char.end(), data_char.end() - 1) &&
        checking.test_search_check(2, data_char.begin(), data_char.end(), data_char.end()))
        std::cout << "There are no problems with Search in char vector!" << std::endl;
    else std::cout << "There are problems with Search in char vector!" << std::endl;

    std::vector<double> data_double{0, 1};
    if (checking.test_search_check(0, &data_double[0], &data_double[1] + 1, &data_double[0]) &&
        checking.test_search_check(1, &data_double[0], &data_double[1] + 1, &data_double[1]) &&
        checking.test_search_check(1.0000001, &data_double[0], &data_double[1] + 1, &data_double[1]) &&
        checking.test_search_check(2, &data_double[0], &data_double[1] + 1, &data_double[1] + 1))
        std::cout << "There are no problems with Search in double vector!" << std::endl;
    else std::cout << "There are problems with Search in double vector!" << std::endl;
}

void test_sort(Check checking){
    std::vector<unsigned char> vector_empty;
    if (checking.test_sort_check(vector_empty.begin(), vector_empty.end(), vector_empty))
        std::cout << "There are no problems with Sort in empty vector!" << std::endl;
    else std::cout << "There are problems with Sort in empty vector!" << std::endl;

    std::vector<char> vector_char{2, 0, 1};
    if (checking.test_sort_check(vector_char.begin(), vector_char.end(), std::vector <char> {0, 1, 2}))
         std::cout << "There are no problems with Sort in char vector!" << std::endl;
    else std::cout << "There are problems with Sort in char vector!" << std::endl;

    std::vector<double> vector_double{2, 0, 1};
    if (checking.test_sort_check(vector_double.begin(), vector_double.end(), std::vector <double> {0, 1, 2}))
        std::cout << "There are no problems with Sort in double vector!" << std::endl;
    else std::cout << "There are problems with Sort in double vector!" << std::endl;
}

int main (int argc, char** argv){
    Check my_checker;
    test_search(my_checker);
    test_sort(my_checker);

    return 0;
}
