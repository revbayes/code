/**
 * @file
 * This file contains the declaration of Func_quit, which is
 * the function used to quit RevBayes.
 *
 * @brief Declaration of Func_clear
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-04-22 12:06:58 -0700 (Sun, 22 Apr 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @interface RbFunction
 * @package functions
 * @since Version 1.0, 2012-09-07
 *
 * $Id: Func_source.h 1431 2012-04-22 19:06:58Z hoehna $
 */

#ifndef Func_quit_H
#define Func_quit_H

#include "Procedure.h"

namespace RevLanguage {
    
    class Func_quit : public Procedure {
        
    public:
        Func_quit( void );
        
        // Basic utility functions
        Func_quit*                              clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_quit functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RevPtr<RevVariable>                        execute(void);                                                              //!< Execute function
        
    };
    
}

#endif

