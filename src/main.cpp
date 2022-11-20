#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <iterator>
#include <cctype>

class empty_command_line_exception : public std::exception {
public:
    empty_command_line_exception() {
        
    }
};

class invalid_command_line_argument_exception : public std::exception {
public:
    invalid_command_line_argument_exception() {

    }
};

class Application {
public:
    Application() : _input_line{}, _tokens{} {

    }

public:
    void run() {
        std::getline(std::cin, _input_line);
        _tokens = parse();
        if(validate()) {
            execute();
        }
    }

private:
    std::string _input_line;
    std::vector<std::string> _tokens;

private:
    std::vector<std::string> parse() {
        if (_input_line.empty()) {
            throw empty_command_line_exception();
        }
        std::istringstream iss(_input_line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                        std::istream_iterator<std::string>{}};
        return tokens;
    }

    bool validate() {
        for(const auto& token : _tokens) {
            for(const auto& c : token) {
                if(c != 'k' || c != 'm' || c != 'n') {
                    if(!std::isdigit(c)) {
                        throw invalid_command_line_argument_exception();
                    }
                }
            }
        }
        return true;
    }

    void execute() {
        return;
    }
};

int main() {
    try {
        Application app{};
        app.run();
    } catch(std::exception& ex) {
        std::cout << "exception caught!";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}