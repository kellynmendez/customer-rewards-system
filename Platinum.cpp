/** Kellyn Mendez - CS 2337.003, Project 1
 *      NetID: kpm200000
 */

#include "Platinum.h"

Platinum::Platinum(std::string id, std::string first, std::string last,
                   float spent, int bucks) : Customer (id, first, last, spent)
{
    bonusBucks = bucks;
}

