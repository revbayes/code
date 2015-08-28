#ifndef Func_setOption_H
#define Func_setOption_H

#include "Procedure.h"

namespace RevLanguage {
    
    
    /**
     * @brief Rev function to set a user option of RevBayes.
     *
     * The setOption function lets the user set new options.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2015-01-08
     *
     */
    class Func_setOption : public Procedure {
        
    public:
        Func_setOption( void );
        
        // Basic utility functions
        Func_setOption*                         clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_source functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                         //!< Function may throw exceptions
        
        RevPtr<RevVariable>                     execute(void);                                                              //!< Execute function
        
    };
    
}

#endif

