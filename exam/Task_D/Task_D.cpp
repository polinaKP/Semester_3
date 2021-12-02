#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <vector>
#include <algorithm>

int main(){
    double vit_d, acat, anti_tg, neural_activity, mch, maxx, minn, dif;

    std::unordered_map<int, std::vector<double>> data;
    std::multimap<double, int> dif_id;
    std::vector<double> difs;
    std::vector<int> minimals;
    bool detected = false;

    int time_stamp, id;

    int n;
    std::cin >> n;

    for (int i = 0; i < n; i++){
        std::cin >> time_stamp >> id >> vit_d >> acat >> anti_tg >> neural_activity >> mch;
        data[id].push_back(neural_activity);
    }

    for (auto i : data){
        if (i.second.size() > 1){
            detected = true;
            maxx = *max_element(i.second.begin(), i.second.end());
            minn = *min_element(i.second.begin(), i.second.end());
            dif = maxx - minn;
            dif_id.insert({dif, i.first});
            difs.push_back(dif);
        }
    }

    if (detected){
        std::multimap<double, int>::iterator it;
        it = dif_id.begin();
        if (dif_id.size() == 1) std::cout << (*it).second << ' ';
        if (dif_id.size() == 2){
            minimals.push_back((*it).second);
            it++;
            minimals.push_back((*it).second);
            sort(minimals.begin(), minimals.end());
            for (auto i : minimals) std::cout << i << ' ';
            std::cout << std::endl;
        }
        if (difs.size() > 2){
            minimals.push_back((*it).second);
            it++;
            minimals.push_back((*it).second);
            it++;
            minimals.push_back((*it).second);
            sort(minimals.begin(), minimals.end());
            for (auto i : minimals) std::cout << i << ' ';
            std::cout << std::endl;
        }
    }
    else std::cout << -1 << std::endl;

    return 0;
}
