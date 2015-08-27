/**
 * @file
 * This file contains the declaration and implementation
 * of the templated Func__div, which is used to div two
 * variables.
 *
 * @brief Declaration and implementation of Func__div
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__div.h 1406 2012-04-20 02:06:14Z hoehna $
 */

#ifndef Func_div_H
#define Func_div_H

#include "RlTypedFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    template <typename firstValType, typename secondValType, typename retType>
    class Func__div : public TypedFunction<retType> {
        
    public:
        Func__div( void );
        
        // Basic utility functions
        Func__div*                                                      clone(void) const;                              //!< Clone the object
        static const std::string&                                       getClassType(void);                             //!< Get Rev type
        static const TypeSpec&                                          getClassTypeSpec(void);                         //!< Get class type spec
        const TypeSpec&                                                 getTypeSpec(void) const;                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevBayesCore::TypedFunction<typename retType::valueType>*       createFunction(void) const ;                    //!< Create a new internal function object
        const ArgumentRules&                                            getArgumentRules(void) const;                   //!< Get argument rules
        
    };
    
}

#include "BinaryDivision.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

/** default constructor */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__div<firstValType, secondValType, retType>::Func__div( void ) : TypedFunction<retType>( ) {
    
}


/** Clone object */
template <typename firstValType, typename secondValType, typename retType>
RevLanguage::Func__div<firstValType, secondValType, retType>* RevLanguage::Func__div<firstValType, secondValType, retType>::clone( void ) const {
    
    return new Func__div( *this );
}


template <typename firstValType, typename secondValType, typename retType>
RevBayesCore::TypedFunction< typename retType::valueType>* RevLanguage::Func__div<firstValType, secondValType, retType>::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<typename firstValType::valueType>* firstArg = static_cast<const firstValType &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<typename secondValType::valueType>* secondArg = static_cast<const secondValType &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::BinaryDivision<typename firstValType::valueType, typename secondValType::valueType, typename retType::valueType> *func = new RevBayesCore::BinaryDivision<typename firstValType::valueType, typename secondValType::valueType, typename retType::valueType>(firstArg, secondArg);
    
    return func;
}


/* Get argument rules */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::ArgumentRules& RevLanguage::Func__div<firstValType, secondValType, retType>::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "first" , firstValType::getClassTypeSpec() , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "second", secondValType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


template <typename firstValType, typename secondValType, typename retType>
const std::string& RevLanguage::Func__div<firstValType, secondValType, retType>::getClassType(void) { 
    
    static std::string revType = "Func__div<" + firstValType::getClassType() + "," + secondValType::getClassType() + "," + retType::getClassType() + ">";
    
	return revType; 
}

/* Get class type spec describing type of object */
template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__div<firstValType, secondValType, retType>::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


template <typename firstValType, typename secondValType, typename retType>
const RevLanguage::TypeSpec& RevLanguage::Func__div<firstValType, secondValType, retType>::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

#endif
