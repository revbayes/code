/**
 * @file
 * This file contains the implementation of some of the functions
 * in the abstract base class for internal language objects, RbInternal.
 *
 * @brief Partial implementation of RbInternal
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-02, version 1.0
 * @extends RbInternal
 *
 * $Id$
 */

#include "RbUtil.h"
#include "RbInternal.h"
#include "VectorString.h"

#include <sstream>


/** Brief info about object: use static class attribute */
std::string RbInternal::briefInfo(void) const {
    
    std::ostringstream o;
    o << "Object of type ";
    getClass().printValue(o);
    return o.str();
}

/** Get class vector describing type of object */
const VectorString& RbInternal::getClass(void) const { 

    static VectorString rbClass = VectorString(RbInternal_name);
	return rbClass; 
}


/** Get type (first entry in class vector) */
const std::string& RbInternal::getType(void) const {

    return getClass()[0];
}


/** Are we of specified type? We need to check entire class vector in case we are derived from type. */
bool RbInternal::isType(const std::string& type) const {

    const VectorString& classVec = getClass();

    for (size_t i=0; i<classVec.size(); i++) {
        if (type == classVec[i])
            return true;
    }

	return false;
}


/** Print info about object simply by using richInfo function */
void RbInternal::printValue(std::ostream& o) const {
    
    o << richInfo() << std::endl;
}


/** Complete info about object */
std::string RbInternal::richInfo(void) const {
    
    std::ostringstream o;
    o <<  "RbInternal:\t" << getType() << std::endl;
//    printValue(o);
    
    return o.str();
}





