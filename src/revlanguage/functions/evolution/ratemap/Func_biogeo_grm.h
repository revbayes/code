/**
 * @file
 * This file contains the declaration of the RevLanguage dec_ddd function,
 * which is used to created deterministic variable associated with the dec_ddd
 * function.
 *
 * @brief Declaration and implementation of Func_biogeo_grm
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-20 04:06:14 +0200 (Fri, 20 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: Func__add.h 1406 2012-04-20 02:06:14Z hoehna $
 */


#ifndef Func_biogeo_grm_H
#define Func_biogeo_grm_H

#include "RlFunction.h"

#include <map>
#include <string>

namespace RevLanguage {
    
    class Func_biogeo_grm :  public Function {
        
    public:
        Func_biogeo_grm( void );
        
        // Basic utility functions
        Func_biogeo_grm*                                clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassName(void);                                                             //!< Get class name
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        
        // Function functions you have to override
        RevObject*                                      execute(void);                                                                  //!< Execute function
        const ArgumentRules&                            getArgumentRules(void) const;                                                   //!< Get argument rules
        const TypeSpec&                                 getReturnType(void) const;                                                      //!< Get type of return value
        
    };
    
}

#endif
