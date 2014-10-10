#include "AbstractDiscreteCharacterData.h"

#include <sstream>

using namespace RevBayesCore;

std::ostream& RevBayesCore::operator<<(std::ostream& o, const AbstractDiscreteCharacterData& x) {
    
    std::stringstream s;
    
    // Generate nice header
    o << std::endl;
    s << x.getDatatype() << " character matrix with " << x.getNumberOfTaxa() << " taxa and " << x.getNumberOfCharacters() << " characters" << std::endl;
    o << s.str();
    
    for ( size_t i = 0; i < s.str().length() - 1; ++i )
        o << "=";
    o << std::endl;
    
    o << "Origination:                   " << x.getFileName() << std::endl;
    o << "Number of taxa:                " << x.getNumberOfTaxa() << std::endl;
    o << "Number of included taxa:       " << x.getNumberOfIncludedTaxa() << std::endl;
    o << "Number of characters:          " << x.getNumberOfCharacters() << std::endl;
    o << "Number of included characters: " << x.getNumberOfIncludedCharacters() << std::endl;
    o << "Datatype:                      " << x.getDatatype() << std::endl;
    o << std::endl;
    
    return o;
}
