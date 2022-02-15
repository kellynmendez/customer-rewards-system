/** Gold Customer class
 */

#include "Gold.h"

Gold::Gold(std::string id, std::string first, std::string last,
           float spent, int disc) : Customer(id,first, last, spent)
{
    discount = disc;
}

