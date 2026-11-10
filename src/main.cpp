#include "utils.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <print>
#include <string>

using namespace std;
namespace fs = std::filesystem;
fs::path data_path{std::filesystem::current_path() / "data" /
                   "measurements.txt"};
ifstream data_stream{data_path};
string sbuf{};
constexpr size_t SBUF_RESERVE_AMOUNT = 64;

struct map_contents
{
    float max;
    float min;
    float mean;
    size_t count;
};

// map<string, float> map_max;
// map<string, float> map_min;
// map<string, pair<float, size_t>> map_mean;
map<string, struct map_contents> mapv;

// parse the first two lines as they are comments
void prepare_file()
{
    getline(data_stream, sbuf);
    getline(data_stream, sbuf);
}

void print_result()
{
    // auto it_max = map_max.begin();
    // auto it_min = map_min.begin();
    // auto it_mean = map_mean.begin();
    auto it = mapv.begin();
    print("{{");
    for (size_t i{0}; i < mapv.size(); i++)
    {
        print("{}={:.1f}/{:.1f}/{:.1f}",
              it->first,
              it->second.min,
              it->second.mean,
              it->second.max);
        it++;
        if (i < mapv.size() - 1)
        {
            print(", ");
        }
    }
    println("}}");
}

void calculate_means()
{
    for_each(mapv.begin(),
             mapv.end(),
             [](auto &obj)
             {
                 // println("calculating mean {}: {}/{} = {}",
                 //         obj.first,
                 //         obj.second.first,
                 //         obj.second.second,
                 //         obj.second.first / obj.second.second);
                 auto temp = obj.second.mean / obj.second.count;
                 obj.second.mean = (ceil(temp * 10.0 - 0.5) / 10.0) + 0.0f;
                 // obj.second.first = (ceil(temp * 10.0 - 0.5) / 10.0) + 0.0f;
             });
}
void parse_file()
{
    // load each line:
    // process character by character until we reach '\n'
    // data_stream.read(&sbuf[0], 64);
    getline(data_stream, sbuf);
    while (!data_stream.eof())
    {
        obrc::log("sbuf: {}", sbuf);
        const size_t separator_index{sbuf.find(';')};
        const string city{sbuf.substr(0, separator_index)};
        const string temperature_string{sbuf.substr(separator_index + 1)};
        const float temperature_float{stof(temperature_string)};

        float inserted{temperature_float};
        if (mapv.contains(city))
        {
            mapv[city].max = max(inserted, mapv[city].max);
            mapv[city].min = min(inserted, mapv[city].min);
            mapv[city].mean += inserted;
            mapv[city].count++;
        }
        else
        {
            mapv[city] = map_contents{inserted, inserted, inserted, 1};
        }
        getline(data_stream, sbuf);
    }

    calculate_means();
}

int main(int argc, char **argv)
{
    // config
    if (argc == 2)
    {
        data_path = fs::path{argv[1]};
        data_stream = ifstream{data_path};
    }

    if (!data_stream.good())
    {
        cout << "failed to find " << data_path.string() << "\n";
        return 1;
    }

    sbuf.reserve(SBUF_RESERVE_AMOUNT);

    prepare_file();
    parse_file();
    print_result();
}
