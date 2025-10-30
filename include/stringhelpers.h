#include <vector>
#include <string>

// tokenizes a list by the delimiter
std::vector<std::string> split(std::string in, char delim) {
    std::vector<std::string> out;
    int start = 0;
    int end = 0;
    for (int i = 0; i <= in.size(); i++) {
        if (in[i] == delim || i == in.size()) {
            end = i;
            std::string word = "";
            word.append(in, start, end - start);
            if (!word.empty()) {
                out.push_back(word);
                start = end + 1;
            }
        }
    }
    return out;
}