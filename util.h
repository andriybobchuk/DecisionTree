#pragma once
#pragma warning(disable : 4996)
#include <fstream>
#include <map>
#include <list>
#include <array>
#include <iostream>
#include "tree.h"

// Collection of utill functions
namespace util {


	// Convert string to float function
	float parseFloat(char* token) {//take char array
		size_t i(0);//unsigned int i = 0;
		float f = std::stof(token, &i); // convert chaar array token to float

		if (std::string(token).size() != i) { // if token is NOT 0 so far, then smth went wrong
			throw std::invalid_argument("Invalid data, failed to parse to float."); // print an error
		}

		return f; // function returns float
	}


	// Convert string to int function
	int parseInt(char* token) {//take char array
		size_t i(0);//unsigned int i = 0;

		int x = 0 ; // variable to which we'll write out int token

		//if token is not 0
		if (token)
		{
			x = std::stoi(token, &i); // convert token from char array to int

			// if token is NOT 0 so far, then smth went wrong
			if (std::string(token).size() != i) {
				throw std::invalid_argument("Invalid data, failed to parse to int.");// print an error
			}
		}
		return x; // return int value of token
	}


	// Copy string function
	char* c_str(std::string str) { // take a string

		char* copy = new char[str.length() + 1]; // declare new char array to which we'll copy

		strcpy(copy, str.c_str()); // copy from STR to COPY

		return copy; // return copied string
	}


	// Function for recognizing operators < & >
	Op parseOp(char* token) { // take token

		if (std::string(token) == ">") {
			return Op::GREATER;
		}
		else if (std::string(token) == "<") {
			return Op::LESS;
		}
		else throw std::invalid_argument("Invalid data, failed to parse to Op");
	}



    // Convert operator back to string
	std::string toString(Op op) {

		//if (op == Op::GREATER)
		//{
		//	return ">";
		//}
		//else
		//{
		//	return "<";
		//}

		return op == Op::GREATER ? ">" : "<"; // short form of if statement
	}



	//Open file
	int getOpenModeForType(ParamType type) {
		if (type == ParamType::OUTPUT_FILE)
			return std::ios::out;//writting stream
		else return std::ios::in;//reading stream
	}

	// count number of rows in the file
	int countRows(std::fstream* stream) {
		std::string row;
		getline(*stream, row);
		int size = 0;
		while (!stream->eof()) {
			size++;
			getline(*stream, row);
		}
		stream->clear();                 // clear fail and eof bits
		stream->seekg(0, std::ios::beg); //returns the cursor to the beginning of the file
		return size;
	};



	// Convert list to an array
	std::string* listToArray(std::list<std::string> l) {
		std::string* arr = new std::string[l.size()];
		size_t i = 0;
		for (auto it = l.begin(); it != l.end(); it++) {
			arr[i] = it->c_str();
			i++;
		}
		return arr;
	}

	//print the whole list to the file
	void writeList(std::fstream* stream, std::list<float> l) {
		for (auto it = l.begin(); it != l.end(); it++) {
			*stream << *it << " ";
		}
		*stream << "\n";
	}

	//// Print list to file
	//void writeList(std::fstream* stream, std::list<std::string> l) {
	//	for (auto it = l.begin(); it != l.end(); it++) {
	//		*stream << *it << " ";
	//	}
	//	*stream << "\n";
	//}



	// write result data to file
	void writeData(std::fstream* stream, std::map<std::string, std::list<std::list<float>>> data
		, std::list<std::string> attrList) {

		for (auto d = data.begin(); d != data.end(); d++) {
			*stream << d->first.c_str() << "\n";
			writeList(stream, attrList);
			for (auto dd = d->second.begin(); dd != d->second.end(); dd++) {
				writeList(stream, *dd);
			}
			*stream << "\n\n";
		}
	}


	// function for stopping program
	void exit() {
		system("pause");
		std::exit(0);
	}

}
