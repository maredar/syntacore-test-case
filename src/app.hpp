#ifndef STC_SRC_APP_HPP_INCLUDED
#define STC_SRC_APP_HPP_INCLUDED

#include <iostream>
#include <string>
#include <queue>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <iterator>
#include <cctype>
#include <regex>

#include "tree.hpp"

namespace STC {

namespace except{

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

}

class Application {
    struct CliCommand {
        const char* cmd;
        const int arg;
        CliCommand(const char* cmd, const int arg) : cmd{ cmd }, arg{ arg } {
            //empty
        }
    };
public:
    Application() : _input_line{}, _tokens{}, _tree{} {

    }

public:
    void run() {
        std::getline(std::cin, _input_line);
        _tokens = _parse();
        _execute();
        _tree.print();
    }

private:
    std::string _input_line;
    std::vector<CliCommand> _tokens;

    Containers::RBTree _tree;

private:
    bool _is_integer( std::string token ) {
        return std::regex_match(token, std::regex(("((\\+|-)?[[:digit:]]+)?")));
    }

    std::vector<CliCommand> _parse() {
        if (_input_line.empty()) {
            throw except::empty_command_line_exception();
        }
        std::istringstream iss(_input_line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                        std::istream_iterator<std::string>{}};

        std::vector<CliCommand> commands{};
        commands.reserve(tokens.size());

        if(_validate(tokens)){
            for(int i = 0; i < tokens.size(); i += 2) {
                commands.push_back(CliCommand(tokens[i].c_str(), stoi(tokens[i+1])));
            }
        }
        return commands;
    }

    bool _validate(const std::vector<std::string>& tokens) {
        size_t sz = tokens.size();
        if (sz % 2 != 0) {
            throw except::invalid_command_line_argument_exception();
        }
        for(int i = 0; i < sz; i += 2) {
            const std::string* token = &tokens.at(i);
            const std::string* arg = &tokens.at(i+1);

            if (*token != "k" && *token != "m" && *token != "n") {
                throw except::invalid_command_line_argument_exception();
            }
            if (!_is_integer(*arg)) {
                throw except::invalid_command_line_argument_exception();
            }
        }
        return true;
    }

    void _execute() {
        for (const auto& token : _tokens) {
            switch (*token.cmd)
            {
            case 'k':
                _tree.insert(token.arg);
                break;
            case 'm':
                //_tree.find_from_begin(token.arg);
                break;
            case 'n':
                //__tree.count_nodes_under(token.arg);
                break;
            default:
                break;
            }
        }
    }

};

} //namespace STC

#endif