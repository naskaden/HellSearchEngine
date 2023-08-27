#ifndef HELLSEARCHENGINE_CREATOR_H
#define HELLSEARCHENGINE_CREATOR_H

#include "Base.h"
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class Creator{
public:
    Creator() = default;
    static void CreateConfig();
    static void CreateRequests();
};

#endif //HELLSEARCHENGINE_CREATOR_H
