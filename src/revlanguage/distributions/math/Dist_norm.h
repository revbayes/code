#ifndef Dist_normal_H
#define Dist_normal_H

#include "NormalDistribution.h"
#include "RlContinuousDistribution.h"

namespace RevLanguage {
    
    
    /**
     * The RevLanguage wrapper of the normal distribution.
     *
     * The RevLanguage wrapper of the normal distribution takes care of create the internal distribution object
     * and provides the RevLanguage object that can be used within Rev.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2012-08-06, version 1.0
     *
     */
    class Dist_norm :  public ContinuousDistribution {
    
        public:
        Dist_norm( void );
    
        // Basic utility functions
        Dist_norm*                                      clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')

    
        // Distribution functions you have to override
        RevBayesCore::NormalDistribution*               createDistribution(void) const;
    
        protected:
    
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable

    
        private:
        
        RevPtr<const RevVariable>                          mean;
        RevPtr<const RevVariable>                          sd;
    
    };
    
}

#endif
