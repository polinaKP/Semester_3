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
        return a_ -> contains(s) || b_ -> contains(s);
    }
} ;

class Continuous_with_gaps_and_complements: public State{
private:
    State *a_, *b_, *c_;

public:
    Continuous_with_gaps_and_complements(State *ptr_a, State *ptr_b, State *ptr_c): a_(ptr_a), b_(ptr_b), c_(ptr_c) {}

    bool contains(int s) const override {
        return (a_ -> contains(s) || c_ -> contains(s)) && !(b_ -> contains(s));
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

class Check{
public:
    static bool test(State* state, int point, bool correct_result){
        bool result = (*state).contains(point);
        if (result != correct_result)
            return false;
    return true;
    }
};

void test(Check checking)
{
    DiscreteState discrete(1);
    if (checking.test(&discrete, 0, false) && checking.test(&discrete, 1, true) && checking.test(&discrete, 2, false))
        std::cout << "There are no problems with DiscreteState!" << std::endl;
    else std::cout << "There are problems with DiscreteState!" << std::endl;

    SegmentState segment(-1, 0);
    if (checking.test(&segment, -2, false) && checking.test(&segment, -1, true) && checking.test(&segment, 0, true) && checking.test(&segment, 1, false))
        std::cout << "There are no problems with SegmentState!" << std::endl;
    else std::cout << "There are problems with SegmentState!" << std::endl;

    SetState set(std::set<int>{0, 2, 3, 5});
    if (checking.test(&set, -1, false) && checking.test(&set, 0, true) && checking.test(&set, 1, false) && checking.test(&set, 2, true) &&
        checking.test(&set, 3, true) && checking.test(&set, 4, false) && checking.test(&set, 5, true) && checking.test(&set, 6, false))
        std::cout << "There are no problems with SetState!" << std::endl;
    else std::cout << "There are problems with SetState!" << std::endl;

    Continuous_with_gaps gap(&segment, &set);
    if (checking.test(&gap, -2, false) && checking.test(&gap, -1, true) && checking.test(&gap, 0, false) && checking.test(&gap, 1, false) &&
        checking.test(&gap, 3, false) && checking.test(&gap, 4, false))
        std::cout << "There are no problems with Continuous_with_gaps!" << std::endl;
    else std::cout << "There are problems with Continuous_with_gaps!" << std::endl;

    Continuous_with_complements complement(&set, &segment);
    if (checking.test(&complement, -2, false) && checking.test(&complement, -1, true) && checking.test(&complement, 0, true) &&
        checking.test(&complement, 1, false) && checking.test(&complement, 2, true) && checking.test(&complement, 3, true) &&
        checking.test(&complement, 4, false) && checking.test(&complement, 5, true) && checking.test(&complement, 6, false))
        std::cout << "There are no problems with Continuous_with_complements!" << std::endl;
    else std::cout << "There are problems with Continuous_with_complements!" << std::endl;

    Continuous_with_gaps_and_complements gap_complement(&segment, &set, &discrete);
    if (checking.test(&gap_complement, -2, false) && checking.test(&gap_complement, -1, true) && checking.test(&gap_complement, 0, false) &&
        checking.test(&gap_complement, 1, true) && checking.test(&gap_complement, 2, false) && checking.test(&gap_complement, 3, false) &&
        checking.test(&gap_complement, 4, false) && checking.test(&gap_complement, 5, false) && checking.test(&gap_complement, 6, false))
        std::cout << "There are no problems with Continuous_with_gaps_and_complements!" << std::endl;
    else std::cout << "There are problems with Continuous_with_gaps_and_complements!" << std::endl;

    CrossingState cross(&set, &segment);
    if (checking.test(&cross, -2, false) && checking.test(&cross, -1, false) && checking.test(&cross, 0, true) &&
        checking.test(&cross, 1, false) && checking.test(&cross, 2, false) && checking.test(&cross, 3, false) &&
        checking.test(&cross, 4, false) && checking.test(&cross, 5, false) && checking.test(&cross, 6, false))
        std::cout << "There are no problems with CrossingState!" << std::endl;
    else std::cout << "There are problems with CrossingState!" << std::endl;

    CombiningState combine(&discrete, &set);
    if (checking.test(&combine, -1, false) && checking.test(&combine, 0, true) && checking.test(&combine, 1, true) &&
        checking.test(&combine, 2, true) && checking.test(&combine, 3, true) && checking.test(&combine, 4, false) &&
        checking.test(&combine, 5, true) && checking.test(&combine, 6, false))
        std::cout << "There are no problems with CombiningState!" << std::endl;
    else std::cout << "There are problems with CombiningState!" << std::endl;
}

int main(int argc, const char * argv[]) {
    Check unit_test;
    test(unit_test);
    return 0;
}
