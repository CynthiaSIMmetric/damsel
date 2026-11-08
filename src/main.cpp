#include <algorithm>
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

struct CollateLess
{
    locale loc;
    explicit CollateLess(const locale &l = locale("en_US.UTF-8")) : loc(l)
    {
    }

    bool operator()(const string &a, const string &b) const
    {
        const auto &coll = use_facet<collate<char>>(loc);
        return coll.compare(a.data(),
                            a.data() + a.size(),
                            b.data(),
                            b.data() + b.size()) < 0;
    }
};

map<string, float, CollateLess> map_max;
map<string, float, CollateLess> map_min;
map<string, pair<float, size_t>, CollateLess> map_mean;

// parse the first two lines as they are comments
void prepare_file()
{
    getline(data_stream, sbuf);
    getline(data_stream, sbuf);
}

void print_result()
{
    auto it_max = map_max.begin();
    auto it_min = map_min.begin();
    auto it_mean = map_mean.begin();
    print("{{");
    for (size_t i{0}; i < map_max.size(); i++)
    {
        print("{}={}/{}/{}, ",
              it_max->first,
              it_min->second,
              it_mean->second.first,
              it_max->second);
        it_max++;
        it_min++;
        it_mean++;
    }
    print("}}");
}
void parse_file()
{
    // load each line:
    // process character by character until we reach '\n'
    // data_stream.read(&sbuf[0], 64);
    getline(data_stream, sbuf);
    while (!data_stream.eof())
    {
        // println("sbuf: {}", sbuf);
        const size_t separator_index{sbuf.find(';')};
        const string city{sbuf.substr(0, separator_index)};
        const string temperature_string{sbuf.substr(separator_index + 1)};
        // println("sep: {}\ncity: {}\ntemp: {}",
        //         separator_index,
        //         city,
        //         temperature_string);
        const float temperature_float{stof(temperature_string)};

        float inserted{temperature_float};
        if (map_max.contains(temperature_string))
        {
            inserted = max(inserted, map_max.at(temperature_string));
        }
        map_max.insert_or_assign(city, inserted);

        inserted = temperature_float;
        if (map_min.contains(temperature_string))
        {
            inserted = min(inserted, map_min.at(temperature_string));
        }
        map_min.insert_or_assign(city, inserted);

        auto count{1};
        if (map_mean.contains(temperature_string))
        {
            inserted += map_mean.at(temperature_string).first;
            count += map_mean.at(temperature_string).second;
        }
        map_mean.insert_or_assign(city, pair<float, size_t>{inserted, count});
        getline(data_stream, sbuf);
    }
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
    cout << "loaded file successfully: " << data_path.string() << endl;

    sbuf.reserve(SBUF_RESERVE_AMOUNT);
    cout << "reserved " << SBUF_RESERVE_AMOUNT << " bytes for sbuf\n";

    prepare_file();
    parse_file();
    print_result();
}
