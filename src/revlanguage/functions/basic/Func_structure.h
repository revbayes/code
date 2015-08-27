#ifndef Func_structure_H
#define Func_structure_H

#include "Procedure.h"

namespace RevLanguage {
    
    /**
     * @brief Rev function to print the structural information of a workspace variable.
     * 
     * The structure function is used by Rev to print structural information,
     * such as children and parents as well type and value of a variable.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2014-01-21
     *
     */
    class Func_structure : public Procedure {
        
    public:
        Func_structure( void );                                                                                             //!< Default constructor
        
        // Basic utility functions
        Func_structure*                         clone(void) const;                                                          //!< Clone object
        static const std::string&               getClassType(void);                                                         //!< Get Rev type
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
        
        // Func_source functions
        const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
        bool                                    throws(void) const { return false; }                                        //!< Function may throw exceptions
        
        RevPtr<RevVariable>                        execute(void);                                                              //!< Execute function
        
    };
    
}

#endif

