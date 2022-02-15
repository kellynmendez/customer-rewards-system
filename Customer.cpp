/** Kellyn Mendez - CS 2337.003, Project 1
 *      NetID: kpm200000
 */

#include "Customer.h"

Customer::Customer()
{
    firstName = "";
    lastName = "";
    guestID = "";
    amtSpent = 0;
}

Customer::Customer(std::string id, std::string first, std::string last, float spent)
{
    guestID = id;
    firstName = first;
    lastName = last;
    amtSpent = spent;
}