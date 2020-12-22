
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../include/FileUtils.hpp"

using namespace std;

int FileUtils::getSize(std::string fileName)
{
    unsigned int result = 0;
    ifstream infile(fileName, std::ifstream::binary);
    if (infile)
    {
        infile.seekg(0, ios_base::end);
        result = infile.tellg();
    }
    else
    {
        cout << "Error: the file " << fileName << "doesn't exist." << endl;
        result = -1;
    }
    return result;
}

vector<unsigned char> FileUtils::readFile(string fileName)
{
    vector<unsigned char> results;
    ifstream infile(fileName);
    if (infile)
    {
        unsigned char tmp = infile.get();
        do
        {
            results.push_back(tmp);
            tmp = infile.get();
        } while (!infile.eof());
    }
    else
    {
        cout << "Error: the file " << fileName << "doesn't exist." << endl;
    }

    return results;
}

FileUtils::FileUtils()
{
    cout << "FileUtils singleton creation." << endl;
}

FileUtils::~FileUtils()
{
    FileUtils::instance = NULL;
}