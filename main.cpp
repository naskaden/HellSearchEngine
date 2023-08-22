#include <iostream>
#include "Base.h"
#include <fstream>
#include "Creator.h"
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

//#define CREATE_CONFIG_FILE
//#define CREATE_REQUESTS_FILE
#define SEARCH
//#define DIRECT_INPUT

int main() {
#ifdef CREATE_CONFIG_FILE
    try{
        Creator::CreateConfig();
    }
    catch(const std::string& message)
    {
        std::cerr <<message << std::endl;
    }
#endif

#ifdef CREATE_REQUESTS_FILE
    Creator::CreateRequests();
#endif

#ifdef SEARCH
    auto base = std::make_shared<Base>();

    try{
        /// from files
         InvertedIndex::UpdateDocumentBase(base);
         auto result = SearchServer::Search(base);
         ConverterJSON::PutAnswers(base,result);
    }
    catch(const std::string& message){
        std::cerr <<message << std::endl;
    }
#endif

#ifdef DIRECT_INPUT
    auto base = std::make_shared<Base>();

    try{
        /// directly
           const std::vector<std::string> docs = {
                   "Magic Lemon Pie: prepared graham cracker crust, lemon juice, sweetened condensed milk, eggs, cream of tartar, white sugar",
                   "Old-Fashioned Chocolate Fudge: white sugar, unsweetened cocoa powder, whole milk, unsalted butter, vanilla extract",
                   "Best-Ever Birthday Cake:\n"
                   "cooking spray, \n"
                   "eggs, \n"
                   "granulated sugar, \n"
                   "unsalted butter, \n"
                   "vanilla extract, \n"
                   "all-purpose flour, \n"
                   "baking powder, \n"
                   "buttermilk, \n"
                   "multicolored candy sprinkles,\n"
                   "sour cream,\n"
                   "unsweetened cocoa,\n"
                   "powdered sugar",
                   "Lemon Pudding Cakes:\n"
                   "cup granulated sugar\n"
                   "all-purpose flour\n"
                   "eggs\n"
                   "unsalted butter\n"
                   "skim milk\n"
                   "fresh lemon juice\n"
                   "finely grated lemon zest\n"
                   "salt\n"
                   "fresh raspberries or blackberries",
                   "Buckeye Cookies: creamy peanut butter; unsalted butter; vanilla extract; confectioners' sugar; semisweet chocolate chips; shortening",
           };
           auto docs_ptr = std::make_shared<std::vector<std::string>>(std::move(docs));

           const std::vector<std::string> requests = {"lemon, sugar, vanilla",
                                                   "unsalted butter",
                                                   "carrot"};
           auto requests_ptr = std::make_shared<std::vector<std::string>>(std::move(requests));

           InvertedIndex::UpdateDocumentBase(base,docs_ptr);
           auto result = SearchServer::Search(base,requests_ptr);
           ConverterJSON::PutAnswers(base,result);

    }
    catch(const std::string& message){
        std::cerr <<message << std::endl;
    }
#endif
}

