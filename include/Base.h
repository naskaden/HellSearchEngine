#ifndef HELLSEARCHENGINE_BASE_H
#define HELLSEARCHENGINE_BASE_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;
using ordered_json = nlohmann::basic_json<nlohmann::ordered_map>;

#define CONFIG_PATH "../config.json"
#define CONFIG_PATH_FOR_TEST "../../config.json"
#define RESOURCES_PATH std::string{"../resources"}
#define REQUESTS_PATH std::string{"../requests.json"}
#define ANSWERS_PATH std::string{"../answers.json"}

struct Entry {
    size_t doc_id, count;
    Entry(size_t _doc_id, size_t _count):doc_id(_doc_id),count(_count){};
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id &&
                count == other.count);
    }
};

struct RelativeIndex{
    size_t doc_id;
    float rank;
    RelativeIndex(size_t _doc_id, float _rank):doc_id(_doc_id),rank(_rank){};
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};

class Base{
public:
    Base()=default;
    json config;
    json requests;

    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;

};

#endif //HELLSEARCHENGINE_BASE_H
