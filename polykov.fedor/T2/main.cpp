#include "data_struct.h"
#include <vector>
#include <algorithm>
#include <iterator>

int main()
{
    std::vector<DataStruct> data;
    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), DataStructComparator());

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}

/*asdf
(:key1 50.0d:key2 0xFFFA:key3 "Data":)
(:key2 0xA:key1 10.0d:key3 "Hello":)
(:key3 "World":key1 10.0d:key2 0xB:)
(:key1 10.0d:key2 0xA:key3 "Short":)
(:key1 10.0d:key2 0xA:key3 "Longer":)*/