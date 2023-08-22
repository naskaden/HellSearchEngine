#include "InvertedIndex.h"
#include "gtest/gtest.h"

void TestInvertedIndexFunctionality(const std::shared_ptr<std::vector<std::string>> &docs,
                                    const std::vector<std::string>& requests,
                                    const std::vector<std::vector<Entry>>& expected){
    auto base = std::make_shared<Base>();
    InvertedIndex::UpdateDocumentBase(base,docs);
    std::vector<std::vector<Entry>> result;

    for(auto& request : requests) {
        auto word_count = InvertedIndex::GetWordCount(base,request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    auto docs_ptr = std::make_shared<std::vector<std::string>>(std::move(docs));
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
            {
                    {0, 1}
            }, {
                    {0, 1}, {1, 3}
            }
    };
    TestInvertedIndexFunctionality(docs_ptr, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    auto docs_ptr = std::make_shared<std::vector<std::string>>(std::move(docs));
    const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
    const std::vector<std::vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            }, {
                    {0, 3}, {1, 2}, {2, 5}
            }, {
                    {3, 1}
            }
    };
    TestInvertedIndexFunctionality(docs_ptr, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const std::vector<std::string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    auto docs_ptr = std::make_shared<std::vector<std::string>>(std::move(docs));
    const std::vector<std::string> requests = {"m", "statement"};
    const std::vector<std::vector<Entry>> expected = {
            {
            }, {
                    {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs_ptr, requests, expected);
}
