/**
 * @file
 * This file contains the declaration of Func_tiprobs, which is used to construct
 * a transition probability matrix
 * 
 * @brief Declaration of Func_tiprobs
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id$
 */

#ifndef Func_tiprobs_H
#define Func_tiprobs_H

#include "RbFunction.h"

class DAGNode;
class VectorString;



const std::string Func_tiprobs_name = "Transition probability function";

class Func_tiprobs :  public RbFunction {
    
    public:
        // Basic utility functions
        Func_tiprobs*               clone(void) const;                                                       //!< Clone the object
        const VectorString&         getClass(void) const;                                                    //!< Get class vector
        const TypeSpec&             getTypeSpec(void) const;                                                //!< Get language type of the object
        
        // Regular functions
        RbPtr<RbObject>             execute(void);                                                           //!< Execute function
        RbPtr<const ArgumentRules>  getArgumentRules(void) const;                                            //!< Get argument rules
        const TypeSpec&             getReturnType(void) const;                                               //!< Get type of return value
    
    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       returnTypeSpec;

};

#endif

