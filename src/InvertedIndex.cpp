#include "InvertedIndex.h"

void InvertedIndex::UpdateDocumentBase(std::shared_ptr<Base> &_base){
    try{
        ConverterJSON::GetTextDocuments(_base);
#ifdef SINGLETHREAD
        IndexDocumentBase(_base);
#endif
#ifdef MULTITHREAD

#ifdef VECTORTHREAD
        /// Simple
        std::vector<IndexThread> many_dictionaries;
        std::vector<std::thread> threads;
        for(int id = 0; id<_base->docs.size(); id++){
            IndexThread one_dictionary(_base,id);
            many_dictionaries.push_back(one_dictionary);
        }

        for(int id = 0; id<_base->docs.size(); id++){
            threads.emplace_back([&,id]() {
                many_dictionaries[id].IndexDocument();
            });
        }
        for (auto &thread : threads) {
            if(thread.joinable()){
                thread.join();
            }
        }
        MergeThreads(_base, many_dictionaries);
#endif
#ifdef THREADPOOL
        /// Thread Pool
        std::vector<IndexThread> many_dictionaries;
        for(int id = 0; id<_base->docs.size(); id++){
            IndexThread one_dictionary(_base,id);
            many_dictionaries.push_back(one_dictionary);
        }

        int nThreads = _base->docs.size()>std::thread::hardware_concurrency()?std::thread::hardware_concurrency():_base->docs.size();
        ThreadPool threads(nThreads);
        for(int id = 0; id<many_dictionaries.size(); id++){
            threads.AddTask(std::bind(&IndexThread::IndexDocument,&many_dictionaries[id]));
        }

        threads.WaitAll();
        MergeThreads(_base, many_dictionaries);
#endif
#endif
    }
    catch(const std::string& message){
        std::cerr <<message << std::endl;
    }
}

void InvertedIndex::UpdateDocumentBase(std::shared_ptr<Base> &_base, const std::shared_ptr<std::vector<std::string>> &input_docs){
    for(auto it:*input_docs){
        _base->docs.push_back(it);
    }
#ifdef SINGLETHREAD
    IndexDocumentBase(_base);
#endif

#ifdef MULTITHREAD

#ifdef VECTORTHREAD
    /// Simple
    std::vector<IndexThread> many_dictionaries;
    std::vector<std::thread> threads;
    for(int id = 0; id<_base->docs.size(); id++){
        IndexThread one_dictionary(_base,id);
        many_dictionaries.push_back(one_dictionary);
    }
    for(int id = 0; id<_base->docs.size(); id++){
        threads.emplace_back([&,id]() {
            many_dictionaries[id].IndexDocument();
        });
    }
    for (auto &thread : threads) {
        if(thread.joinable()){
            thread.join();
        }
    }
    MergeThreads(_base, many_dictionaries);
#endif

#ifdef THREADPOOL
    /// Thread Pool
    std::vector<IndexThread> many_dictionaries;
    for(int id = 0; id<_base->docs.size(); id++){
        IndexThread one_dictionary(_base,id);
        many_dictionaries.push_back(one_dictionary);
    }

    int nThreads = _base->docs.size()>std::thread::hardware_concurrency()?std::thread::hardware_concurrency():_base->docs.size();
    ThreadPool threads(nThreads);
    for(int id = 0; id<many_dictionaries.size(); id++){
        threads.AddTask(std::bind(&IndexThread::IndexDocument,&many_dictionaries[id]));
    }

    threads.WaitAll();
    MergeThreads(_base, many_dictionaries);
#endif
#endif
}

#ifdef SINGLETHREAD
void InvertedIndex::IndexDocumentBase(std::shared_ptr<Base> &_base){
    for(int id = 0; id<_base->docs.size(); id++){
        auto words = ParseText(_base->docs.at(id));
        for(auto word:words){
            auto key = _base->freq_dictionary.find(word);
            if(key!=_base->freq_dictionary.end()){
                auto last_entry = key->second.rbegin();
                if(last_entry->doc_id==id){
                    last_entry->count++;
                }
                else{
                    Entry entry(id,1);
                    key->second.push_back(entry);
                }
            } else{
                Entry entry(id,1);
                std::vector<Entry> freq{entry};
                _base->freq_dictionary.insert(std::pair<std::string, std::vector<Entry>> (word, freq));
            }
        }
    }
}
#endif

#ifdef MULTITHREAD
void InvertedIndex::MergeThreads(std::shared_ptr<Base> &_base, const std::vector<IndexThread> &thread_dictionaries){
    for(auto thread_dictionary:thread_dictionaries){
        _base->freq_dictionary.insert(thread_dictionary.thread_freq_dictionary.begin(), thread_dictionary.thread_freq_dictionary.end());
    }
}
#endif

std::vector<Entry> InvertedIndex::GetWordCount(const std::shared_ptr<Base> &_base, const std::string &word){
    if(_base->freq_dictionary.empty()){
        throw std::string{"Frequency dictionary is empty! Run function UpdateDocumentBase"};
    }
    auto key = _base->freq_dictionary.find(word);
    if(key!=_base->freq_dictionary.end()){
        return key->second;
    }
    else{
        std::cout<<"Word '"<<word<<"' not found in documents"<<std::endl;
        return std::vector<Entry>();
    }
}

std::vector<std::string> InvertedIndex::ParseText(const std::string &str){
    int start, end = 0, i=0;
    std::vector<std::string> v;
    std::string del{" ,;:.\"!?'*\n"};
    while ((start = str.find_first_not_of(del, end)) != std::string::npos){
        end = str.find_first_of(del, start);
        v.push_back(str.substr(start, end - start));
        i++;
    }
    return v;
}

void IndexThread::IndexDocument() {
    auto words = InvertedIndex::ParseText(doc);
    std::set<std::string> unique_words(words.begin(),words.end());
    for(int id = 0; id<docs.size(); id++) {
        auto doc_words = InvertedIndex::ParseText(docs.at(id));
        for (auto doc_word: doc_words) {
            for (auto word: unique_words) {
                if (doc_word == word) {
                    auto key = thread_freq_dictionary.find(word);
                    if (key != thread_freq_dictionary.end()) {
                        auto last_entry = key->second.rbegin();
                        if (last_entry->doc_id == id) {
                            last_entry->count++;
                        } else {
                            Entry entry(id, 1);
                            key->second.push_back(entry);
                        }
                    } else {
                        Entry entry(id, 1);
                        std::vector<Entry> freq{entry};
                        thread_freq_dictionary.insert(std::pair<std::string, std::vector<Entry>>(word, freq));
                    }
                }
            }
        }
    }
}