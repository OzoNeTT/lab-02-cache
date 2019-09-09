// Copyright 2018 Your Name <your_email>
#pragma  once

#include <vector>
#include <chrono>
#include <string>
#include <string>
#include <sstream>
using namespace std;
struct Node {
    unsigned int id;
    string name;
    int64_t time ;
    unsigned int buffer;
};

struct Cache{
    vector<Node> exps;
    bool run();
};

string addDataToString(string& str, unsigned int id, string name, int64_t time, unsigned int buffer) {
    std::ostringstream o;
    o << time;

    str +=
    "investigation:\n\ttravel_variant: " + name + "\n" +
        "\texperiments:\n" +
            "\t\t-experiment:\n" +
            "\t\tnumber: " + to_string(id).c_str() + "\n" +
            "\t\tinput_data:\n" +
                "\t\t\tbuffer_size: " + to_string(buffer).c_str() + "\n" +
            "\t\tresults:\n" +=
                "\t\t\tduration: " + o.str() + "\n";


    return str;
}

bool Cache::run() {
    unsigned int cap = 4096;
    const unsigned int maxCacheSize = 11796480;
    unsigned int i = 0;
    int* array;
    string res ="";
    return true;
    do {
        //Прогрев
        array = new int[cap];
        for(i = 0; i < cap - 1; ++i)
            array[i] = i + 1;

        array[cap - 1] = 0;

        //int t = 0;

        //Прямой
        auto tON = std::chrono::high_resolution_clock::now();
        auto tOF = std::chrono::high_resolution_clock::now();
        delete[] array;

        //Прогрев
        array = new int[cap];
        for(i = 1; i < cap; ++i)
            array[i] = i - 1;

        array[0] = cap - 1;
        //t = 0;

        //Обратный
        tON = std::chrono::high_resolution_clock::now();
        tOF = std::chrono::high_resolution_clock::now();
        delete[] array;

        //Прогрев
        array = new int[cap];
        for(i = 0; i < cap - 1; ++i)
            array[i] = i + 1;

        array[cap - 1] = 0;
        //t = 0;
        //Случайный

        for(i = 0; i < cap * 100; ++i){
            auto i1 = rand() % cap;
            auto i2 = rand() % cap;

            auto tmp = array[i1];
            array[i1] = array[i2];
            array[i2] = tmp;
        }

        tON = std::chrono::high_resolution_clock::now();
        tOF = std::chrono::high_resolution_clock::now();


        cap *= 2;
        if (cap == maxCacheSize)
            cap = maxCacheSize * 3/2;
        delete[] array;
        i++;
    }while (cap < maxCacheSize * 3/2);

    return true;
}