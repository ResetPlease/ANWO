#include "LHTTPI.h"

std::string render_template(std::string path) {

    std::ifstream file;
    file.open(path);
    std::string data = "";
    if(file.is_open()){

        std::string line;
        while(std::getline(file, line)){
            data += (line+"\n");
        }
        file.close();
    }
    else{
        return "Error: error file reading\n";
    }
    return data;
}


std::vector<std::string> split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}
