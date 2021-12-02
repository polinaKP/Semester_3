#include <vector>
#include <iostream>

template<typename T>
void log(T v){
    std::cout << v << " ";
}

int main (int arch, char** argv){
    std::vector<double> v;
    std::vector<double> v_c(10, 3.14);
    std::vector<double> v_il{1, 2, 3, 4, 5, 6, 7};

    v.reserve(5);
    v_c.reserve(4);
    v_il.reserve(10);

    v_il.shrink_to_fit();

    log(v.size()); log(v.capacity()); log("\n");
    log(v_c.size()); log(v_c.capacity()); log("\n");
    log(v_il.size()); log(v_il.capacity()); log("\n");

    v_il[4] = 1.5;

    for (auto const& element: v_il)
        log(element);
    log("\n");

    for (auto it = v_c.begin(); it != v_c.end(); ++it)
        log(*it);
    log("\n");

    return(0);
}
