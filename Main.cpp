/** This project reads in customer information from a text file and tracks customer purchases and 
 *  the rewards applied to each customer’s account
 */

#include "Customer.h"
#include "Gold.h"
#include "Platinum.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>
using namespace std;

/********************** GLOBAL CONSTANTS **********************/
// conversion from percent to decimal
const int PERC_TO_DEC = 100;
const int PRECISION = 2;
// price per ounce for drink types
const float SODA_PRICE = 0.20f;
const float TEA_PRICE = 0.12f;
const float PUNCH_PRICE = 0.15f;
// pi * diameter * height = surface area of given drink size
const float SMALL_SA = 4.0f * 4.5f * static_cast<float>(M_PI);
const float MED_SA = 4.5f * 5.75f * static_cast<float>(M_PI);
const float LARGE_SA = 5.5f * 7.0f * static_cast<float>(M_PI);
// number of ounces in drink size
const int SMALL_OZ = 12;
const int MED_OZ = 20;
const int LARGE_OZ = 32;
// promotion boundaries
const float REG_TO_GOLD = 50;
const float GOLD_1_TO_2 = 100;
const float GOLD_2_TO_3 = 150;
const float GOLD_TO_PLAT = 200;
// gold discounts
const int GOLD_DISC_1 = 5;
const int GOLD_DISC_2 = 10;
const int GOLD_DISC_3 = 15;
// bonus bucks
const int FIND_BUCKS = 5;

/******************** FUNCTION DECLARATIONS ********************/
float calcAmtSpent(char, const string&, float, int);
void discountsAndPromotions(Customer*, float, Customer**&, int&, Customer**&, int&);
Gold* promoteToGold(Customer*, Customer**&, int&, Customer**&, int&);
void promoteToPlat(Customer*, Customer**&, int&);

int main()
{
    // Prompting user for names of the regular customer file, the preferred customer file,
    //   and the orders file
    string regFileName, prefFileName, ordersFileName;
    cout << "Enter regular customer file name: ";
    cin >> regFileName;
    cout << "Enter preferred customer file name: ";
    cin >> prefFileName;
    cout << "Enter orders file name: ";
    cin >> ordersFileName;

    // Declaring the regular array and the preferred array and their counters
    Customer **regular;
    Customer **preferred;
    int regSize = 0;
    int prefSize = 0;

    // Declaring file and stream objects
    ifstream infile;
    istringstream stream;


    // OPENING REGULAR CUSTOMERS FILE
    infile.open(regFileName);

    // Variables for storing customer data read from the file
    string line, id, first, last, temp;
    float spent;

    /* While there is still a line in the file to be read and the line is not empty
     */
    while (getline(infile, line))
    {
        // Creating new array with size plus one
        Customer **newReg = new Customer*[regSize + 1];

        // clearing string stream and reassigning stream to new line
        stream.clear();
        stream.str(line);

        // storing information from file into customer data variables
        stream >> id;
        stream >> first;
        stream >> last;
        stream >> temp;
        spent = stof(temp);

        // Creating customer from given information
        Customer *toAdd = new Customer(id, first, last, spent);

        // If array size is not equal to zero
        if(regSize != 0)
        {
            // For every index of old array, copies over customer to new array
            for (int index = 0; index < regSize; index++)
            {
                newReg[index] = regular[index];
            }
            // Adds latest customer to new array
            newReg[regSize] = toAdd;
            // Deletes old regular array
            delete [] regular;
        }
        // If array size equals zero
        else
        {
            // Adds latest customer to new array
            newReg[0] = toAdd;
        }

        // Assigns the regular array to be the new regular array
        regular = newReg;
        // Clears the new array variable (so no duplicates)
        newReg = nullptr;
        // Increment the regular size array counter now that customer has been added
        regSize++;
    }

    // CLOSING REGULAR CUSTOMERS FILE
    infile.close();



    // OPENING PREFERRED CUSTOMER FILE
    infile.open(prefFileName);

    // To check in the future if preferred array exists
    bool prefExists = false;
    // If file exists and is open
    if (infile.is_open())
    {
        prefExists = true;

        // Variables for storing customer data read from the file
        string rewardCheck;

        /* While there is still a line in the file to be read and the line is not empty
         */
        while (getline(infile, line) && !line.empty())
        {
            // Creating preferred array
            Customer **newPref = new Customer*[prefSize + 1];

            // Clearing string stream and reassigning stream to new line
            stream.clear();
            stream.str(line);

            // Storing information from file into customer data variables
            stream >> id;
            stream >> first;
            stream >> last;
            stream >> temp;
            spent = stof(temp);
            stream >> rewardCheck;

            // If customer's reward contains a %, the customer is gold
            if (rewardCheck.find('%') != string::npos)
            {
                // Removing % from number
                rewardCheck = rewardCheck.substr(0, rewardCheck.size() - 1);
                // Parsing integer from string
                int disc;
                istringstream(rewardCheck) >> disc;

                // Creating Gold customer and adding to array
                Gold *toAdd = new Gold(id, first, last, spent, disc);

                // If the size of the array is not 0
                if(prefSize != 0)
                {
                    // For every index of old array, copies over customer to new array
                    for (int index = 0; index < prefSize; index++)
                    {
                        newPref[index] = preferred[index];
                    }
                    // Adds latest customer to new array
                    newPref[prefSize] = toAdd;
                    // Deletes old regular array
                    delete [] preferred;
                }
                // If the array size is zero
                else
                {
                    // Adds latest customer to new array
                    newPref[0] = toAdd;
                }

                // Assigns the preferred array to be the new preferred array
                preferred = newPref;
                // Clears the new array variable (so no duplicates)
                newPref = nullptr;
            }
            // If the customer's reward does not contain a %, the customer is platinum
            else
            {
                // Parsing integer from string
                int bucks;
                istringstream(rewardCheck) >> bucks;

                // Creating Platinum customer and adding to array
                Platinum *toAdd = new Platinum(id, first, last, spent, bucks);

                // If array size is not equal to zero
                if(prefSize != 0)
                {
                    // For every index of old array, copies over customer to new array
                    for (int index = 0; index < prefSize; index++)
                    {
                        newPref[index] = preferred[index];
                    }
                    // Adds latest customer to new array
                    newPref[prefSize] = toAdd;
                    // Deletes old regular array
                    delete [] preferred;
                }
                // If array size equals zero
                else
                {
                    // Adds latest customer to new array
                    newPref[0] = toAdd;
                }

                // Assigns the preferred array to be the new preferred array
                preferred = newPref;
                // Clears the new array variable (so no duplicates)
                newPref = nullptr;
            }
            // Incrementing preferred array's size now that customer is added
            prefSize++;
        }

        // CLOSING PREFERRED CUSTOMER FILE
        infile.close();
    }


    // OPENING ORDERS FILE
    infile.open(ordersFileName);

    // Variables for storing customer data read from the file
    char size;
    string drinkType;
    float inchPrice;
    int quantity;

    /* While there is still a line in the file to be read and the line is not empty
     */
    while (getline(infile, line) && !line.empty())
    {
        // Clearing string stream and reassigning stream to new line
        stream.clear();
        stream.str(line);

        // Validating then storing information from file into order data variables if valid
        bool valid = true;

        // Reading ID from stream
        stream >> id;

        // Validating the ID
        bool found = false;
        Customer *toProcess;
        // Searching regular customers array for customer with given id
        for(int index = 0; index < regSize && !found; index++)
        {
            // If customer at index is the customer
            if (regular[index]->getGuestID() == id)
            {
                // Store the customer in toProcess to use for processing order later
                found = true;
                toProcess = regular[index];
            }
        }
        // If the preferred array exists
        if(prefExists)
        {
            // If not found in the regular customers array
            if (!found)
            {
                // Searching the preferred array for customer
                for(int index = 0; index < prefSize && !found; index++)
                {
                    // If customer at index is the customer
                    if (preferred[index]->getGuestID() == id)
                    {
                        // Store the customer in toProcess to use for processing order later
                        found = true;
                        toProcess = preferred[index];
                    }
                }
            }
        }
        // If the customer was never found, then the id is not valid
        valid = found;


        // If still more info to read and input has been valid so far, validate drink size
        if (valid)
        {
            // If there is no more information to read from stream, line is not valid
            if (stream.eof())
            {
                valid = false;
            }
            else
            {
                stream >> temp;
                // If the string size is greater than 1, then not a valid size
                if (temp.size() > 1)
                {
                    valid = false;
                }
                else
                {
                    size = temp[0];
                    // If the size is not small, medium, or large, then not a valid size
                    if (!(size == 'S' || size == 'M' || size == 'L'))
                    {
                        valid = false;
                    }
                }

            }
        }

        // If still more info to read and input has been valid so far, validate drink type
        if (valid)
        {
            // If there is no more information to read from stream, line is not valid
            if (stream.eof())
            {
                valid = false;
            }
            else
            {
                stream >> drinkType;
                // If drink type is not soda, tea, or punch, then not valid type
                if (!(drinkType == "soda" || drinkType == "tea" || drinkType == "punch"))
                {
                    valid = false;
                }
            }
        }

        // If still more info to read and input has been valid so far, validate square inch price
        if (valid)
        {
            // If there is no more information to read from stream, line is not valid
            if (stream.eof())
            {
                valid = false;
            }
            else
            {
                stream >> temp;
                // For every index of string, check that the character is a digit or is '.'
                for (int index = 0; index < static_cast<int>(temp.size()) && valid; index++)
                {
                    if (!(isdigit(temp[index]) || temp[index] == '.'))
                    {
                        valid = false;
                    }
                }
                // If all characters are digits or a decimal point, then the price is valid
                if (valid)
                {
                    inchPrice = stof(temp);
                }
            }
        }

        // If still more info to read and input has been valid so far, validate quantity
        if (valid)
        {
            // If there is no more information to read from stream, line is not valid
            if (stream.eof())
            {
                valid = false;
            }
            else
            {
                stream >> temp;
                // For every index of string, check that the character is a digit
                for (int index = 0; index < static_cast<int>(temp.size()) && valid; index++)
                {
                    valid = isdigit(temp[index]);
                }
                // If every character is a digit, then the quantity is valid
                if (valid)
                {
                    quantity = stoi(temp);
                }
            }
        }

        // If there is more to be read from the stream, then line is not valid because there are too many fields
        if (!stream.eof())
        {
            valid = false;
        }

        // If all information was valid, then the order is processed
        if (valid)
        {
            // Calculates amount spent in order
            float amtSpent = calcAmtSpent(size, drinkType, inchPrice, quantity);

            // Adding the amount spent in order to the total amount the customer has spent
            toProcess->setAmountSpent(toProcess->getAmountSpent() + amtSpent);

            // Applying appropriate discounts and makes any promotions necessary
            discountsAndPromotions(toProcess, amtSpent, regular, regSize, preferred, prefSize);

        }
    }

    // CLOSING ORDERS FILE
    infile.close();

    // Writing the regular array to an output file
    ofstream outfile("customer.dat");
    for (int index = 0; index < regSize; index++)
    {
        outfile << regular[index]->getGuestID() << " ";
        outfile << regular[index]->getFirstName() << " ";
        outfile << regular[index]->getLastName() << " ";
        // Formatting the amount spent
        outfile << fixed << setprecision(static_cast<int>(PRECISION)) << showpoint << regular[index]->getAmountSpent();
        outfile << endl;
    }
    outfile.close();

    // Writing the preferred array to an output file
    ofstream outfile2("preferred.dat");
    for (int index = 0; index < prefSize; index++)
    {
        outfile2 << preferred[index]->getGuestID() << " ";
        outfile2 << preferred[index]->getFirstName() << " ";
        outfile2 << preferred[index]->getLastName() << " ";
        // Formatting the amount spent
        outfile2 << fixed << setprecision(static_cast<int>(PRECISION)) << showpoint << preferred[index]->getAmountSpent() << " ";

        Customer *goldCheck = dynamic_cast<Gold*>(preferred[index]);
        // If preferred customer is a gold object, writes discount number to file formatted with a "%"
        if (goldCheck)
        {
            Gold *write = dynamic_cast<Gold*>(preferred[index]);
            outfile2 << write->getDiscount() << "%";
        }
            // If preferred customer is a platinum object, writes number of bonus bucks to file
        else
        {
            Platinum *write = dynamic_cast<Platinum*>(preferred[index]);
            outfile2 << write->getBonusBucks();
        }
        outfile2 << endl;
    }
    outfile2.close();


    return 0;
}

// Calculates the amount spent with the given order values
float calcAmtSpent(char size, const string& drink, float inchPrice, int quantity)
{
    float amtSpent;

    // Calculating the amount spent based on the drink type and size
    // If the drink is a soda
    if (drink == "soda")
    {
        // If the drink is a small
        if (size == 'S')
        {
            amtSpent = static_cast<float>(quantity) * ((inchPrice * SMALL_SA) + (SODA_PRICE * SMALL_OZ));
        }
        // If the drink is a medium
        else if (size == 'M')
        {
            amtSpent = static_cast<float>(quantity) * ((inchPrice * MED_SA) + (SODA_PRICE * MED_OZ));
        }
        // If the drink is a large
        else
        {
            amtSpent = static_cast<float>(quantity) * ((inchPrice * LARGE_SA) + (SODA_PRICE * LARGE_OZ));
        }
    }
    // If the drink is tea
    else if (drink == "tea")
    {
        // If the drink is a small
        if (size == 'S')
        {
            amtSpent = static_cast<float>(quantity) * ((inchPrice * SMALL_SA) + (TEA_PRICE * SMALL_OZ));
        }
        // If the drink is a medium
        else if (size == 'M')
        {
            amtSpent = static_cast<float>(quantity) * ((inchPrice * MED_SA) + (TEA_PRICE * MED_OZ));
        }
        // If the drink is a large
        else
        {
            amtSpent = static_cast<float>(quantity) * ((inchPrice * LARGE_SA) + (TEA_PRICE * LARGE_OZ));
        }

    }
    // If the drink is punch
    else
    {
        // If the drink is a small
        if (size == 'S')
        {
            amtSpent = static_cast<float>(quantity) * ((inchPrice * SMALL_SA) + (PUNCH_PRICE * SMALL_OZ));
        }
        // If the drink is a medium
        else if (size == 'M')
        {
            amtSpent = static_cast<float>(quantity) * ((inchPrice * MED_SA) + (PUNCH_PRICE * MED_OZ));
        }
        // If the drink is a large
        else
        {
            amtSpent = static_cast<float>(quantity) * ((inchPrice * LARGE_SA) + (PUNCH_PRICE * LARGE_OZ));
        }
    }

    // Returning the amount spent
    return amtSpent;

}

// Applies discounts to the order for gold customers. If discounts cause any upgraded discounts for gold customers, then the discounts are upgraded.
//      If amount spent in order causes a promotion to gold or platinum, then customer is promoted.
void discountsAndPromotions(Customer *customer, float amtSpent, Customer** &regular, int& regSize, Customer** &preferred, int& prefSize)
{
    // If total amount customer has spent is greater than the regular to gold customer
    //   boundary
    if (customer->getAmountSpent() >= REG_TO_GOLD)
    {
        // Subtracting amount spent in order to apply discounts
        customer->setAmountSpent(customer->getAmountSpent() - amtSpent);

        // Checking if the customer is gold
        Customer *goldCheck = dynamic_cast<Gold *>(customer);
        // Checking if the customer is platinum
        Customer *platCheck = dynamic_cast<Platinum *>(customer);

        // If customer is gold
        if (goldCheck)
        {
            // Casting to be a gold customer
            Gold *nowGold = dynamic_cast<Gold*>(customer);

            // Applying gold discount to amount spent
            float discSpent = amtSpent * (1 - (static_cast<float>(nowGold->getDiscount()) / PERC_TO_DEC));
            nowGold->setAmountSpent(nowGold->getAmountSpent() + discSpent);

            // If customer is gold 1, check if customer can be promoted to gold 2, gold 3, or platinum
            if (nowGold->getDiscount() == GOLD_DISC_1)
            {
                // If customer has spent enough to have a 10% discount
                if (nowGold->getAmountSpent() >= GOLD_1_TO_2)
                {
                    // Subtracts old amount spent from the total spent
                    nowGold->setAmountSpent(nowGold->getAmountSpent() - discSpent);
                    // Sets discount to 10%
                    nowGold->setDiscount(GOLD_DISC_2);
                    // Calculates new amount spent in order with the new discount
                    discSpent = amtSpent * (1 - (static_cast<float>(nowGold->getDiscount()) / PERC_TO_DEC));
                    // Adds new amount spent to total amount spent
                    nowGold->setAmountSpent(nowGold->getAmountSpent() + discSpent);

                    // If customer has spent enough to have a 15% discount
                    if (nowGold->getAmountSpent() >= GOLD_2_TO_3)
                    {
                        // Subtracts old amount spent from the total spent
                        nowGold->setAmountSpent(nowGold->getAmountSpent() - discSpent);
                        // Sets discount to 15%
                        nowGold->setDiscount(GOLD_DISC_3);
                        // Calculates new amount spent in order with the new discount
                        discSpent = amtSpent * (1 - (static_cast<float>(nowGold->getDiscount()) / PERC_TO_DEC));
                        // Adds new amount spent to total amount spent
                        nowGold->setAmountSpent(nowGold->getAmountSpent() + discSpent);

                        // If customer has spent enough to be promoted to platinum, then customer is promoted
                        if (nowGold->getAmountSpent() >= GOLD_TO_PLAT)
                        {
                            promoteToPlat(nowGold, preferred, prefSize);
                        }
                    }
                }
            }
            // If customer is gold 2, check if customer can be promoted to gold 3 or platinum
            else if (nowGold->getDiscount() == GOLD_DISC_2)
            {
                // If customer has spent enough to have a 15% discount
                if (nowGold->getAmountSpent() >= GOLD_2_TO_3)
                {
                    // Subtracts old amount spent from the total spent
                    nowGold->setAmountSpent(nowGold->getAmountSpent() - discSpent);
                    // Sets discount to 15%
                    nowGold->setDiscount(GOLD_DISC_3);
                    // Calculates new amount spent in order with the new discount
                    discSpent = amtSpent * (1 - (static_cast<float>(nowGold->getDiscount()) / PERC_TO_DEC));
                    // Adds new amount spent to total amount spent
                    nowGold->setAmountSpent(nowGold->getAmountSpent() + discSpent);

                    // // If customer has spent enough to be promoted to platinum, then customer is promoted
                    if (nowGold->getAmountSpent() >= GOLD_TO_PLAT)
                    {
                        promoteToPlat(nowGold, preferred, prefSize);
                    }
                }
            }
            // If customer is gold 3, check if customer can be promoted to platinum
            else if (nowGold->getDiscount() == GOLD_DISC_3)
            {
                // // If customer has spent enough to be promoted to platinum, then customer is promoted
                if (nowGold->getAmountSpent() >= GOLD_TO_PLAT)
                {
                    promoteToPlat(nowGold, preferred, prefSize);
                }
            }
        }
        // If customer is platinum
        else if (platCheck)
        {
            // Casting to be platinum customer
            Platinum *nowPlat = dynamic_cast<Platinum *>(customer);

            // Subtract all bonus bucks from amount being spent in the order
            float discSpent = amtSpent - static_cast<float>(nowPlat->getBonusBucks());
            // Find new amount of bonus bucks that can be applied and set customer's available bonus bucks equal to it
            int checkBucks = static_cast<int>(nowPlat->getAmountSpent());
            int newBonus = 0;
            // If the amount spent is already a multiple of five, do not count in new bonus bucks counter
            if (checkBucks % FIND_BUCKS == 0)
                checkBucks++;
            // Checking every number in the range from old total amount spent to new total amount spent (the old amount plus the
            //    amount spent in the order)
            while (checkBucks <= static_cast<int>(nowPlat->getAmountSpent() + discSpent))
            {
                // If number is a multiple of five, add one bonus buck
                if (checkBucks % FIND_BUCKS == 0)
                    newBonus++;
                checkBucks += 1;
            }
            // Resetting number of bonus bucks
            nowPlat->setBonusBucks(newBonus);
            // Adding the new amount spent to the total spent
            nowPlat->setAmountSpent(nowPlat->getAmountSpent() + discSpent);
        }
        // If neither gold nor platinum, but amount spent is now more than the regular to gold
        //   boundary, then causes a promotion from regular to gold
        else
        {
            // Applying gold discount to the amt spent in the order
            float discSpent = amtSpent * (1 - (static_cast<float>(GOLD_DISC_1) / PERC_TO_DEC));
            // Adds the amount spent in order to the total amount spent
            customer->setAmountSpent(discSpent + customer->getAmountSpent());

            // Promotes customer to gold, and stores the new gold customer in nowGold
            Gold* nowGold = promoteToGold(customer, regular, regSize, preferred, prefSize);

            // Checking if amount spent is over the gold 1 to gold 2 boundary, upgrade discount
            if (nowGold->getAmountSpent() >= GOLD_1_TO_2)
            {
                // Subtracts old amount spent from the total spent
                nowGold->setAmountSpent(nowGold->getAmountSpent() - discSpent);
                // Resetting and applying new gold discount to the amt spent in the order
                nowGold->setDiscount(GOLD_DISC_2);
                discSpent = amtSpent * (1 - (static_cast<float>(nowGold->getDiscount()) / PERC_TO_DEC));
                // Adds the amount spent in order to the total amount spent
                nowGold->setAmountSpent(discSpent + nowGold->getAmountSpent());

                // If amount spent went over the gold 2 to gold 3 boundary, upgrade discount
                if (nowGold->getAmountSpent() >= GOLD_2_TO_3)
                {
                    // Subtracts old amount spent from the total spent
                    nowGold->setAmountSpent(nowGold->getAmountSpent() - discSpent);
                    // Resetting and applying new gold discount to the amt spent in the order
                    nowGold->setDiscount(GOLD_DISC_3);
                    discSpent = amtSpent * (1 - (static_cast<float>(nowGold->getDiscount()) / PERC_TO_DEC));
                    // Adds the amount spent in order to the total amount spent
                    nowGold->setAmountSpent(discSpent + nowGold->getAmountSpent());

                    // If amount spent went over the gold to platinum boundary, promote to platinum
                    if (nowGold->getAmountSpent() >= GOLD_TO_PLAT)
                    {
                        promoteToPlat(nowGold, preferred, prefSize);
                    }
                }
            }
        }
    }
}

// Promotes a given customer to gold by making the customer a gold object, removing customer from regular customer array,
//    and adding customer to preferred array
Gold* promoteToGold(Customer* promote, Customer** &regular, int& regSize, Customer** &preferred, int& prefSize)
{
    // Creating new preferred array and new regular array
    Customer **newPref = new Customer*[prefSize + 1];
    Customer **newReg = new Customer*[regSize - 1];

    // Copying over data from old customer object to a gold customer object
    Gold *toAdd = new Gold(promote->getGuestID(), promote->getFirstName(), promote->getLastName(),
                           promote->getAmountSpent(), GOLD_DISC_1);

    // For every index of regular array, copies over customer to new array unless it is
    //    the customer being promoted
    int newIndex = 0;
    for (int index = 0; index < regSize; index++)
    {
        // If customer is found
        if (regular[index] != promote)
        {
            // Copies over customer at spot index in the old array to the new array's next available index
            newReg[newIndex] = regular[index];
            newIndex++;
        }
    }
    // Decrements size of array now that old customer is removed and array is resized
    regSize--;

    // Deletes old regular array
    delete [] regular;
    // Assigns the regular array to be the new regular array
    regular = newReg;
    // Clears the new array variable (so no duplicates)
    newReg = nullptr;

    if (prefSize != 0)
    {
        // For every index of preferred array, copies over customer at index in the old array to the new array's
        //    index
        for (int index = 0; index < prefSize; index++)
        {
            newPref[index] = preferred[index];
        }
        // Adding the new preferred customer to the end of the array
        newPref[prefSize] = toAdd;
        // Deletes old preferred array
        delete [] preferred;
    }
    else
    {
        // Adds the new preferred customer to the array
        newPref[0] = toAdd;
    }
    // Increments size of array now that customer has been added
    prefSize++;
    // Assigns the preferred array to be the new preferred array
    preferred = newPref;
    // Clears the new array variable (so no duplicates)
    newPref = nullptr;

    // Returning promoted customer object
    return toAdd;
}

// Promotes a given customer to platinum by making the customer a platinum object and calculates
//    number of bonus bucks to apply to the customer
void promoteToPlat(Customer* promote, Customer** &preferred, int& prefSize)
{
    // Finds number of bonus bucks by finding the number of multiples of five between the gold
    //    to platinum boundary and the total amount spent
    int bonusBucks = static_cast<int>(promote->getAmountSpent() - GOLD_TO_PLAT) / FIND_BUCKS;
    // Creates platinum object by copying over the information from the gold object and storing the bonus bucks
    Platinum *toAdd = new Platinum(promote->getGuestID(), promote->getFirstName(), promote->getLastName(),
                           promote->getAmountSpent(), bonusBucks);

    // For every index of regular array until customer is found
    for (int index = 0; index < prefSize; index++)
    {
        // If customer is found
        if (preferred[index] == promote)
        {
            // Remove old customer from array
            preferred[index] = toAdd;
            break;
        }
    }
}
