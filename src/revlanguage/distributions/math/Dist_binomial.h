#ifndef Dist_binomial_H
#define Dist_binomial_H

#include "BinomialDistribution.h"
#include "Natural.h"
#include "Probability.h"
#include "RlTypedDistribution.h"

namespace RevLanguage {
    
    /**
     * The RevLanguage wrapper of the binomial distribution.
     *
     * The RevLanguage wrapper of the binomial distribution simply
     * manages the interactions through the Rev with our core.
     * That is, the internal distribution object can be constructed and hooked up
     * in a model graph.
     * See the BinomialDistribution for more details.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (John Huelsenbeck)
     * @since 2014-08-25, version 1.0
     *
     */
    class Dist_binomial :  public TypedDistribution<Natural> {
        
    public:
                                                        Dist_binomial(void);
        virtual                                        ~Dist_binomial(void);
        
        // Basic utility functions
        Dist_binomial*                                  clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::BinomialDistribution*             createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                       p;
        RevPtr<const RevVariable>                       n;
    };
    
}

#endif
