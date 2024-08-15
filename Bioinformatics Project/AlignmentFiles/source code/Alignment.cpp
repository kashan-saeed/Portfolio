#include "Alignment.hpp"


Alignment::Alignment()
    :matchValue{0}, mismatchValue{0}, gapValue{0}
{
}

void Alignment::setValues(int matchValueIn, int mismatchValueIn, int gapValueIn)
{
    matchValue = matchValueIn;
    mismatchValue = mismatchValueIn;
    gapValue = gapValueIn;
}

void Alignment::insertSeq(std::string firstSeqIn, std::string secondSeqIn, bool isLocal)
{
    firstSeq = firstSeqIn;
    secondSeq = secondSeqIn;
    local = isLocal;
}

void Alignment::createMatrix()
{
    for(int i = 0; i < firstSeq.size()+1; ++i)
    {
        std::vector<node> nodeVector;
        for(int j = 0; j < secondSeq.size()+1; ++j)
        {
            node newNode;
            nodeVector.push_back(newNode);
        }
        matrix.push_back(nodeVector);
    }

    for(int i = 0; i < firstSeq.size()+1; ++i)
    {
        for(int j = 0; j < secondSeq.size()+1; ++j)
        {
            if(i == 0 && j == 0)
            {
                (matrix[i][j]).value = 0;
            }
            else
            {
                if(i == 0)
                {
                    int theValue = (matrix[i][j-1]).value + gapValue;
                    if(local == true && theValue < 0)
                    {
                        theValue = 0;
                    }
                    (matrix[i][j]).value = theValue;
                    ((matrix[i][j]).previousPairs).push_back({i, j-1});
                }
                else if(j == 0)
                {
                    int theValue = (matrix[i-1][j]).value + gapValue;
                    if(local == true && theValue < 0)
                    {
                        theValue = 0;
                    }
                    (matrix[i][j]).value = theValue;
                    ((matrix[i][j]).previousPairs).push_back({i-1, j});
                }
                else
                {
                    int top = (matrix[i-1][j]).value + gapValue;
                    int left = (matrix[i][j-1]).value + gapValue;
                    int diagonal;
                    if(firstSeq[i-1] == secondSeq[j-1])
                    {
                        diagonal = (matrix[i-1][j-1]).value + matchValue;
                    }
                    else
                    {
                        diagonal = (matrix[i-1][j-1]).value + mismatchValue;
                    }
                    int maxValue = std::max({top, left, diagonal});

                    if(top == maxValue)
                    {
                        ((matrix[i][j]).previousPairs).push_back({i-1, j});
                    }
                    if(left == maxValue)
                    {
                        ((matrix[i][j]).previousPairs).push_back({i, j-1});
                    }
                    if(diagonal == maxValue)
                    {
                        ((matrix[i][j]).previousPairs).push_back({i-1, j-1});
                    }

                    int theValue = maxValue;
                    if(local == true && theValue < 0)
                    {
                        theValue = 0;
                    }
                    (matrix[i][j]).value = theValue;
                }
                
            }
        }   
    }
}

void Alignment::printMatrix()
{
    std::cout << std::endl;
    std::cout << "    -";
    for(int i = 0; i < secondSeq.size(); ++i)
    {
        std::cout << "      " << secondSeq[i];
    }
    for (int i = 0; i < matrix.size(); i++)
    {
        std::string lineString = "    ";
        std::stringstream buffer;
        if(i == 0)
        {
            buffer << "-";
        }
        else
        {
            buffer << firstSeq[i-1];
        }
        
        for (int j = 0; j < matrix[i].size(); j++)
        {
            bool leftFound = false;
            bool upFound = false;
            bool diagonalFound = false;
            for(int k = 0; k < (matrix[i][j].previousPairs).size(); ++k)
            {
                //if left
                if(((matrix[i][j].previousPairs)[k]).first == i && ((matrix[i][j].previousPairs)[k]).second == j-1)
                {
                    leftFound = true;
                }
                //if up
                if(((matrix[i][j].previousPairs)[k]).first == i-1 && ((matrix[i][j].previousPairs)[k]).second == j)
                {
                    upFound = true;
                }
                //if diagonal
                if(((matrix[i][j].previousPairs)[k]).first == i-1 && ((matrix[i][j].previousPairs)[k]).second == j-1)
                {
                    diagonalFound = true;
                }
            }
            
            if(leftFound == false)
            {
                if(j != 0 )
                {
                    buffer << std::right << std::setw(3) << std::setfill(' ') << "";
                }
            }
            else
            {
                buffer << std::right << std::setw(3) << std::setfill(' ') << "-";
            }
            
            //if it is not the top row
            if(i != 0)
            {
                if(j != 0)
                {
                    if(diagonalFound == false)
                    {
                        lineString = lineString + "   ";
                    }
                    else
                    {
                        lineString = lineString + "\\  ";
                    }
                }
                if(upFound == false)
                {
                    lineString = lineString + "    ";
                }
                else
                {
                    lineString = lineString + "|   ";
                }
            }

            buffer << std::right << std::setw(4) << std::setfill(' ') << matrix[i][j].value;
        }    
        std::cout << lineString << std::endl;
        std::cout << buffer.str() << std::endl; 
    }
}

void Alignment::outputAlignments()
{
    if(local == true)
    {
        outputLocal();
    }
    else
    {
        outputGlobal();
    }
    
}

void Alignment::outputGlobal()
{
    std::stack<std::string> iStack;
    std::stack<std::string> jStack;
    int i = matrix.size()-1;
    int j = matrix[i].size()-1;

    outputGlobalR(i, j, iStack, jStack);
}

void Alignment::outputGlobalR(int i, int j, std::stack<std::string> iStack, std::stack<std::string> jStack)
{
    if(i == 0 && j == 0)
    {
        int jOrigSize = jStack.size();
        for(int j = 0; j < jOrigSize; ++j)
        {
            std::cout << jStack.top();
            jStack.pop();
        }
        std::cout << std::endl;
        
        int iOrigSize = iStack.size();
        for(int i = 0; i < iOrigSize; ++i)
        {
            std::cout << iStack.top();
            iStack.pop();
        }
        std::cout << std::endl << std::endl;

        return;
    }

    std::vector<std::pair<int,int>> pairVector = (matrix[i][j]).previousPairs;
    for(auto iter = pairVector.cbegin(); iter != pairVector.cend(); ++iter)
    {
        //if diagonal
        if(iter->first == i-1 && iter->second == j-1)
        {
            iStack.push(std::string(1,firstSeq[i-1]));
            jStack.push(std::string(1,secondSeq[j-1]));
            outputGlobalR(i-1, j-1, iStack, jStack);
            iStack.pop();
            jStack.pop();
        }
        //if up
        if(iter->first == i-1 && iter->second == j)
        {
            iStack.push(std::string(1,firstSeq[i-1]));
            jStack.push("-");
            outputGlobalR(i-1, j, iStack, jStack);
            iStack.pop();
            jStack.pop();
        }
        //if left
        if(iter->first == i && iter->second == j-1)
        {
            iStack.push("-");
            jStack.push(std::string(1,secondSeq[j-1]));
            outputGlobalR(i, j-1, iStack, jStack);
            iStack.pop();
            jStack.pop();
        }
    }
}


void Alignment::outputLocal()
{
    //find the highest value
    int highestValue = matrix[0][0].value;
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if(highestValue < matrix[i][j].value)
            {
                highestValue = matrix[i][j].value;
            }
        }
    }

    //if that position contains the highest value
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if(matrix[i][j].value == highestValue)
            {
                std::stack<std::string> iStack;
                std::stack<std::string> jStack;
                outputLocalR(i, j, iStack, jStack);
            }
        }
    }
}

void Alignment::outputLocalR(int i, int j, std::stack<std::string> iStack, std::stack<std::string> jStack)
{
    if(matrix[i][j].value == 0)
    {
        int jOrigSize = jStack.size();
        for(int j = 0; j < jOrigSize; ++j)
        {
            std::cout << jStack.top();
            jStack.pop();
        }
        std::cout << std::endl;
        
        int iOrigSize = iStack.size();
        for(int i = 0; i < iOrigSize; ++i)
        {
            std::cout << iStack.top();
            iStack.pop();
        }
        std::cout << std::endl << std::endl;

        return;
    }

    std::vector<std::pair<int,int>> pairVector = (matrix[i][j]).previousPairs;
    for(auto iter = pairVector.cbegin(); iter != pairVector.cend(); ++iter)
    {
        //if diagonal
        if(iter->first == i-1 && iter->second == j-1)
        {
            iStack.push(std::string(1,firstSeq[i-1]));
            jStack.push(std::string(1,secondSeq[j-1]));
            outputLocalR(i-1, j-1, iStack, jStack);
            iStack.pop();
            jStack.pop();
        }
        //if up
        if(iter->first == i-1 && iter->second == j)
        {
            iStack.push(std::string(1,firstSeq[i-1]));
            jStack.push("-");
            outputLocalR(i-1, j, iStack, jStack);
            iStack.pop();
            jStack.pop();
        }
        //if left
        if(iter->first == i && iter->second == j-1)
        {
            iStack.push("-");
            jStack.push(std::string(1,secondSeq[j-1]));
            outputLocalR(i, j-1, iStack, jStack);
            iStack.pop();
            jStack.pop();
        }
    }
}