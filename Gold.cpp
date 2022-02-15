/** Kellyn Mendez - CS 2337.003, Project 1
 *      NetID: kpm200000
 */

#include "Gold.h"

Gold::Gold(std::string id, std::string first, std::string last,
           float spent, int disc) : Customer(id,first, last, spent)
{
    discount = disc;
}

