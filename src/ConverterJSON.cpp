#include "ConverterJSON.h"

std::string ConverterJSON::ReadText(const std::string &path) {
    std::stringstream text;
    std::ifstream file(path);
    if(file.fail()){
        throw std::string{"File " + path + " does not exist"};
    }
    text << file.rdbuf();
    file.close();
    return text.str();
}

void ConverterJSON::GetConfig(std::shared_ptr<Base> &_base){
    std::ifstream file_config(CONFIG_PATH);
    if(file_config.fail()){
        file_config.close();
        throw std::string{"Config file does not exist"};
    }
    else if(file_config.eof()){
        file_config.close();
        throw std::string{"Config file is empty"};
    }
    else{
        file_config>>_base->config;
        file_config.close();
    }
}

void ConverterJSON::GetConfig(std::shared_ptr<Base> &_base, const std::string &path){
    std::ifstream file_config(path);
    if(file_config.fail()){
        file_config.close();
        throw std::string{"Config file does not exist"};
    }
    else if(file_config.eof()){
        file_config.close();
        throw std::string{"Config file is empty"};
    }
    else{
        file_config>>_base->config;
        file_config.close();
    }
}

void ConverterJSON::GetRequests(std::shared_ptr<Base> &_base){
    std::ifstream file_requests(REQUESTS_PATH);
    if(file_requests.fail()){
        file_requests.close();
        throw std::string{"Requests file does not exist"};
    }
    else if(file_requests.eof()){
        file_requests.close();
        throw std::string{"Requests file is empty"};
    }
    else{
        file_requests>>_base->requests;
        file_requests.close();
    }
}

std::shared_ptr<std::vector<std::string>> ConverterJSON::GetTextDocuments(std::shared_ptr<Base> &_base){
    if(_base->config.empty()){
        try{
            ConverterJSON::GetConfig(_base);
        }
        catch(const std::string& message){
            std::cerr <<message << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            Creator::CreateConfig();
            ConverterJSON::GetConfig(_base);
        }
    }

    std::vector<std::string> files_paths;
    auto it = _base->config.find("files");
    if(it==_base->config.end()){
        throw std::string{"'files' are empty in config file"};
    }
    else{
        files_paths = *it;
    }

    for(const auto& path:files_paths){
        try{
            auto text = ConverterJSON::ReadText(path);
            _base->docs.push_back(text);
        }
        catch(const std::string& message){
            std::cerr <<message << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    return std::make_shared<std::vector<std::string>>(_base->docs);
}

int ConverterJSON::GetResponsesLimit(std::shared_ptr<Base> &_base) {
    if(_base->config.empty()){
        try{
            GetConfig(_base);
        }
        catch(const std::string& message){
            std::cerr <<message << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            Creator::CreateConfig();
        }
    }
    auto it = _base->config.find("config");
    if(it==_base->config.end()){
        throw std::string{"Config parameters are empty in config file"};
    }
    json inner_config = it.value();
    auto it2 = inner_config.find("max_responses");
    if(it2==inner_config.end()){
        throw std::string{"Max_responses not found"};
    }
    return (int)it2.value();
}

void ConverterJSON::SetResponsesLimit(std::shared_ptr<Base> &_base, int val){
    _base->config["max_responses"] = val;
    std::ofstream file_config(CONFIG_PATH);
    file_config<<_base->config;
    file_config.close();
}

void ConverterJSON::PutAnswers(std::shared_ptr<Base> &_base, const std::shared_ptr<std::vector<std::vector<RelativeIndex>>> &input_answers){
    ordered_json answers;
    std::map<std::string,ordered_json> request_answer;
    for(int req = 0; req<input_answers->size(); req++){
        ordered_json result;
        std::stringstream s;
        s<<"request"<<std::setfill('0')<<std::setw(3)<<req+1;
        auto current_ans = input_answers->at(req);
        if(current_ans.empty()){
            result["result"] = "false";
        }
        else if(current_ans.size()==1){
            result["result"] = "true";
            result["docid"] = current_ans.begin()->doc_id;
            result["rank"] = 100*(current_ans.begin()->rank)/100;
        }
        else{
            result["result"] = "true";
            std::vector<std::pair<std::pair<std::string,int>,std::pair<std::string,float>>> relevance;
            int limit;
            try{
                limit = ConverterJSON::GetResponsesLimit(_base);
            }
            catch(const std::string& message){
                std::cerr <<message << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                Creator::CreateConfig();
            }
            int size = current_ans.size();
            int max = limit > size ? size : limit;
            for(int i=0; i<max;i++){
                auto p1 = std::make_pair("docid", current_ans.at(i).doc_id);
                float t = roundf(100*current_ans.at(i).rank)/100;
                auto p2 = std::make_pair("rank", t);
                auto p = std::make_pair(p1,p2);
                relevance.emplace_back(p);
            }
            result["relevance"] = relevance;
        }
        request_answer.insert(std::make_pair(s.str(), result));

    }
    answers["answers"] = request_answer;
    std::ofstream file_answers(ANSWERS_PATH);
    file_answers << std::setw(4) << answers << std::endl;
    file_answers.close();
    std::cout<<"Answers are in the file 'answers.json'"<<std::endl;
}



