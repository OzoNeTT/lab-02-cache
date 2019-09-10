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

void GraphBuild(vector<Node> vect) {
    std::string result = R"(<html>
        <head>
          <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
            <script type="text/javascript">
              google.charts.load('current', {'packages':['corechart']});
              google.charts.setOnLoadCallback(drawChart);
            function drawChart() {
              var data = new google.visualization.DataTable();
              data.addColumn('number', 'X');
         data.addColumn('number', 'forward');
        data.addColumn('number', 'backward');
         data.addColumn('number', 'random');
        )";

    vector<unsigned int> bufferSizes;
    vector<int64_t> forwardTime;
    vector<int64_t> backwardTime;
    vector<int64_t> randomTime;
    int counter = 0;

    for( unsigned int i = 0; i < vect.size(); i++) {
        Node node = vect[i];
        if(counter  == 3) {
            bufferSizes.push_back(node.buffer);
            counter = 0;
        }

        if(node.name == "Forward"){
            forwardTime.push_back(node.time);
        }
        else if(node.name == "Backward") {
            backwardTime.push_back(node.time);
        }
        else if(node.name == "Random") {
            randomTime.push_back(node.time);
        }
        counter++;
    }

    result += "\ndata.addRows([\n";
    for (size_t i = 0; i < bufferSizes.size(); i++) {
        result += "[" + std::to_string(bufferSizes[i]) + ", ";
        result += std::to_string(forwardTime[i]) + ", ";
        result += std::to_string(backwardTime[i]) + ", ";
        result += std::to_string(randomTime[i]) + ", ";

        result += "],\n";
    }
    result += "]);\n";

    result += R"(
              var options = {
                chart: {
                  title: 'CPU cache',
                },
                hAxis: {
                  title: 'Buffer size, KiB',
                  logScale: false
                },
                vAxis: {
                  title: 'Time, milliseconds',
                  logScale: false
                },
                explorer: {
                  axis: 'horizontal',
                  keepInBounds: true,
                  maxZoomIn: 8.0,
                },
                width: 1200,
                height: 800,
              };
              var chart = new google.visualization.LineChart(document.getElementById('line_top_x'));
              chart.draw(data, options);
            }
          </script>
        </head>
        <body>
          <div id="line_top_x"></div>
        </body>
        </html>
        )";

    std::ofstream out;          // поток для записи
    out.open("..\\result\\graf.html"); // окрываем файл для записи
    if (out.is_open())
    {
        out << result << endl;
    }
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
    GraphBuild(exps);
    return true;
}