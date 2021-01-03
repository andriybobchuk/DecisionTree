#include "util.h"
#include "functions.h"

std::list<std::string> attrList; // InputFile: first line with column titles

std::map<int, std::list<float>> tableRows; // InputFile: each line with it's values

/* reading command line arguments*/
std::map<std::string, ParamType> typeMap; // Map where 1st elemnt is the flag & 2nd - meaning of it
// e.g. ""-i" - INPUT_FILE "
//        -o   - OUTPUT
//        -t   - TREEDEFINE

std::map<ParamType, std::fstream*> paramMap; // Map with the INPUT/TREE/OUTPUT on the first place and corresponding filestreams on the second place




std::map<int, Node*> treeMap; // decision tree (1st - index of node; 2nd - node itself)

std::map<std::string, std::list<std::list<float>>> results; // map which holds the final results (decisions)


// Reset everything to -1 because these are impossible values
int rowCount = 0;
int columnCount = 0;

// assign flags to its definitions
void typeMapInit() {
	typeMap["-i"] = ParamType::INPUT_FILE;
	typeMap["-o"] = ParamType::OUTPUT_FILE;
	typeMap["-t"] = ParamType::TREE;
}

//////////////////////

using namespace std;

int main(int argc, char* argv[]) {
	typeMapInit();

	// we are running the important functions one by one and if one of them is not working we
	// stop execution of the program

	if (argc == 1) // if we have 0 arguments at program start
	{
		showManual(); // print help message
		util::exit(); // terminate the prog
	}
	else if (!validateArgs(argc, argv)) util::exit(); // terminate if arguments are not valid

	if (!extractInputData()) util::exit(); // terminate if couldn't extract data from InputFile

	if (!extractTreeData()) util::exit(); // terminate if couldn't extract data from TreeDefine file

	if (!prepareResults()) util::exit(); // terminate if couldn't prepare results

	util::writeData(paramMap.at(ParamType::OUTPUT_FILE), results, attrList); // Show results in output file

}

/*
* Return true if extraction was successful otherwise false.
*/
bool extractInputData() {
	//gjdgk ejgskmjhbges hjgdsvhdj,mvs
	fstream* input = paramMap.at(ParamType::INPUT_FILE); // open Input file
	rowCount = util::countRows(input); // count the number of rows in this file

	/**the following block processes first row -> attributes and saves it to attribute list*/
	string line = "";

	getline(*input, line);
	string attr = "";

	for (char x : line)
	{
		if (x == ' ')
		{
			attrList.push_back(attr); // put space
			attr = "";
		}
		else if (x == '%') {
			attrList.push_back(attr); // put nothing
			break;
		}
		else {
			attr = attr + x; // put value
		}
	}
	columnCount = attrList.size();
	cout << "Data table size = " << rowCount << "x" << columnCount << endl;





	/** the following block processes values and saves them to values list using token*/
	int i = 0;
	while (!input->eof()) { //while not end of input file
		list<float> values;
		getline(*input, line); // get each line

		//122 121 122 21
		char* copy = util::c_str(line); //copy LINE to COPY variable

		char* token = strtok(copy, " "); //Token is only a splitted word from the whole line


		while (token != NULL) { // if it exists
			try {
				float f = util::parseFloat(token); //convert it to float
				values.push_back(f); // put it to values vector
				token = strtok(NULL, " "); // resets the token
			}
			catch (invalid_argument ia) { // otherwise print error
				cerr << "\nData extraction failed...\n";
				return false;
			}
		}
		// jgfjhfgh jghfjhvf jghfjhvfh
		//   1          2        3         4
		if (values.size() < columnCount) { // if we do not have enough values in our row
			cerr << "\nNumber of values didn't match in row #" << i << ", failed to process data...\n\n";
			return false;
		}
		//if (values.size() > columnCount) { // if we do not have enough values in our row
		//	cerr << "\nNumber of values didn't match in row #" << i << ", failed to process data...\n\n";
		//	return false;
		//}
		tableRows.insert({ i++,values }); // go next

	}
	cout << "\nData extraction successful!\n\n";
	return true;
}

bool extractTreeData() {
	fstream* input = paramMap.at(ParamType::TREE);// open treeDefine file

	//process first row -- attributes
	int lineNumber = 0;

	while (!input->eof()) {// while not end of treeDefine file
		string line = "";

		getline(*input, line); // get each line of the file

		char* copy = util::c_str(line); //copy line

		char* token = strtok(copy, "%"); //remove the comment

		token = strtok(token, " "); // cut the line into words by space

		try {
			bool falseIndexExists = false, trueIndexExists = false; //initially we do not have the indices
			int falseIndex = -1, trueIndex = -1; // indices set to -1 -> impossible state
			string falseResult, trueResult; // declare strings for false and true results



			/*This block extracts index, attribute, operator, value, false\true index etc from each line,
			 converts it to numbers if needed using parseInt(), parseFloat()..*/
			int index = util::parseInt(token);
			token = strtok(NULL, " "); //reset the token | token = 0

			string attr = token;
			token = strtok(NULL, " ");

			Op op = util::parseOp(token);
			token = strtok(NULL, " ");

			float val = util::parseFloat(token);
			token = strtok(NULL, " ");

			try {
				falseIndex = util::parseInt(token); //1
				falseIndexExists = true;
			}
			catch (invalid_argument) {
				falseResult = token; // basketball
			}
			token = strtok(NULL, " ");

			try {
				trueIndex = util::parseInt(token);
				trueIndexExists = true;
			}
			catch (invalid_argument) {
				trueResult = token;
			}

			/*Then we put our ready date for the current line as a node to the map of nodes */
			treeMap.insert({ index, new Node(index, op, val, attr, falseIndexExists,
				trueIndexExists, falseIndex, trueIndex, falseResult, trueResult) });
		}

		/*Further just error messages*/
		catch (invalid_argument ia) {
			cerr << ia.what() << endl;
			return false;
		}

		token = strtok(NULL, " ");
		if (token != NULL) {
			cerr << "Extra elements in the tree structure... At line " << lineNumber + 1 << ", starting at: " << token << endl;
			return false;
		}
		lineNumber++; // GO TO THE NEXT LINE
	}
	cout << "\nTree structure exctraction successful...\n\n";
	return true;
}

bool prepareResults() {// util func which takes 1 node each time and sends it for the evaluation

	for (auto it = tableRows.begin(); it != tableRows.end(); it++) {

		//take node data
		list<float> row = it->second;
		int rowId = it->first;
		Node node = *treeMap.begin()->second;
		int index = treeMap.begin()->first;

		//send for the evaluation
		evaluateNode(row, node, index);

	}
	return true;
}


/* So in this function we are  going through the decision tree from one node to another */
void evaluateNode(list<float> row, Node node, int index) {
	auto iterAttr = attrList.begin(); // iterator for attributes
	auto iterRow = row.begin(); // iterator for rows


	while (iterAttr != attrList.end()) {//while not end of attr list
		if (iterAttr->c_str() == node.getCond()->getAttr()) { //take the condition
			if (node.getCond()->eval(*iterRow)) {
				if (node.hasTrueIndex()) { //if node has true index
					Node nn = *treeMap.at(node.getTrueIndex());//get this index
					evaluateNode(row, nn, nn.getIndex());//and run the func recursively but with the POINTED node
				}
				else {
					saveRow(row, node.getTrueResult()); // else just save this node for result because it has no pointers to other nodes
					return;
				}
			}
			else {
				if (node.hasFalseIndex()) {//if node hase false index
					Node nn = *treeMap.at(node.getFalseIndex());//get this index
					evaluateNode(row, nn, nn.getIndex());//and run the function again but now for the new (POINTED) node
				}
				else {
					saveRow(row, node.getFalseResult());// else just save this node for result because it has no pointers to other nodes
					return;
				}
			}
		}
		iterAttr++; iterRow++;//increase counters
	}
}



// this function receives the row and puts it into the map "results". 
//That's all. then we move on to our main func and print these results
void saveRow(list<float> row, std::string resultGroup) {

	if (results.find(resultGroup) == results.end()) {
		results.insert({ resultGroup, list<list<float>>() });
	}
	results.at(resultGroup).push_back(row);
}



/* this big and scary thing just checks if the input file exists, otherwise it throws a lot of errors*/
bool validateArgs(int argc, char* argv[]) {

	for (int i = 1; i < argc; i++) { // loop: for each argument the user entered

		string x = argv[i]; // current x == current argument with the index i

		try {
			ParamType type = typeMap.at(x); //if we fount the marker of the type ("-i"/"-o"/"-t")
			typeMap.erase(x); // erase this flag
			string fileName = argv[++i]; //and save NEXT element because the next element AFTER THE FLAG is an actual filename

			fstream* fileStream = new fstream(fileName.c_str(), util::getOpenModeForType(type));//open filestream and......

			// check if it is opened
			if (fileStream->good()) {
				paramMap.insert({ type, fileStream });//insert "-i" and input filestream to the paramMap
				continue;
			}
			else { // else it does not exist (because we couldn't open it)
				throw "File : " + fileName + " does not exist!";
			}
		}
		catch (out_of_range e) { //if not the normal number of arguments
			showMissingArguments(); // show what is missing
			return false;
		}
		catch (string err) { // if another error
			cout << err << endl; //print it
			return false;
		}
	}
	if (typeMap.size() > 0) {
		showMissingArguments();//again show what we forgot to enter
		return false;
	}
	cout << "\nInput arguments validated successfully...\n\n";
	return true;
}


/*prints what piece of data you missed in your file*/
void showMissingArguments() {
	cerr << "Input argument validation failed...\n   Following operators missing: ";

	map<string, ParamType>::iterator it; //iterator to go through the map of arguments
	for (it = typeMap.begin(); it != typeMap.end(); it++) { //from begin to end of this map
		cout << it->first << " "; //cout everything
	}
	cout << endl; // new line
}


/*help message*/
void showManual() {
	fstream stream = fstream("manual.txt", ios::in); //opens manual file
	string row; 
	while (!stream.eof()) { // while not end of file
		getline(stream, row); //read each line from file
		cout << row << endl; // print each line to console
	}
}