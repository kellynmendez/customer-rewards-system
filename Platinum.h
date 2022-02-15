/** Kellyn Mendez - CS 2337.003, Project 1
 *      NetID: kpm200000
 */

#ifndef PLATINUM_H
#define PLATINUM_H
#include "Customer.h"

class Platinum : public Customer
{
private:
    int bonusBucks;

public:
    Platinum() {bonusBucks = 0;}
    Platinum(std::string id, std::string first, std::string last, float spent, int bucks);

    int getBonusBucks() const {return bonusBucks;}
    void setBonusBucks(int bucks) {bonusBucks = bucks;}

};

#endif //PLATINUM_H
