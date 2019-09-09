// Copyright 2018 Your Name <your_email>
#pragma  once

#include <vector>
#include <chrono>
#include <string>

struct Node {
    unsigned int id;
    std::string name;
    int64_t time ;
    unsigned int buffer;
};

struct Cache{
    std::vector<Node> exps;
    bool run();
};

bool Cache::run() {
    return false;
}