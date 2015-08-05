#ifndef Func_simTree_H
#define Func_simTree_H

#include "Procedure.h"
#include "TimeTree.h"
#include "TopologyNode.h"

namespace RevLanguage {
    
    /**
     * @brief Rev function to simumlate a balanced or catapilar tree.
     *
     * This procedure simulates a simple tree.
     * The tree is either totally balanced, or a caterpillar tree.
     * The internode distance is fix to 1.0.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2015-02-27
     *
     */
    class Func_simTree : public Procedure {
        
    public:
        Func_simTree( void );
        
        // Basic utility functions
        Func_simTree*                               clone(void) const;                                                          //!< Clone object
        static const std::string&                   getClassType(void);                                                         //!< Get Rev simTree
        static const TypeSpec&                      getClassTypeSpec(void);                                                     //!< Get class simTree spec
        const TypeSpec&                             getTypeSpec(void) const;                                                    //!< Get language simTree of the object
        
        // Func_source functions
        const ArgumentRules&                        getArgumentRules(void) const;                                               //!< Get argument rules
        const TypeSpec&                             getReturnType(void) const;                                                  //!< Get simTree of return val
        bool                                        throws(void) const { return false; }                                        //!< Function may throw exceptions
        
        RevPtr<RevVariable>                         execute(void);                                                              //!< Execute function
        
    private:
        
        void                                        setAges(RevBayesCore::TimeTree *t, RevBayesCore::TopologyNode &n);
        void                                        simulateBalancedTree(size_t n, std::vector<RevBayesCore::TopologyNode*> &no);
        void                                        simulateCaterpillarTree(size_t n, RevBayesCore::TopologyNode* no);
        
    };
    
}

#endif

