#include <iostream>
#include <vector>
#include <algorithm>

bool comp(std::pair<std::string, int> a, std::pair<std::string, int>  b){
    if(a.first.substr(6,4) != b.first.substr(6,4)){
        return a.first.substr(6,4) < b.first.substr(6,4);
    }
    if(a.first.substr(3,2) != b.first.substr(3,2)){
        return a.first.substr(3,2) < b.first.substr(3,2);
    }
    if(a.first.substr(0,2) != b.first.substr(0,2)){
        return a.first.substr(0,2) < b.first.substr(0,2);
    }
    return a.second <= b.second;
}

bool str_comp(std::string a, std::string b){
    if(a.substr(6,4) != b.substr(6,4)){
        return a.substr(6,4) < b.substr(6,4);
    }
    if(a.substr(3,2) != b.substr(3,2)){
        return a.substr(3,2) < b.substr(3,2);
    }
    else{
        return a.substr(0,2) < b.substr(0,2);
    }
}

std::vector<std::pair<std::string,int>>::iterator bin_poisk(std::vector<std::pair<std::string,int>>::iterator itt_beg, std::vector<std::pair<std::string,int>>::iterator itt_end, const std::string& s){
    if(itt_beg != itt_end && next(itt_beg)!= itt_end){
        auto it = itt_beg;
        advance(it, distance(itt_beg, itt_end)/2);
        if(str_comp(s,(*it).first)){
           return bin_poisk(itt_beg,it,s);
        }
        else{
            return bin_poisk(it,itt_end,s);
        }
    }
    else{
        return itt_beg;
    }
}

void print(std::vector<int> count) {
    for (int i : count){
        std::cout << i << '\n';
    }
}

int main()
{
    std::string s;
    int n,m;
    int max = 0;
    std::cin>>n;

    std::vector<std::pair<std::string,int>> data(n);
    std::vector<std::pair<std::string,int>> vec;
    std::pair<std::string,int> p;
    for (int i = 0; i < n; i++){
        std::cin >> s;
        std::cin >> m;
        p.first = s;
        p.second = m;
        if (m > max){
            max = m;
        }
        data[i] = p;
    }

    sort(data.begin(),data.end(),comp);

    m = data[0].second;
    for(long long unsigned int i = 0; i < data.size(); i++){
        if (i != data.size() - 1 && data[i].first != data[i + 1].first){
            if (data[i].second < m){
                data[i].second = m;
            }
            else{
                m = data[i].second;
            }
            vec.push_back(data[i]);
        }
    }

    std::cin >> n;
    std::vector<int> count;
    for(int i = 0; i < n; i++){
        std::cin >> s;

        if (str_comp(vec[vec.size()-1].first,s)){
            count.push_back(max);
            continue;
        }
        if (str_comp(s,vec[0].first)){
            count.push_back(0);
            continue;
        }

        count.push_back((*bin_poisk(vec.begin(),prev(vec.end()),s)).second);
    }

    print(count);

    return 0;
}
