/**
 * @file
 * This file contains the declaration of TransitionProbabilityMatrix, which is
 * class that holds a matrix of transition.
 *
 * @brief Declaration of TransitionProbabilityMatrix
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef TransitionProbabilityMatrix_H
#define TransitionProbabilityMatrix_H

#include "ConstantMemberObject.h"

class ArgumentRule;
class DAGNode;
class MatrixReal;
class VectorReal;
class VectorString;



const std::string TransitionProbabilityMatrix_name = "Transition probability matrix";

class TransitionProbabilityMatrix : public ConstantMemberObject {

    public:
                                            TransitionProbabilityMatrix(void);                                 //!< Default constructor (never call this except from the workspace once)
                                            TransitionProbabilityMatrix(const TransitionProbabilityMatrix& m); //!< Copy constructor
                                            TransitionProbabilityMatrix(size_t n);                             //!< Construct rate matrix with n states
                                           ~TransitionProbabilityMatrix(void);                                 //!< Destructor
        RbPtr<VectorReal>                   operator[](size_t i);                                              //!< Subscript operator
        RbPtr<const VectorReal>             operator[](size_t i) const;                                        //!< Subscript operator (const)
    
        // Basic utility functions
        TransitionProbabilityMatrix*        clone(void) const;                                                 //!< Clone object
        const VectorString&                 getClass(void) const;                                              //!< Get class vector   
        const TypeSpec&                     getTypeSpec(void) const;                                           //!< Get language type of the object
        void                                printValue(std::ostream& o) const;                                 //!< Print value for user
        std::string                         richInfo(void) const;                                              //!< Complete info

        // Member variable rules
        RbPtr<const MemberRules>            getMemberRules(void) const;                                        //!< Get member rules

        // Member method inits
        RbPtr<const MethodTable>            getMethods(void) const;                                            //!< Get methods
        RbPtr<RbObject>                     executeOperation(const std::string& name, const RbPtr<Environment>& args);      //!< Map method call to internal functions

    private:
        size_t                              numStates;                                                         //!< The number of character states
        RbPtr<MatrixReal>                   theMatrix;                                                         //!< Holds the transition probability matrix
    
        static const TypeSpec               typeSpec;
};

#endif

