/** Gold Customer class header file
 */

#ifndef GOLD_H
#define GOLD_H
#include "Customer.h"

class Gold : public Customer
{
private:
    int discount;

public:
    Gold() {discount = 0;}
    Gold(std::string id, std::string first, std::string last, float spent, int disc);

    int getDiscount() const {return discount;}
    void setDiscount(int disc) {discount = disc;}

};

#endif //GOLD_H
