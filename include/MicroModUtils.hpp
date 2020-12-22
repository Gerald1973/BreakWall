#include <iostream>
#include <string>
#include <vector>

class MicroModUtils
{
public:
   
    static inline int MAX_CHANNELS = 16;

    static inline MicroModUtils *getInstance()
    {
        if (instance)
        {
            instance = new MicroModUtils();
        }
        return instance;
    }

     std::string getVersion();
     long calculateNumChannels(unsigned char moduleHeader[]);

private:
    static inline MicroModUtils *instance = NULL;
    static inline std::string MICROMOD_VERSION = "Based on: Micromod Protracker replay 20180625 (c)mumart@gmail.com";
    MicroModUtils();
    ~MicroModUtils();
};