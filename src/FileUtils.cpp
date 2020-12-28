
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "../include/FileUtils.hpp"

using namespace std;

int FileUtils::getSize(std::string fileName)
{
    unsigned int result = 0;
    ifstream infile;
    infile.open(fileName.c_str(), ios::in | ios::binary);
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
    infile.close();
    return result;
}

vector<unsigned char> FileUtils::readFile(string fileName)
{
    vector<unsigned char> results;
    ifstream infile;
    infile.open(fileName.c_str(), ios::in | ios::binary);
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
    infile.close();
    return results;
}

vector<unsigned char> FileUtils::readFile(std::string fileName, unsigned int from, unsigned int length)
{
    vector<unsigned char> results;
    ifstream infile(fileName, ios_base::binary);
    if (infile)
    {
        infile.seekg(from);
        for (unsigned int i = 0; i < length; i++)
        {
            results.push_back(infile.get());
        }
    }
    else
    {
        cout << "Error: the file " << fileName << "doesn't exist." << endl;
    }
    infile.close();
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
