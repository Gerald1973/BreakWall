
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../include/FileUtils.hpp"

using namespace std;

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