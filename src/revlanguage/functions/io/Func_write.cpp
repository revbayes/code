/**
 * @file
 * This file contains the implementation of Func_readAlignment.
 *
 * @brief Implementation of Func_readAlingment
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-08-22 11:45:25 +0200 (Wed, 22 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @package functions
 * @since Version 1.0, 2009-09-03
 *
 * $Id: Func_readDiscreteCharacterData.cpp 1765 2012-08-22 09:45:25Z hoehna $
 */


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "Func_write.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlString.h"

#include <string>



using namespace RevLanguage;

/** Clone object */
Func_write* Func_write::clone( void ) const
{
    
    return new Func_write( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_write::execute( void )
{
    
    // get the information from the arguments for reading the file
    const std::string& fn = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    bool  append = static_cast<const RlBoolean&>( args[2].getVariable()->getRevObject() ).getValue();
    const std::string& separator = static_cast<const RlString&>( args[3].getVariable()->getRevObject() ).getValue();
    
    if ( fn != "" ) 
    {
        std::ofstream outStream;
        
        if ( append ) 
        {
            
            // open the stream to the file
            outStream.open(fn.c_str(), std::fstream::out | std::fstream::app);
        }
        else 
        {
            
            // open the stream to the file
            outStream.open(fn.c_str(), std::fstream::out);
        }
        
        // print the arguments
        args[0].getVariable()->getRevObject().printValue(outStream, false);
        for (size_t i = 4; i < args.size(); i++) 
        {
            outStream << separator;
            args[i].getVariable()->getRevObject().printValue( outStream , false );
        }
        
        outStream.close();
    }
    else
    {
        
        std::ostream& o = std::cout;
        
        // print the arguments
        args[0].getVariable()->getRevObject().printValue( o, false );
        for (size_t i = 4; i < args.size(); i++) 
        {
            o << separator;
            args[i].getVariable()->getRevObject().printValue( o, false );
        }
        o << std::endl;
    }

    
    return NULL;
}




/** Get argument rules */
const ArgumentRules& Func_write::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet) 
    {
        
        argumentRules.push_back( new ArgumentRule( "", RevObject::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        argumentRules.push_back( new Ellipsis( RevObject::getClassTypeSpec() ) );
        argumentRules.push_back( new ArgumentRule( "filename" , RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("") ) );
        argumentRules.push_back( new ArgumentRule( "append"   , RlBoolean::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(false) ) );
        argumentRules.push_back( new ArgumentRule( "separator", RlString::getClassTypeSpec() , ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("\t") ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_write::getClassType(void) { 
    
    static std::string revType = "Func_write";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Func_write::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}

/** Get type spec */
const TypeSpec& Func_write::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_write::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = RevNullObject::getClassTypeSpec();
    return returnTypeSpec;
}




