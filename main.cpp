#include <iostream>
#include <string>
#include <unistd.h>
#include <map>
#include <iterator>
#include <vector>
#include <math.h>
#include <pthread.h>

using namespace std;

// create a struct that has integer value, numBits, pointer to the compressed message
struct Data {
    int base10Val;
    string binary;
    char letter;
    int numBits;
    int frequency;
    string message;
};

struct Decompress {
    char let;
    map<string,char> *alphabet;
    string mes;
};

void * binaryAndFrequency(void* data) {
    Data* structData = (Data*) data; // typecast the pointer to a struct pointer
    string base2 = "";
    string newBinary = "";
    string bstr = "", bstr2 = "";
    int val = structData->base10Val;
    int bits = structData->numBits;
    int bNums[bits];

    int remainder = 0;
    int j = 0;
    while (val != 0) {
        remainder = val % 2;
        val /= 2;
        bNums[j] = remainder;
        j++;
    }

    for (int i = j-1; i >= 0; i--) {
        bstr.append(to_string(bNums[i]));
    }

    if (bstr.length() < bits) {
        bstr2.append(bits - (int)bstr.length(), '0');
        bstr = bstr2 + bstr;
    }
    
    structData->binary = bstr;

    // binary has been generated . . . move on to calculating the frequency
    string mes = structData->message;
    int amt = 0;
    int start = 0;
    while (mes != "") {
        // cout << tempMes << endl;
        if (mes.substr(0, structData->binary.size()) == structData->binary) {
            amt++;
        }
        mes.erase(start, structData->binary.size());
    }
    structData->frequency = amt;
    
    return nullptr;
}

void * decompress(void* structWithMap) {
    Decompress* decom = (Decompress*) structWithMap;
    string mes = decom->mes;
    map<string,char>::iterator it;
    it = decom->alphabet->find(mes);
    decom->let = it->second;

    return nullptr;
}

int main() {

    string line;
    string n_str; // n lines and n number of symbols in this alphabet
    int n, maxNum = 0;
    vector<Data> vecData; // VECTOR OF STRUCT
    string message; // declare compressed message variable 

    getline(cin, n_str);
    n = stoi(n_str);

    // get all letters and their code value //
    for (int k = 0; k < n; k++) {
        // getline(input,line);
        getline(cin,line); // this is input redirection

        // VECTOR
        Data temp;
        temp.base10Val = stoi(line.substr(2));
        temp.letter = line[0];
        temp.binary = "";
        temp.frequency = 0;
        vecData.push_back(temp);

        if (maxNum < stoi(line.substr(2))) { // get the max code value
            maxNum = stoi(line.substr(2));
        }
    } 
    // get last string which is the compressed message //

    getline(cin,line); // this is input redirection
    message = line; // message variable defined

    // calculate the # of bits used per symbol //
    int numBits = (int)ceil(log2(maxNum + 1));

    // lets do the tasks

    for (int i = 0; i < vecData.size(); i++) {
        vecData[i].numBits = numBits;
        vecData[i].message = message;
    }

    // threads will convert the value to binary then count the # of occurences in the message

    // pthread_t pthread[n]; // make array to hold pthreads // make it a dynamic or static array instead //
    pthread_t* pthread = new pthread_t[n];

    for (int i = 0; i < n; i++) {
        if (pthread_create(&pthread[i], nullptr, binaryAndFrequency, &vecData[i]))
		{
			fprintf(stderr, "Error creating thread\n");
			return 1;
		} 
    }
    for (int i = 0; i < n; i++) {
            pthread_join(pthread[i], nullptr);
    }
    // store char and binary string into map for next set of threads and 2nd struct //
    map<string,char> *alphabet = new map<string,char>;

    int lenOfDecMes = 0;
    cout << "Alphabet:\n";
    for (int i = 0; i < n; i++) {
        alphabet->insert(pair<string,char>(vecData[i].binary,vecData[i].letter));
        cout << "Character: " << vecData[i].letter << ", Code: " << vecData[i].binary << ", Frequency: " << vecData[i].frequency << endl;
        lenOfDecMes += vecData[i].frequency;
    }

    pthread_t *pthread2 = new pthread_t[lenOfDecMes];
    vector<Decompress> vecData2;

    for (int i = 0; i < lenOfDecMes; i++) {
        Decompress d;
        d.alphabet = alphabet; // assigned struct pointer to point to this map in main
        d.mes = message.substr((i*numBits),numBits); // give struct the section of the message to be decompressed
        vecData2.push_back(d);
    }

    // create lenOfDecMes threads to determine each character in the encoded message
    for (int i = 0; i < lenOfDecMes; i++) {
        pthread_create(&pthread2[i], nullptr, decompress, &vecData2[i]); 

        // pass the substring with only the bits needed, pass a pointer to our alphabet so we dont have to store the entire alphabet in our struct
        // compare the substring with our alphabet and store the letter used
    }

    for (int i = 0; i < lenOfDecMes; i++) {
        pthread_join(pthread2[i], nullptr);
    }
    cout << endl << "Decompressed message: ";
    for (int i = 0; i < lenOfDecMes; i++) {
        cout << vecData2[i].let;
    }
    cout << endl;
    delete alphabet;
    delete[] pthread;
    delete[] pthread2;
    return 0;
}