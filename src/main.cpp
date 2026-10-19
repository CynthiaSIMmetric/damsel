#include <filesystem>
#include <fstream>

std::filesystem::path data_path{std::filesystem::current_path() / "data" /
                                "weather_stations.csv"};
int main()
{
    // load each line:
    // process character by character until we reach '\n'
    // process each line:
    // each string is a hash for the weather station
    // ';' is a delimiter
    // we keep 3 data elements for each weather station min, max and total
    // we do the mean calculation at the end
    // display the output

    ifstream file{};
}

void verify()
{
}
