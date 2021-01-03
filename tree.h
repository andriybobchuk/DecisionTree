#pragma once
#include <string>


/* List of parameters (command line args) variables */
enum class ParamType {
	INPUT_FILE, TREE, OUTPUT_FILE
};


/* function ParseOp uses it, just a word representation of < and > */
enum class Op {

	GREATER, LESS
};



// Sttructure for describing the condition which consists of operator, value and attribute
struct Condition {
private:
	Op cond_operator;///< operator
	float value;///<value
	std::string attr;///<attribute
public:
	Condition(Op oper, float val, std::string attr) : cond_operator(oper), value(val), attr(attr) {}

	std::string getAttr() {
		return attr;
	}

	// method which returns true is x > value and false if x < value
	bool eval(float x) {
		if (cond_operator == Op::GREATER) {
			return x > value;
		}
		else return x < value;
	}
};

size_t numberOfResults = 0;

//Structure of NODES
struct Node {
	int index; ///< index of each node
	Condition* cond; ///< Condition which was descpibed above (consists of 3 keys)
	std::string falseResult, trueResult; ///< string for false and true result
	int falseIndex, trueIndex; ///< index of node where to go in case of false and true results
	bool falseIndexExists, trueIndexExists; ///< is there an index for false/true result?
public:
	Node(int index, Op greaterOrLess, float val, std::string attribute, bool falseIndexExists,
		bool trueIndexExists, int falseIndex, int trueIndex, std::string falseResult, std::string trueResult) :
		index(index), cond(new Condition(greaterOrLess, val, attribute)),
		falseIndexExists(falseIndexExists), trueIndexExists(trueIndexExists) 
	{
		if (falseIndexExists)
		{
			this->falseIndex = falseIndex; //assign false index in this context
		}
		else
		{
			this->falseResult = falseResult;//assign false result in this context
			numberOfResults++; // increase number of results

		}

		//The same but for true index
		if (trueIndexExists) 
			this->trueIndex = trueIndex;
		else 
		{
			this->trueResult = trueResult;
			numberOfResults++;
		}
	}

	/* headers for other functions */
	Condition* getCond() {
		return cond;
	}
	bool hasFalseIndex() {
		return falseIndexExists;   
	}
	bool hasTrueIndex() {
		return trueIndexExists;
	}
	int getTrueIndex() {
		return trueIndex;
	}
	int getFalseIndex() {
		return falseIndex;
	}
	int getIndex() {
		return index;
	}
	std::string getTrueResult() {
		return trueResult;
	}
	std::string getFalseResult() {
		return falseResult;
	}
	~Node() {

	}

}; 
