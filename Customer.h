/** Customer class header file
 */

#ifndef CUSTOMER_H

#include <string>

class Customer
{
private:
    std::string firstName;
    std::string lastName;
    std::string guestID;
    float amtSpent;

public:
    Customer();
    Customer(std::string id, std::string first, std::string last, float spent);
    virtual ~Customer() {}

    std::string getFirstName() const {return firstName;};
    void setFirstName(std::string& first) {firstName = first;}
    std::string getLastName() const {return lastName;}
    void setLastName(std::string& last) {lastName = last;}
    std::string getGuestID() const {return guestID;}
    void setGuestID(std::string& id) {guestID = id;}
    float getAmountSpent() const {return amtSpent;}
    void setAmountSpent(float newAmt) {amtSpent = newAmt;}

};

#define CUSTOMER_H

#endif //CUSTOMER_H
