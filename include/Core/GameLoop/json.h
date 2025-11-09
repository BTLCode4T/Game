#ifndef JSONPROCESSOR_H
#define JSONPROCESSOR_H

#include <string>
#include "../../src/Utils/json.hpp" 

using namespace std;
using json = nlohmann::json;

class JsonProcessor {
public:
    
    void updateAndWriteData(const string& filename);
    void readAndPrintData(const string& filename);
};

#endif 