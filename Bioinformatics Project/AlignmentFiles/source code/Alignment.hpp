#ifndef ALIGNMENT_HPP
#define ALIGNMENT_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stack>
#include <iomanip>
#include <sstream>


struct node
{
    std::vector<std::pair<int, int>> previousPairs;
    int value;
};


class Alignment
{
public:
    Alignment();

    void setValues(int matchValueIn, int mismatchValueIn, int gapValueIn);

    void insertSeq(std::string firstSeq, std::string secondSeq, bool isLocal);

    void createMatrix();

    void outputAlignments();

    void printMatrix();

private:
    void outputGlobal();
    void outputGlobalR(int i, int j, std::stack<std::string> iStack, std::stack<std::string> jStack);
    void outputLocal();
    void outputLocalR(int i, int j, std::stack<std::string> iStack, std::stack<std::string> jStack);
    

private:
    int matchValue;
    int mismatchValue;
    int gapValue;
    bool local;
    std::vector<std::vector<node>> matrix;
    std::string firstSeq;
    std::string secondSeq;
};


#endif // ALIGNMENT_HPP