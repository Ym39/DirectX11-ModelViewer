#pragma once
#include<vector>
#include<string>

namespace Getter
{
    bool VectorGetter(void* list, int count, const char** outText)
    {
        std::vector<std::string>& vector = *static_cast<std::vector<std::string>*>(list);
        if (count < 0 || count > vector.size())
        {
            return false;
        }
        *outText = vector[count].c_str();

        return true;
    }
}