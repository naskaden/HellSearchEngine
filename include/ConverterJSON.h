#ifndef HELLSEARCHENGINE_CONVERTERJSON_H
#define HELLSEARCHENGINE_CONVERTERJSON_H

#include "Base.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <filesystem>

namespace fs = std::filesystem;


class ConverterJSON{
public:
    static void GetConfig(std::shared_ptr<Base> &_base);
    static void GetConfig(std::shared_ptr<Base> &_base, const std::string &path);
    static std::string ReadText(const std::string &path);
    static std::shared_ptr<std::vector<std::string>> GetTextDocuments(std::shared_ptr<Base> &_base);
    static void GetRequests(std::shared_ptr<Base> &_base);
    static int GetResponsesLimit(std::shared_ptr<Base> &_base);
    static void SetResponsesLimit(std::shared_ptr<Base> &_base, int val);
    static void PutAnswers(std::shared_ptr<Base> &_base, const std::shared_ptr<std::vector<std::vector<RelativeIndex>>> &input_answers);

};

#endif //HELLSEARCHENGINE_CONVERTERJSON_H
