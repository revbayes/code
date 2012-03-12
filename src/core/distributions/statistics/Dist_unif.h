/**
 * @file
 * This file contains the declaration of Dist_unif, which is used to hold
 * parameters and functions related to a uniform distribution.
 *
 * @brief Declaration of Dist_unif
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef Dist_unif_H
#define Dist_unif_H

#include "DistributionContinuous.h"
#include "Real.h"

#include <ostream>
#include <string>

class Dist_unif: public DistributionContinuous {

    public:
                                    Dist_unif(void);                                                    //!< constructor

        // Basic utility functions
        Dist_unif*                  clone(void) const;                                                  //!< Clone object
        static const std::string&   getClassName(void);                                                 //!< Get class name
        static const TypeSpec&      getClassTypeSpec(void);                                             //!< Get class type spec
        const TypeSpec&             getTypeSpec(void) const;                                            //!< Get language type of the object

        // Member variable setup
        const MemberRules&          getMemberRules(void) const;                                         //!< Get member variable rules
        void                        setMemberVariable(const std::string& name, const Variable* var);    //!< Catching the setting of the member variables.

        // Real-valued distribution functions
        double                      cdf(const RbLanguageObject& value);                                 //!< Cumulative density
        const Real&                 getMax(void) const;                                                 //!< Get max value
        const Real&                 getMin(void) const;                                                 //!< Get min value
        const TypeSpec&             getVariableType(void) const;                                        //!< Get random variable type (Real)
        double                      lnPdf(const RbLanguageObject& value) const;                         //!< Ln probability density
        double                      pdf(const RbLanguageObject& value) const;                           //!< Probability density
        const Real&                 quantile(const double p);                                           //!< Quantile
        const RbLanguageObject&     rv(void);                                                           //!< Generate random variable
    
    private:
        static const TypeSpec       typeSpec;
        static const TypeSpec       varTypeSpec;

        // parameters
        RbConstVariablePtr          min;
        RbConstVariablePtr          max;
    
        // memberfunction return valus
        Real                        randomVariable;

};

#endif

