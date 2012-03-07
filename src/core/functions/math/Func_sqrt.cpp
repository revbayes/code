/**
 * @file
 * This file contains the implementation of Func_sqrt, which 
 * calculates the square root of a number.
 *
 * @brief Implementation of Func_sqrt
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id$
 */

#include "ConstantNode.h"
#include "DAGNode.h"
#include "DeterministicNode.h"
#include "Func_sqrt.h"
#include "RealPos.h"
#include "Real.h"
#include "RbException.h"
#include "RbUtil.h"
#include "StochasticNode.h"
#include "TypeSpec.h"
#include "ValueRule.h"
#include "VectorString.h"

#include <cassert>
#include <cmath>


/** Clone object */
Func_sqrt* Func_sqrt::clone( void ) const {

    return new Func_sqrt( *this );
}


/** Execute function */
const RbLanguageObject& Func_sqrt::executeFunction( void ) {
    
    const double d = static_cast<const RealPos&>( x->getValue() ).getValue();

    value.setValue( sqrt( d ) );
    
    return value;
}


/** Get argument rules */
const ArgumentRules& Func_sqrt::getArgumentRules( void ) const {

    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;

    if (!rulesSet) 
		{
            argumentRules.push_back( new ValueRule( "x", RealPos::getClassTypeSpec() ) );
        rulesSet = true;
		}

    return argumentRules;
}


/** Get class name of object */
const std::string& Func_sqrt::getClassName(void) { 
    
    static std::string rbClassName = "Square root function";
    
	return rbClassName; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_sqrt::getClassTypeSpec(void) { 
    
    static TypeSpec rbClass = TypeSpec( getClassName(), new TypeSpec( RbFunction::getClassTypeSpec() ) );
    
	return rbClass; 
}

/** Get type spec */
const TypeSpec& Func_sqrt::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_sqrt::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RealPos::getClassTypeSpec();
    return returnTypeSpec;
}


/** We catch here the setting of the argument variables to store our parameters. */
void Func_sqrt::setArgumentVariable(std::string const &name, const Variable* var) {
    
    if ( name == "x" ) {
        x = var;
    }
    else {
        RbFunction::setArgumentVariable(name, var);
    }
}

