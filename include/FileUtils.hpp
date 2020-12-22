#include <iostream>
#include <string>
#include <vector>

class FileUtils
{
public:
    std::vector<unsigned char> readFile(std::string fileName);

    std::vector<unsigned char> readFile(std::string fileName, unsigned int from, unsigned int length);

    int getSize(std::string fileName);

    static inline FileUtils *getInstance()
    {
        if (instance)
        {
            instance = new FileUtils();
        }
        return instance;
    }

private:
    static inline FileUtils *instance = NULL;
    FileUtils();
    ~FileUtils();
};