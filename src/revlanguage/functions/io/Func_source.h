#ifndef Func_source_H
#define Func_source_H

#include "Procedure.h"

namespace RevLanguage {

class Func_source : public Procedure {
    
public:
    Func_source( void );
    
    // Basic utility functions
    Func_source*                            clone(void) const;                                                          //!< Clone object
    static const std::string&               getClassType(void);                                                         //!< Get Rev type
    static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
    const TypeSpec&                         getTypeSpec(void) const;                                                    //!< Get language type of the object
    
    // Func_source functions
    const ArgumentRules&                    getArgumentRules(void) const;                                               //!< Get argument rules
    const TypeSpec&                         getReturnType(void) const;                                                  //!< Get type of return val
    bool                                    throws(void) const { return true; }                                         //!< Function may throw exceptions
    
    RevPtr<RevVariable>                        execute(void);                                                              //!< Execute function
    
};
    
}

#endif

