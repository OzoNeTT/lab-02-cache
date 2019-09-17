// Copyright 2018 Your Name <your_email>

#include <../include/cache.hpp>

using namespace std;

int main() {
    int min_kb, max_kb;
    string travel;
    min_kb = 128;
    max_kb = 6144;
    Experiments exp(min_kb, max_kb);
    exp.start();
    return 0;
}