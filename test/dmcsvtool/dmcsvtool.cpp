
#include "dmos.h"
#include "dmutil.h"

#include "dmcsv_parser.hpp"
#include "dmcsv_writer.hpp"

#include <vector>
#include <queue>
#include <filesystem>

int main( int argc, char** argv )
{
    std::string strFile = DMGetRootPath() + PATH_DELIMITER_STR + "slist.csv";


    if (!DMIsFile(strFile))
    {
        std::cout << "file: " << strFile << " not exist" << std::endl;
        return -1;
    }

    std::queue<std::vector<std::string>> q;

    csv::CSVReader reader(strFile, csv::DEFAULT_CSV);
    csv::CSVRow rows;
    for (size_t i = 0; reader.read_row(rows); i++) {
        std::cout <<  rows["age"].get<uint64_t>() << " , " << rows["name"].get<std::string>() << std::endl;

        q.push({ std::to_string(rows["age"].get<uint64_t>()) + "|" + rows["name"].get<std::string>() });
    }
 
    std::ofstream out(strFile);
    auto writer = csv::make_csv_writer(out);

    for (; !q.empty(); q.pop())
        writer.write_row(q.front());

    out.close();

    return 0;
}