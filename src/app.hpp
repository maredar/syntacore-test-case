#ifndef STC_SRC_APP_HPP_INCLUDED
#define STC_SRC_APP_HPP_INCLUDED

#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <sstream>

#include "tree.hpp"

namespace STC {

namespace except{

class empty_command_line_exception : public std::exception {
public:
    empty_command_line_exception() {
        
    }

public:
    const char* what() const noexcept override {
        return "Given command line is empty!";
    }
};

class invalid_commands_counter_exception : public std::exception {
public:
    invalid_commands_counter_exception(size_t cur_size) : _err_msg{} {
        _err_msg += "Wrong number of arguments given: ";
        _err_msg += std::to_string(cur_size) + "; ";
        _err_msg += "expected: " + std::to_string(cur_size + 1) + " or " + std::to_string(cur_size - 1);
    }

public:
    const char* what() const noexcept override {
        return _err_msg.c_str();
    }

private:
    std::string _err_msg;
};

class invalid_command_line_argument_exception : public std::exception {
public:
    invalid_command_line_argument_exception(const std::string& failed) : _err_msg{} {
        _err_msg += "Token '" + failed + "' is not valid argument!";
    }

public:
    const char* what() const noexcept override {
        return _err_msg.c_str();
    }

private:
    std::string _err_msg;
};

} //namespace except

class Application {
    struct CliCommand {
        const char cmd;
        const int arg;
        CliCommand(const char cmd, const int arg) : cmd{ cmd }, arg{ arg } {
            //empty
        }
    };
public:
    Application() : _input_line{}, _tokens{}, _tree{} {
        //empty
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
    bool _is_integer(const std::string& token) {
        bool negative_check = token[0] == '-';
        size_t sz = token.length();

        for(size_t i = negative_check ? 1 : 0; i < sz; ++i) {
            if (!std::isdigit(token.at(i))) {
                return false;
            }
        }
        return true;
    }

    std::vector<CliCommand> _parse() {
        if (_input_line.empty()) {
            throw except::empty_command_line_exception();
        }
        std::istringstream iss(_input_line);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
                        std::istream_iterator<std::string>{}};

        size_t sz = tokens.size();
        if (sz % 2 != 0) {
            throw except::invalid_commands_counter_exception(sz);
        }

        std::vector<CliCommand> commands{};
        commands.reserve(sz);

        for(size_t i = 0; i < sz; i += 2) {
            const char* cmd = tokens.at(i).c_str();

            if(cmd != "k" && cmd != "m" && cmd != "n") {
                throw except::invalid_command_line_argument_exception(cmd);
            }
            if (!_is_integer(tokens.at(i+1))) {
                throw except::invalid_command_line_argument_exception(tokens.at(i+1));
            }
            commands.push_back(CliCommand(*cmd, std::stoi(tokens.at(i+1))));
        }
        return commands;
    }

    void _execute() {
        for (const auto& token : _tokens) {
            switch (token.cmd)
            {
            case 'k':
                _tree.insert(token.arg);
                break;
            case 'm':
                std::cout << _tree.find_from_begin(token.arg) << " ";
                break;
            case 'n':
                std::cout << _tree.count_nodes_less(token.arg) << " ";
                break;
            default:
                break;
            }
        }
        std::cout << std::endl;
    }

};

} //namespace STC

#endif