#include "Creator.h"

void Creator::CreateConfig(){
    json config, inner_config;
    std::vector<std::string> files;

    /// directly
    /*inner_config["name"] = "HellSearchEngine";
    inner_config["version"] = 0.1;
    inner_config["max_responses"] = 5;

    bool dirExists = fs::is_directory(RESOURCES_PATH);
    if(dirExists==false){
        throw std::string{"Resource directory does not exist"};
    }
    else{
        for(auto it: fs::recursive_directory_iterator(RESOURCES_PATH)){
            if(is_regular_file(it.path())){
                files.push_back(it.path().string());
            }
        }
    }

    config["config"] = inner_config;
    config["files"] = files;*/

    /// using cmd
    std::string str; double ver; unsigned int resp;
    std::cout<<"Creating config file."<<std::endl;
    std::cout<<"Enter name of project:"<<std::endl;
    std::getline(std::cin, str);
    inner_config["name"] = "HellSearchEngine";
    while(true) {
        std::cout << "Enter version:" << std::endl;
        std::cin >> ver;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cerr << "Incorrect version! Try again" << std::endl;
        } else {
            break;
        }
    }
    inner_config["version"] = ver;
    while(true) {
        std::cout << "Enter maximum responses:" << std::endl;
        std::cin >> resp;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cerr << "Incorrect maximum responses! Try again" << std::endl;
        } else {
            break;
        }
    }
    inner_config["max_responses"] = resp;
    while(true) {
        std::cout << "Enter path to directory of data base:" << std::endl;
        std::getline(std::cin, str);
        bool dirExists = fs::is_directory(str);
        if (dirExists == false) {
            std::cerr << "Directory does not exist" << std::endl;
        } else {
            break;
        }
    }
    for(auto it: fs::recursive_directory_iterator(str)){
        if(is_regular_file(it.path())){
            files.push_back(it.path().string());
        }
    }
    config["config"] = inner_config;
    config["files"] = files;

    std::ofstream file_config(CONFIG_PATH);
    file_config << std::setw(4) << config << std::endl;
    //file_config<<config;
    file_config.close();
}

void Creator::CreateRequests() {
    json requests;
    std::vector<std::string> words;
    std::string str;
    std::cout<<"Creating requests file."<<std::endl;
    std::cout<<"You can enter several requests."<<std::endl;
    std::cout<<"Each request consists of one or more words separated by spaces. Press 'Enter' at the end of each request."<<std::endl;
    std::cout<<"Enter 'stop' to finish creating requests file."<<std::endl;
    while(str!="stop"){
        std::cout<<"Enter request:"<<std::endl;
        std::getline(std::cin, str);
        if(str=="stop"){
            break;
        }
        words.push_back(str);
    }
    requests["requests"] = words;
    std::ofstream file_requests(REQUESTS_PATH);
    file_requests << std::setw(4) << requests << std::endl;
    //file_requests<<requests;
    file_requests.close();
}




