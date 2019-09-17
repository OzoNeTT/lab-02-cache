#pragma  once

#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
using namespace std;

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


class Experiments{
private:
    const int loops = 1000;
    unsigned int *buffer;
    int *buf_sizes;
    int size;
    struct Node {
        unsigned int id;
        string name;
        int64_t time ;
    };
    vector<Node> exps;
public:
    Experiments(int, int);
    void GraphBuild(vector<Node>);
    int KBtoSize(int kb);
    void InitBuffer(int);
    void Burn(int);
    void Forward(int);
    void Backward(int);
    void Random(int);
    bool start();
};

Experiments::Experiments(int min, int max) {
    size = log2(max / min) + 3;
    buf_sizes = new int[size];
    int n = 0;
    min /= 2;
    while (min <= max * 3 / 2)
    {
        buf_sizes[n++] = min;
        min *= 2;
    }
    buf_sizes[n++] = max * 3 / 2;
    buffer = nullptr;
}

int Experiments::KBtoSize(int kb) {
    return kb * 1024 / sizeof(int);
}

int dump(int something){
    return something;
}

void Experiments::InitBuffer(int count) {
    if (buffer != nullptr)
        delete[] buffer;
    buffer = new unsigned int[count];
    for (unsigned int i = 0; i < (unsigned int)count; i++)
        buffer[i] = rand() % count;
}


void Experiments::Burn(int value) {
    int something = 0;
    for (int i = 0; i < value; i++)
        something = buffer[rand() % value];
    dump(something);
}

void Experiments::Forward(int value) {
    int something = 0;
    for (int i = 0; i < loops; i++)
    {
        for (unsigned int j = 0; j < (unsigned int)value; j++)
            something = buffer[j];
    }
    dump(something);
}

void Experiments::Backward(int value) {
    int something = 0;
    for (int i = 0; i < loops; i++)
    {
        for (int j = value - 1; j >= 0; j--)
            something = buffer[j];
    }
    dump(something);
}

void Experiments::Random(int value) {
    int something = 0;
    for (int i = 0; i < loops; i++)
    {
        for (unsigned int j = 0; j < (unsigned int)value; j++)
            something = buffer[rand() % value];
    }
    dump(something);
}

bool Experiments::start() {
    string res="";
    for (int i = 0; i < size; i++) {
        int value = KBtoSize(buf_sizes[i]);

        InitBuffer(value);
        Burn(value);

        //прямой
        auto tON = std::chrono::high_resolution_clock::now();
        Forward(value);
        auto tOF = std::chrono::high_resolution_clock::now();
        auto dF = std::chrono::duration_cast<std::chrono::milliseconds>(tOF - tON).count();

        Node node{(unsigned int)i+1, "Forward", dF};
        exps.push_back(node);
        addDataToString(res, node.id, node.name, node.time, buf_sizes[i]);

        //обратный
        tON = std::chrono::high_resolution_clock::now();
        Backward(value);
        tOF = std::chrono::high_resolution_clock::now();
        dF = std::chrono::duration_cast<std::chrono::milliseconds>(tOF - tON).count();

        Node node1{(unsigned int)i+1, "Backward", dF};
        exps.push_back(node1);
        addDataToString(res, node1.id, node1.name, node1.time, buf_sizes[i]);

        //случайный
        tON = std::chrono::high_resolution_clock::now();
        Random(value);
        tOF = std::chrono::high_resolution_clock::now();
        dF = std::chrono::duration_cast<std::chrono::milliseconds>(tOF - tON).count();

        Node node2{(unsigned int)i+1, "Random", dF};
        exps.push_back(node2);
        addDataToString(res, node2.id, node2.name, node2.time, buf_sizes[i]);
    }
    std::ofstream out;          // поток для записи
    out.open("..\\result\\data.txt"); // окрываем файл для записи
    if (out.is_open())
    {
        out << res << endl;
    }
    GraphBuild(exps);
    return true;
}

void Experiments::GraphBuild(vector<Node> vect) {
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

    vector<int64_t> forwardTime;
    vector<int64_t> backwardTime;
    vector<int64_t> randomTime;
    for( unsigned int i = 0; i < vect.size(); i++) {
        Node node = vect[i];

        if(node.name == "Forward"){
            forwardTime.push_back(node.time);
        }
        else if(node.name == "Backward") {
            backwardTime.push_back(node.time);
        }
        else if(node.name == "Random") {
            randomTime.push_back(node.time);
        }
    }
    result += "\ndata.addRows([\n";
    for (int i = 0; i < size; i++) {
        result += "[" + std::to_string(buf_sizes[i]) + ", ";
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


