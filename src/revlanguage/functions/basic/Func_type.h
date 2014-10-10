#ifndef Func_type_H
#define Func_type_H

#include "RlFunction.h"

namespace RevLanguage {
    
    /**
     * @brief Rev function to print the type of a workspace variable.
     *
     * The type function is used by Rev to print type information,
     * of a variable.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-01-21
     *
     */
    class Func_type : public Function {
        
    public:
        Func_type( void );
        
        // Basic utility functions
        Func_type*                              clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_source functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RevPtr<Variable>                        execute(void);                                                              //!< Execute function
        
    };
    
}

#endif

