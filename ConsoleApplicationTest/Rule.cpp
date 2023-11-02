#include "Rule.h"
#include "Math.h"
#include "JsonParser.h"

#define arg(i) (*args)[i]

bool xkball::testMember(std::vector<c32string>* args,Member& tester){

    bool include = strings::include == arg(0);
    bool result = false;
    //assert arg(2) == equal
    c32string& key = arg(3);
    c32string& value = arg(4);
    if (strings::stringProperties == arg(1)) {
        if (tester.stringP->contains(key)) {
            result = value == tester.stringP->find(key)->second;
        }
    }
    else if (strings::intProperties == arg(1)) {
        if (tester.intP->contains(key)) {
            result = parseLong(value) == tester.intP->find(key)->second;
        }
    }
    else if (strings::doubleProperties == arg(1)) {
        if (tester.doubleP->contains(key)) {
            result = parseDouble(value) == tester.doubleP->find(key)->second;
        }

    }
    else if (strings::boolProperties == arg(1)) {
        if (tester.boolP->contains(key)) {
            result = (value == strings::true_) == tester.stringP->find(key)->second;
        }
    }
    else if (strings::enumProperties == arg(1)) {
        if (tester.enumP->contains(key)) {
            std::unique_ptr<JsonArray> p = std::unique_ptr<JsonArray>(jsonParser.parser(value)->getAsJsonArray());
            auto it = tester.enumP->equal_range(key);
            for (; it.first != it.second; ++it.first) {
                for (int i = 0; i < p.get()->size(); i++) {
                    result |= *(*p.get())[i]->getAsJsonString()->getValue() == it.first->second;
                }
            }
        }
    }
    else throw arg(1);
    
    return include ? result : !result;
}
