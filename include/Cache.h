// Copyright 2018 Your Name <your_email>
#pragma  once

#include <vector>
#include <chrono>
#include <string>
#include <string>
#include <sstream>
#include <fstream>
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

void addDataToString(string& str, unsigned int id, string name, int64_t time, unsigned int buffer) {
    std::ostringstream o;
    o << time;

    str +=
    "investigation:\n\ttravel_variant: " + name + "\n" +
        "\texperiments:\n" +
            "\t\t-experiment:\n" +
            "\t\tnumber: " + to_string(id) + "\n" +
            "\t\tinput_data:\n" +
                "\t\t\tbuffer_size: " + to_string(buffer) + "\n" +
            "\t\tresults:\n" +
                "\t\t\tduration: " + o.str() + "\n";

}

bool Cache::run() {
    unsigned int counter = 0;
    unsigned int cap = 4096;
    const unsigned int CacheSizeL1 = 262144;
    //const unsigned int CacheSizeL2 = 1048576;
    //const unsigned int CacheSizeL3 = 6291456;
    unsigned int i = 0;
    int* array;
    string res ="";

    do {
        counter++;
        //Прогрев
        array = new int[cap];
        for(i = 0; i < cap - 1; ++i)
            array[i] = i + 1;

        array[cap - 1] = 0;

        int t = 0;

        //Прямой
        auto tON = std::chrono::high_resolution_clock::now();
        for(i = 0; i < 1000; ++i) {
            for (unsigned int j = 0; j < cap; ++j)
                t = array[t];
        }
        auto tOF = std::chrono::high_resolution_clock::now();
        auto dF = std::chrono::duration_cast<std::chrono::milliseconds>(tOF - tON).count();
        Node node{counter, "Forward", dF, (cap * 4) / 1024};
        exps.push_back(node);
        addDataToString(res, node.id, node.name, node.time, node.buffer);
        delete[] array;

        //Прогрев
        array = new int[cap];
        for(i = 1; i < cap; ++i)
            array[i] = i - 1;

        array[0] = cap - 1;
        t = 0;

        //Обратный
        tON = std::chrono::high_resolution_clock::now();
        for (i = 0; i < 1000; ++i) {
            for(unsigned int j = 0; j < cap; ++j)
                t = array[t];
        }
        tOF = std::chrono::high_resolution_clock::now();
        dF = std::chrono::duration_cast<std::chrono::milliseconds>(tOF - tON).count();
        Node node1{counter, "Backward", dF, (cap * 4) / 1024};
        exps.push_back(node1);
        addDataToString(res, node1.id, node1.name, node1.time, node1.buffer);
        delete[] array;

        //Прогрев
        array = new int[cap];
        for(i = 0; i < cap - 1; ++i)
            array[i] = i + 1;

        array[cap - 1] = 0;
        t = 0;
        //Случайный

        for(i = 0; i < cap * 100; ++i){
            auto i1 = rand() % cap;
            auto i2 = rand() % cap;

            auto tmp = array[i1];
            array[i1] = array[i2];
            array[i2] = tmp;
        }

        tON = std::chrono::high_resolution_clock::now();
        for(i = 0; i < 1000; ++i) {
            for (unsigned int j = 0; j < cap; ++j)
                t = array[t];
        }
        tOF = std::chrono::high_resolution_clock::now();
        dF = std::chrono::duration_cast<std::chrono::milliseconds>(tOF - tON).count();
        Node node2{counter, "Random", dF, (cap * 4) / 1024};
        exps.push_back(node2);
        addDataToString(res, node2.id, node2.name, node2.time, node2.buffer);

        cap *= 2;
        if (cap == CacheSizeL1)
            cap = CacheSizeL1 * 3/2;
        delete[] array;
        i++;
    }while (cap < CacheSizeL1 * 3/2);

    std::ofstream out;          // поток для записи
    out.open("..\\result\\data.txt"); // окрываем файл для записи
    if (out.is_open())
    {
        out << res << endl;
    }
    return true;
}