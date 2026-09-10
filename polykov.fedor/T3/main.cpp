#include "polygon_commands.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <cctype>

using namespace std::placeholders;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error: cannot open file " << argv[1] << '\n';
        return 1;
    }

    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        Polygon temp;
        if (iss >> temp)
        {
            iss >> std::ws;
            if (iss.eof())
            {
                polygons.push_back(std::move(temp));
            }
        }
    }

    while (std::getline(std::cin, line))
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string command;
        if (!(iss >> command))
            continue;

        try
        {
            if (command == "AREA")
            {
                std::string arg;
                if (!(iss >> arg))
                    throw std::invalid_argument("");

                std::string extra;
                if (iss >> extra)
                    throw std::invalid_argument("");

                std::cout << std::fixed << std::setprecision(1);

                if (arg == "EVEN")
                {
                    double sum = std::accumulate(polygons.begin(), polygons.end(),
                        0.0, SumAreaIfEven());
                    std::cout << sum << '\n';
                }
                else if (arg == "ODD")
                {
                    double sum = std::accumulate(polygons.begin(), polygons.end(),
                        0.0, SumAreaIfOdd());
                    std::cout << sum << '\n';
                }
                else if (arg == "MEAN")
                {
                    if (polygons.empty())
                        throw std::invalid_argument("");
                    double sum = std::accumulate(polygons.begin(), polygons.end(),
                        0.0, SumArea());
                    std::cout << sum / polygons.size() << '\n';
                }
                else
                {
                    if (arg.empty() ||
                        !std::all_of(arg.begin(), arg.end(),
                            [](unsigned char c) { return std::isdigit(c); }))
                    {
                        throw std::invalid_argument("");
                    }
                    std::size_t pos = 0;
                    std::size_t n = std::stoull(arg, &pos);
                    if (pos != arg.size())
                        throw std::invalid_argument("");
                    double sum = std::accumulate(polygons.begin(), polygons.end(),
                        0.0, SumAreaByVertexes(n));
                    std::cout << sum << '\n';
                }
            }
            else if (command == "MAX")
            {
                if (polygons.empty())
                    throw std::invalid_argument("");

                std::string arg;
                if (!(iss >> arg))
                    throw std::invalid_argument("");

                std::string extra;
                if (iss >> extra)
                    throw std::invalid_argument("");

                if (arg == "AREA")
                {
                    auto it = std::max_element(polygons.begin(), polygons.end(),
                        CompareByArea());
                    std::cout << std::fixed << std::setprecision(1)
                        << getArea(*it) << '\n';
                }
                else if (arg == "VERTEXES")
                {
                    auto comp = std::bind(std::less<size_t>(),
                        std::bind(&std::vector<Point>::size,
                            std::bind(&Polygon::points, _1)),
                        std::bind(&std::vector<Point>::size,
                            std::bind(&Polygon::points, _2)));
                    auto it = std::max_element(polygons.begin(), polygons.end(), comp);
                    std::cout << std::defaultfloat;
                    std::cout << it->points.size() << '\n';
                }
                else
                {
                    throw std::invalid_argument("");
                }
            }
            else if (command == "MIN")
            {
                if (polygons.empty())
                    throw std::invalid_argument("");

                std::string arg;
                if (!(iss >> arg))
                    throw std::invalid_argument("");

                std::string extra;
                if (iss >> extra)
                    throw std::invalid_argument("");

                if (arg == "AREA")
                {
                    auto it = std::min_element(polygons.begin(), polygons.end(),
                        CompareByArea());
                    std::cout << std::fixed << std::setprecision(1)
                        << getArea(*it) << '\n';
                }
                else if (arg == "VERTEXES")
                {
                    auto comp = std::bind(std::less<size_t>(),
                        std::bind(&std::vector<Point>::size,
                            std::bind(&Polygon::points, _1)),
                        std::bind(&std::vector<Point>::size,
                            std::bind(&Polygon::points, _2)));
                    auto it = std::min_element(polygons.begin(), polygons.end(), comp);
                    std::cout << std::defaultfloat;
                    std::cout << it->points.size() << '\n';
                }
                else
                {
                    throw std::invalid_argument("");
                }
            }
            else if (command == "COUNT")
            {
                std::string arg;
                if (!(iss >> arg))
                    throw std::invalid_argument("");

                std::string extra;
                if (iss >> extra)
                    throw std::invalid_argument("");

                if (arg == "EVEN" || arg == "ODD")
                {
                    int count = std::count_if(polygons.begin(), polygons.end(),
                        CountIfEvenOrOdd(arg));
                    std::cout << std::defaultfloat;
                    std::cout << count << '\n';
                }
                else
                {
                    if (arg.empty() ||
                        !std::all_of(arg.begin(), arg.end(),
                            [](unsigned char c) { return std::isdigit(c); }))
                    {
                        throw std::invalid_argument("");
                    }
                    std::size_t pos = 0;
                    std::size_t n = std::stoull(arg, &pos);
                    if (pos != arg.size())
                        throw std::invalid_argument("");
                    auto pred = std::bind(std::equal_to<size_t>(),
                        std::bind(&std::vector<Point>::size,
                            std::bind(&Polygon::points, _1)),
                        n);
                    int count = std::count_if(polygons.begin(), polygons.end(), pred);
                    std::cout << std::defaultfloat;
                    std::cout << count << '\n';
                }
            }
            else if (command == "PERMS")
            {
                Polygon sample;
                if (!(iss >> sample))
                    throw std::invalid_argument("");

                std::string extra;
                if (iss >> extra)
                    throw std::invalid_argument("");

                int count = std::count_if(polygons.begin(), polygons.end(),
                    IsPermutationOf(sample));
                std::cout << std::defaultfloat;
                std::cout << count << '\n';
            }
            else if (command == "RIGHTSHAPES")
            {
                std::string extra;
                if (iss >> extra)
                    throw std::invalid_argument("");

                int count = std::count_if(polygons.begin(), polygons.end(),
                    HasRightAngle());
                std::cout << std::defaultfloat;
                std::cout << count << '\n';
            }
            else
            {
                throw std::invalid_argument("");
            }
        }
        catch (...)
        {
            std::cout << "<INVALID COMMAND>\n";
        }
    }

    return 0;
}
