#ifndef HELLSEARCHENGINE_INVERTEDINDEX_H
#define HELLSEARCHENGINE_INVERTEDINDEX_H

#include "Base.h"
#include <iostream>
#include <fstream>
#include <set>
#include "ConverterJSON.h"

//#define SINGLETHREAD
#define MULTITHREAD

#ifdef MULTITHREAD
//#define VECTORTHREAD
#define THREADPOOL

#include <thread>
#include <chrono>
#include <mutex>
#include "ThreadPool.h"

class IndexThread{
public:
    IndexThread(std::shared_ptr<Base> &_base, const int _id):id(_id){
        docs = _base->docs;
        doc = docs.at(id);
    };
    int id;
    std::string doc;
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> thread_freq_dictionary;
    void IndexDocument();
};
#endif


class InvertedIndex {
public:
    InvertedIndex() = default;

    static void UpdateDocumentBase(std::shared_ptr<Base> &_base);
    static void UpdateDocumentBase(std::shared_ptr<Base> &_base, const std::shared_ptr<std::vector<std::string>> &input_docs);
    static std::vector<Entry> GetWordCount(const std::shared_ptr<Base> &_base, const std::string &word);
    static std::vector<std::string> ParseText(const std::string &str);

private:
#ifdef SINGLETHREAD
    static void IndexDocumentBase(std::shared_ptr<Base> &_base);
#endif
#ifdef MULTITHREAD
static void MergeThreads(std::shared_ptr<Base> &_base,const std::vector<IndexThread> &thread_dictionaries);
#endif
};

#endif //HELLSEARCHENGINE_INVERTEDINDEX_H
