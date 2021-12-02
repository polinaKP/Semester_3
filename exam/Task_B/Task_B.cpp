#include <functional>
#include <list>
#include <array>
#include <cmath>
#include <iostream>

template <typename K, typename V, typename H = std::hash<K>>
class AssociativeArray {
public:
    using value_type = std::pair<K const, V>;
    using reference = std::pair<K const, V>&;
    using const_reference = std::pair<K const, V> const &;
    using pointer = std::pair<K const, V>*;
    using iterator = typename std::list<value_type>::iterator;
    using const_iterator = typename std::list<value_type>::const_iterator;

    AssociativeArray() {}

    iterator insert(const K k, V && v){
        iterator itt;
        int hesh = H{}(k);

        for(auto it = arr[abs(hesh%50000)].begin();it!=arr[abs(hesh%50000)].end();it++){
            if((*it).first == k){
                (*((*it).second)).second = std::forward<V>(v);
                return (*it).second;}}

        value_type para1(k, std::forward<V>(v));
        lst.push_back(std::forward<value_type>(para1));
        std::pair<K, iterator> para2;
        para2.first = k;
        para2.second = prev(lst.end());
        arr[abs(hesh%50000)].push_back(para2);
        return prev(lst.end());
        }

    iterator find(K k){
        int hesh = H{}(k);
        for(auto itt = arr[abs(hesh%50000)].begin();itt!=arr[abs(hesh%50000)].end();itt++){
            if((*itt).first == k){
                return (*itt).second;}}
        return lst.end();}

    const_iterator find(K k) const{
        int hesh = H{}(k);
        for(auto itt = arr[abs(hesh%50000)].begin();itt!=arr[abs(hesh%50000)].end();itt++){
            if((*itt).first == k){
                return (*itt).second;}}
        return lst.cend();}


    iterator erase(K k){
        int hesh = H{}(k);
        auto it = lst.begin();
        for(auto itt = arr[abs(hesh%50000)].begin();itt!=arr[abs(hesh%50000)].end();itt++){
            if((*itt).first == k){
                it = (*itt).second;
                arr[abs(hesh%50000)].erase(itt);
                return lst.erase(it);}}
        return lst.end();
        }

    iterator erase(iterator itt){
        int hesh = H{}((*itt).first);
        for(auto it = arr[abs(hesh%50000)].begin();it!=arr[abs(hesh%50000)].end();it++){
            if((*it).first == (*itt).first){
                arr[abs(hesh%50000)].erase(it);
                break;}}
        return lst.erase(itt);
    }

    iterator begin(){
        return lst.begin();}
    iterator end(){
        return lst.end();}
    const_iterator begin() const{
        return lst.cbegin();}
    const_iterator end() const{
        return lst.cend();}

private:
    std::array<std::list<std::pair<K, iterator>>, 50000> arr;
    std::list<value_type> lst;
};
