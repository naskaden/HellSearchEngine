#include "SearchServer.h"

std::shared_ptr<std::vector<std::vector<RelativeIndex>>> SearchServer::Search(std::shared_ptr<Base> &_base){
    if(_base->requests.empty()){
        try{
            ConverterJSON::GetRequests(_base);
        }
        catch(const std::string& message){
            std::cerr <<message << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            Creator::CreateRequests();
            ConverterJSON::GetRequests(_base);
        }
    }
    std::vector<std::vector<RelativeIndex>> AllRelIndex;
    std::vector<std::string> requests;
    auto it = _base->requests.find("requests");
    if(it==_base->requests.end()){
        throw std::string{"'requests' are empty in requests file"};
    }
    else{
        requests = *it;
    }
    std::cout<<"Processing requests..."<<std::endl;
    for(const auto& req:requests){
        auto RelIndex = SearchInRequest(_base,req);
        AllRelIndex.push_back(*RelIndex);
    }
    std::cout<<"Search is completed"<<std::endl;
    return std::make_shared<std::vector<std::vector<RelativeIndex>>>(AllRelIndex);
}

std::shared_ptr<std::vector<std::vector<RelativeIndex>>> SearchServer::Search(std::shared_ptr<Base> &_base, const std::shared_ptr<std::vector<std::string>> &input_requests){
    if(input_requests== nullptr){
        std::cerr<<"Requests do not exist"<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        Creator::CreateRequests();
        ConverterJSON::GetRequests(_base);
        *input_requests = _base->requests;
    }
    std::cout<<"Processing requests..."<<std::endl;
    std::vector<std::vector<RelativeIndex>> AllRelIndex;
    for(const auto& req:*input_requests){
        auto RelIndex = SearchInRequest(_base,req);
        AllRelIndex.push_back(*RelIndex);
    }
    std::cout<<"Search is completed"<<std::endl;
    return std::make_shared<std::vector<std::vector<RelativeIndex>>>(AllRelIndex);
}

std::shared_ptr<std::vector<RelativeIndex>> SearchServer::SearchInRequest(std::shared_ptr<Base> &_base, const std::string &input_request){
    auto words = InvertedIndex::ParseText(input_request);
    std::set<std::string> unique_words(words.begin(),words.end());
    std::vector<RelativeIndex> RelIndex;
    std::map<int,float> AbsIndex;
    for(const auto& word:unique_words){
        auto word_count = InvertedIndex::GetWordCount(_base,word);
        if(!word_count.empty()){
            for(auto entry:word_count){
                auto key_doc = AbsIndex.find(entry.doc_id);
                if(key_doc!=AbsIndex.end()){
                    key_doc->second+=(float)entry.count;
                }
                else{
                    AbsIndex.insert(std::pair<int, float> (entry.doc_id, (float)entry.count));
                }
            }
        }
    }
    if(!AbsIndex.empty()){
        auto maxAbsIndex = std::max_element(AbsIndex.begin(), AbsIndex.end(), [](const auto &x, const auto &y){
            return x.second < y.second;})->second;
//        for(auto doc = AbsIndex.begin(); doc != AbsIndex.end(); doc++){
//            doc->second/=maxAbsIndex;
//        }
        for(auto& doc:AbsIndex){
            doc.second/=maxAbsIndex;
        }
        std::multimap<float,int> SwapRelIndex;
        for(auto doc:AbsIndex){
            SwapRelIndex.insert(std::pair<float, int> (doc.second, doc.first));
        }
        int i =0;
        auto limit = ConverterJSON::GetResponsesLimit(_base);
        while(!SwapRelIndex.empty()){
            auto maxSwapRelIndex = std::max_element(SwapRelIndex.begin(), SwapRelIndex.end(),[](const auto &x, const auto &y){
                return x.first < y.first;});

            for(auto doc = maxSwapRelIndex; doc != SwapRelIndex.end(); doc++){
                RelativeIndex relativeIndex(doc->second, doc->first);
                RelIndex.push_back(relativeIndex);
                i++;
                if(i>=limit){
                    break;
                }
            }
            SwapRelIndex.erase(maxSwapRelIndex,SwapRelIndex.end());
        }
    }
    return std::make_shared<std::vector<RelativeIndex>>(RelIndex);
}

