/**
 * @file
 * This file contains the declaration of RbFunction_dexp, the
 * dnorm() function.
 *
 * @brief Declaration of RbFunction_dexp
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author RevBayes core team
 * @license GPL version 3
 * @version 1.0
 * @since Version 1.0, 2009-08-26
 *
 * $Id$
 */

#ifndef RbFunction_dexp_H
#define RbFunction_dexp_H

#include "RbFunction.h"
#include <iostream>
#include <string>
#include <vector>

class ArgumentRule;
class RbDouble;
class RbObject;

/** This is the class for the dnorm() function, which takes a single
 *  scalar real or int.
 */
class RbFunction_dexp :  public RbFunction {

    public:
									RbFunction_dexp(void);                                         //!< Default constructor, allocate workspace
                                    RbFunction_dexp(const RbFunction_dexp& s);                     //!< Copy constructor
								   ~RbFunction_dexp(void);                                         //!< Destructor, delete workspace

        RbObject*                   clone(void) const ;                                             //!< clone this object
        const StringVector&         getClass(void) const;                                           //!< Get class
        std::string                 toString(void) const;                                           //!< General info on object

        const RbObject*             executeOperation(const std::vector<DAGNode*>& arguments) const; //!< Get result
		const ArgumentRule**        getArgumentRules(void) const;                                   //!< Get the number of argument rules
        const std::string           getReturnType(void) const;                                      //!< Get return type

    protected:
        RbDouble*                   value;                                                          //!< Workspace for result
};

#endif
