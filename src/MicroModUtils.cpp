#include "../include/MicroModUtils.hpp"
#include <string>
using namespace std;

string MicroModUtils::getVersion()
{
    return MicroModUtils::MICROMOD_VERSION;
}

long MicroModUtils::calculateNumChannels(unsigned char moduleHeader[])
{
    long numchan;
    switch ((moduleHeader[1082] << 8) | moduleHeader[1083])
    {
    case 0x4b2e: /* M.K. */
    case 0x4b21: /* M!K! */
    case 0x542e: /* N.T. */
    case 0x5434: /* FLT4 */
        numchan = 4;
        break;
    case 0x484e: /* xCHN */
        numchan = moduleHeader[1080] - 48;
        break;
    case 0x4348: /* xxCH */
        numchan = ((moduleHeader[1080] - 48) * 10) + (moduleHeader[1081] - 48);
        break;
    default: /* Not recognised. */
        numchan = 0;
        break;
    }
    if (numchan > MicroModUtils::MAX_CHANNELS)
        numchan = 0;
    return numchan;
}

MicroModUtils::MicroModUtils()
{
    cout << "MicroModUtils creation." << endl;
}

MicroModUtils::~MicroModUtils()
{
    MicroModUtils::instance = NULL;
}
