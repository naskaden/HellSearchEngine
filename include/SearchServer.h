#ifndef HELLSEARCHENGINE_SEARCHSERVER_H
#define HELLSEARCHENGINE_SEARCHSERVER_H

#pragma once
#include "Base.h"
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include <set>

class SearchServer{
public:
    SearchServer()=default;

    static std::shared_ptr<std::vector<std::vector<RelativeIndex>>> Search(std::shared_ptr<Base> &_base);
    static std::shared_ptr<std::vector<std::vector<RelativeIndex>>> Search(std::shared_ptr<Base> &_base, const std::shared_ptr<std::vector<std::string>> &input_requests);

private:
    static std::shared_ptr<std::vector<RelativeIndex>> SearchInRequest(std::shared_ptr<Base> &_base, const std::string &input_request);
};

#endif //HELLSEARCHENGINE_SEARCHSERVER_H
