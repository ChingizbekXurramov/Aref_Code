#include <iostream> 
#include <fstream> 
#include <string> 
#include <vector> 
#include <cmath> 

using namespace std;
typedef long long Longir;


long long leadingDigitThreshold = pow(10, 9);
long long underflowThreshold = pow(10, 8);


Longir stringToLongLong(string inputString) {
    Longir result = 0;
    Longir weight = 1;


    for (int i = inputString.length() - 1; i >= 0; i--) {
        result += (inputString[i] - '0') * weight;
        weight *= 10;
    }

    return result;
}


void encode() {
    string inputMessage = "";
    vector<int> freqTable;
    int totalChar = 256;

    Longir underflowFlag = -1;
    Longir underflowCount = 0;

    Longir l = 0;
    Longir h = 9999999999;

    ifstream inputFile("input.txt");
    string temp;


    while (getline(inputFile, temp)) {
        if (inputMessage == "") {
            inputMessage = temp;
        }
        else {
            inputMessage += '\n' + temp;
        }
    }

    inputMessage += "#";


    for (int i = 0; i <= 255; i++) {
        freqTable.push_back(1);
    }

    ofstream outputFile("output.bin", ios::binary);
    string encodedMessage = "";


    for (int i = 0; i < inputMessage.length(); i++) {
        int characterPosition = int(inputMessage[i]);

        Longir intervalRange = (h - l) + 1;
        Longir cumulativeFrequency = 0;


        for (int k = 0; k < characterPosition; k++) {
            cumulativeFrequency += freqTable[k];
        }


        h = l + intervalRange * (cumulativeFrequency + freqTable[characterPosition]) / (totalChar + 1) - 1;
        l = l + intervalRange * cumulativeFrequency / (totalChar + 1);

        totalChar++;
        freqTable[characterPosition]++;


        while ((l / leadingDigitThreshold) == (h / leadingDigitThreshold)) {
            Longir digit = l / leadingDigitThreshold;
            encodedMessage = encodedMessage + char(l / leadingDigitThreshold + '0');
            l = (l % leadingDigitThreshold) * 10;
            h = (h % leadingDigitThreshold) * 10 + 9;

            char underflowDigit = '0';


            if (digit == underflowFlag) {
                underflowDigit = '9';
            }


            while (underflowCount) {
                underflowCount--;
                encodedMessage = encodedMessage + underflowDigit;
            }

            underflowFlag = -1;
        }


        while (h - l < underflowThreshold) {
            if (underflowFlag == -1) {
                underflowFlag = l / leadingDigitThreshold;
            }

            underflowCount++;

            int temp = l / leadingDigitThreshold;


            l = temp * leadingDigitThreshold + (l % underflowThreshold) * 10;
            temp = h / leadingDigitThreshold;
            h = temp * leadingDigitThreshold + (h % underflowThreshold) * 10 + 9;
        }
    }


    Longir finalCode = (l + h) / 2;
    string finalCodeString = to_string(finalCode);


    while (finalCodeString.length() < 10) {
        finalCodeString = finalCodeString + "0";
    }

    encodedMessage = encodedMessage + finalCodeString;


    for (char c : encodedMessage) {
        outputFile.write(&c, sizeof(char));
    }

    cout << "Encoding of the file was successful!" << endl;
}



int main() {
    encode();
    system("pause");

    return 0;
}