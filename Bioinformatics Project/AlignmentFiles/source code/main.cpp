
#include "Alignment.hpp"

int main()
{
    bool local;
    std::string firstSeq;
    std::string secondSeq;
    int matchValue;
    int mismatchValue;
    int gapValue;

    std::cout << "Is local (0 = false, 1 = true): ";
    std::cin >> local;
    std::cout << "First sequence: ";
    std::cin >> firstSeq;
    std::cout << "Second sequence: ";
    std::cin >> secondSeq;
    std::cout << "Match value: ";
    std::cin >> matchValue;
    std::cout << "Mismatch value: ";
    std::cin >> mismatchValue;
    std::cout << "Gap value: ";
    std::cin >> gapValue;

    Alignment newAlignment;
    newAlignment.setValues(matchValue, mismatchValue, gapValue);
    newAlignment.insertSeq(firstSeq, secondSeq, local);
    newAlignment.createMatrix();
    newAlignment.printMatrix();
    std::cout << std::endl;
    newAlignment.outputAlignments();


    return 0;
}

