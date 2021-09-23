#include <iostream>
#include <set>
#include <random>
#include <fstream>

class State {
public:
    State(){};

    virtual bool contains(int s) const = 0;
};

class DiscreteState: public State {
private:
    int const state;

public:
    DiscreteState(int state): state(state) { }

    bool contains(int s) const override {
        return s == state;
    }
};

class SegmentState: public State {
private:
    int const beg, end;

public:
    SegmentState(): beg(0), end(-1) { }
    SegmentState(int beg, int end): beg(beg), end(end) { }

    bool contains(int s) const override {
        return s >= beg && s <= end;
    }
};

class SetState: public State {
private:
    std::set<int> const states;

public:
    SetState(): states() { }
    SetState(std::set<int> const &src): states(src) { }

    bool contains(int s) const override {
        return states.count(s) > 0;
    }
};

class CrossingState: public State{
private:
    State *a_, *b_;

public:
    CrossingState(State *ptr_a, State *ptr_b): a_(ptr_a), b_(ptr_b) {}

    bool contains(int s) const override {
        return a_ -> contains(s) && b_ -> contains(s);
    }
} ;

class CombiningState: public State{
private:
    State *a_, *b_;

public:
    CombiningState(State *ptr_a, State *ptr_b): a_(ptr_a), b_(ptr_b) {}

    bool contains(int s) const override {
        return a_ -> contains(s) || b_ -> contains(s);
    }
} ;

class Continuous_with_gaps: public State{
private:
    State *a_, *b_;

public:
    Continuous_with_gaps(State *ptr_a, State *ptr_b): a_(ptr_a), b_(ptr_b) {}

    bool contains(int s) const override {
        return a_ -> contains(s) && !(b_ -> contains(s));
    }
} ;

class Continuous_with_complements: public State{
private:
    State *a_, *b_;

public:
    Continuous_with_complements(State *ptr_a, State *ptr_b): a_(ptr_a), b_(ptr_b) {}

    bool contains(int s) const override {
        return a_ -> contains(s) && b_ -> contains(s);
    }
} ;

class Continuous_with_gaps_and_complements: public State{
private:
    State *a_, *b_, *c_;

public:
    Continuous_with_gaps_and_complements(State *ptr_a, State *ptr_b, State *ptr_c): a_(ptr_a), b_(ptr_b), c_(ptr_c) {}

    bool contains(int s) const override {
        return a_ -> contains(s) && !(b_ -> contains(s)) && c_ -> contains(s);
    }
} ;

class Factory {
public:
    static State* create_discrete(int state){
        return new DiscreteState(state);
    }

    static State* create_segment(int beg, int end){
        return new SegmentState(beg, end);
    }

    static State* create_set(std::set<int> const &states){
        return new SetState(states);
    }
};

class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count): seed(seed), test_min(test_min),test_max(test_max), test_count(test_count) { }

    float operator()(State const &s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min,test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good)/static_cast<float>(test_count);
    }
};

int main(int argc, const char * argv[]) {
    std::ofstream fout("output_2.txt");

    DiscreteState discrete(1);
    SegmentState segment(0,10);
    SetState set({1, 3, 5, 7, 23, 48, 57, 60, 90, 99});

    Continuous_with_gaps gap(&segment, &set);
    Continuous_with_complements complement(&discrete, &segment);
    Continuous_with_gaps_and_complements gap_complement(&segment, &set, &discrete);
    CrossingState cross(&discrete, &segment);
    CombiningState combine(&segment, &set);

    ProbabilityTest pt(10,0,100,1000000);
    std::cout << pt(discrete) << std::endl;
    std::cout << pt(segment) << std::endl;
    std::cout << pt(set) << std::endl;
    std::cout << pt(gap) << std::endl;
    std::cout << pt(complement) << std::endl;
    std::cout << pt(cross) << std::endl;
    std::cout << pt(combine) << std::endl;

    for (int i = 0; i <= 1000000; i += 1000){
        ProbabilityTest pt(10,0,100, i);
        std::cout << i << " " << pt(combine) << std::endl;
        fout << i << " " << pt(combine) << std::endl;
    }
    fout.close();
    return 0;
}
