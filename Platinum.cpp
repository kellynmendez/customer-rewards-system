/** Platinum Customer class
 */

#include "Platinum.h"

Platinum::Platinum(std::string id, std::string first, std::string last,
                   float spent, int bucks) : Customer (id, first, last, spent)
{
    bonusBucks = bucks;
}

