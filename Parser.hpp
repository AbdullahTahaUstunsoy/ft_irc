#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include <vector>


class Parser
{
	private:
		std::string message;
		std::string command;
		std::vector<std::string> parameters;

	public:
		Parser();
        void get_message(std::string raw_message);
		int parse_message();
		const std::string& get_command() const;
		const std::vector<std::string>& get_parameters() const;
		bool is_valid_command(const std::string &cmd) const;
		void clear_params();
		~Parser();
};

#endif