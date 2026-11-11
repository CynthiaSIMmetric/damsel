#include "utils.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
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
    int64_t max;
    int64_t min;
    int64_t mean;
    size_t count;
};

// map<string, float> map_max;
// map<string, float> map_min;
// map<string, pair<float, size_t>> map_mean;
map<string, struct map_contents> mapv;

constexpr int64_t my_div(const int64_t num, const int64_t den)
{

    auto ret = num / den;
    if ((num % den != 0) && (num < 0))
        --ret;
    return ret;
}

string fmt_int_float_to_string(int64_t val)
{
    string ret;
    if (val < 0)
    {
        ret = "-";
        val = -val;
    }
    return ret + to_string(val / 10) + "." + to_string(val % 10);
}
void print_result()
{
    // auto it_max = map_max.begin();
    // auto it_min = map_min.begin();
    // auto it_mean = map_mean.begin();
    auto it{mapv.begin()};
    print("{{");
    for (size_t i{0}; i < mapv.size(); i++)
    {
        const int64_t smean = it->second.mean;
        const int64_t scount = it->second.count;

        int64_t my_mean{my_div(smean * 2 + scount, 2 * scount)};
        print("{}={}/{}/{}",
              it->first,
              fmt_int_float_to_string(it->second.min),
              fmt_int_float_to_string(my_mean),
              fmt_int_float_to_string(it->second.max));
        it++;
        if (i < mapv.size() - 1)
        {
            print(", ");
        }
    }
    println("}}");
}

// void calculate_means()
// {
//     for_each(mapv.begin(),
//              mapv.end(),
//              [](auto &obj)
//              {
//                  obrc::log("mean: {} count: {} res: {}",
//                            obj.second.mean,
//                            obj.second.count,
//                            round(obj.second.mean / (double)
//                            obj.second.count));

//                  obj.second.mean = round(obj.second.mean / (double)
//                  obj.second.count);
//              });
// }
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
        const string temp_string{sbuf.substr(separator_index + 1)};
        const float temp_float{stof(temp_string)};
        const int_fast32_t temp_int{static_cast<int_fast32_t>(temp_float * 10)};
        obrc::log(
            "tfloat: {} tint: {} tstr: {}", temp_float, temp_int, temp_string);

        int_fast32_t inserted{temp_int};
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

    // calculate_means();
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

    parse_file();
    print_result();
}
