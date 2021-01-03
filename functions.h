#pragma once

#ifndef FUNCTIONS_H  // if FUNCTIONS_H is not defined ...
#define FUNCTIONS_H //define it

/// <summary>
/// function for checking if the InputFile exists
/// </summary>
/// <param name="argc">argument count</param>
/// <param name="argv">argument array</param>
/// <returns>true is validated successfully</returns>
bool validateArgs(int argc, char* argv[]); 

/// <summary>
/// opens and read InputFile
/// </summary>
/// <returns>true if opened and read file successfully</returns>
bool extractInputData(); 

/// <summary>
/// open and read TreeDefine file
/// </summary>
/// <returns>true if opened and read file successfully</returns>
bool extractTreeData(); 

/// <summary>
/// receives each node from tableRows ad sends it for the evaluation by calling the evaluate() function for this node
/// </summary>
/// <returns>true if ended successfully</returns>
bool prepareResults();

/// <summary>
/// prints help message, returns nothing
/// </summary>
void showManual(); 

/// <summary>
/// takes a node and go to its children and children of children until pointers to them exist
/// </summary>
/// <param name="row">Input file in format of float numbers</param>
/// <param name="node">struct node with all of its fields</param>
/// <param name="index">index of currect node</param>
void evaluateNode(std::list<float>, Node, int);

/// <summary>
/// function only receives the row and puts it into the map "results". 
/// </summary>
/// <param name="row">row which it takes</param>
/// <param name="resultGroup">results string where it writes the content of list "row"</param>
void saveRow(std::list<float>, std::string);

/// <summary>
/// prints what piece of data you missed to enter in console
/// </summary>
void showMissingArguments();


#endif  