/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of the TreeTrace class.
 *
 * @brief Declaration of RlTreeTrace
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-09-06, version 1.0
 * @extends RbObject
 *
 * $Id: Real.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef RlTreeTrace_H
#define RlTreeTrace_H

#include "TreeTrace.h"
#include "TypedDagNode.h"
#include "WorkspaceToCoreWrapperObject.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    template <typename treeType>
    class TreeTrace : public WorkspaceToCoreWrapperObject<RevBayesCore::TreeTrace<typename treeType::valueType> > {
        
    public:
        
        TreeTrace(void);                                                                                                                    //!< Default constructor
        TreeTrace(const RevBayesCore::TreeTrace<typename treeType::valueType>& x);                                                                                              //!< Copy constructor
        
        // Basic utility functions
        virtual TreeTrace*                          clone(void) const;                                                                      //!< Clone object
        void                                        constructInternalObject(void);                                                          //!< We construct the a new internal MCMC object.
        static const std::string&                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                          //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object
        
        // Member method inits
        virtual RevPtr<RevVariable>                 executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
    protected:
        
        virtual void                                printValue(std::ostream& o) const;                                                      //!< Print value (for user)
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);          //!< Set member variable
                
    };
    
}


#include "ArgumentRules.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "Natural.h"
#include "Probability.h"
#include "RlUtils.h"
#include "TreeSummary.h"

template <typename treeType>
RevLanguage::TreeTrace<treeType>::TreeTrace() : WorkspaceToCoreWrapperObject<RevBayesCore::TreeTrace<typename treeType::valueType> >()
{

    ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burninFraction",      Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.1)) );
    summarizeArgRules->push_back( new ArgumentRule("credibleTreeSetSize", Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95)) );
    summarizeArgRules->push_back( new ArgumentRule("minCladeProbability", Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.05)) );
    this->methods.addFunction("summarize", new MemberProcedure( RlUtils::Void, summarizeArgRules) );

}


template <typename treeType>
RevLanguage::TreeTrace<treeType>::TreeTrace(const RevBayesCore::TreeTrace<typename treeType::valueType> &m) : WorkspaceToCoreWrapperObject<RevBayesCore::TreeTrace<typename treeType::valueType> >( new RevBayesCore::TreeTrace<typename treeType::valueType>( m ) )
{

    ArgumentRules* summarizeArgRules = new ArgumentRules();
    summarizeArgRules->push_back( new ArgumentRule("burninFraction",      Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.1)) );
    summarizeArgRules->push_back( new ArgumentRule("credibleTreeSetSize", Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.95)) );
    summarizeArgRules->push_back( new ArgumentRule("minCladeProbability", Probability::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Probability(0.05)) );
    this->methods.addFunction("summarize", new MemberProcedure( RlUtils::Void, summarizeArgRules) );

}


/** Clone object */
template <typename treeType>
RevLanguage::TreeTrace<treeType>* RevLanguage::TreeTrace<treeType>::clone(void) const
{
    
	return new TreeTrace(*this);
}


template <typename treeType>
void RevLanguage::TreeTrace<treeType>::constructInternalObject( void )
{
    throw RbException("We do not support a constructor function for TreeTrace.");
}


/* Map calls to member methods */
template <typename treeType>
RevLanguage::RevPtr<RevLanguage::RevVariable> RevLanguage::TreeTrace<treeType>::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "summarize")
    {
        found = true;
        
        double f            = static_cast<const Probability &>( args[0].getVariable()->getRevObject() ).getValue();
        double treeCI       = static_cast<const Probability &>( args[1].getVariable()->getRevObject() ).getValue();
        double minCladeProb = static_cast<const Probability &>( args[2].getVariable()->getRevObject() ).getValue();
        RevBayesCore::TreeSummary<typename treeType::valueType> summary = RevBayesCore::TreeSummary<typename treeType::valueType>( *this->value );
        int b = int( floor( this->value->size()*f ) );
        summary.summarizeTrees( b );
        summary.printTreeSummary(std::cout, treeCI);
        summary.summarizeClades( b );
        summary.printCladeSummary(std::cout, minCladeProb);
        
        return NULL;
    } 
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
template <typename treeType>
const std::string& RevLanguage::TreeTrace<treeType>::getClassType(void)
{
    
//    static std::string revType = "TreeTrace<" + treeType::getClassType() + ">";
    static std::string revType = "TreeTrace__" + treeType::getClassType();
    
	return revType; 
}

/** Get class type spec describing type of object */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::TreeTrace<treeType>::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::TreeTrace<typename treeType::valueType> >::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
template <typename treeType>
const RevLanguage::MemberRules& RevLanguage::TreeTrace<treeType>::getParameterRules(void) const
{
    
    static MemberRules modelMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        rulesSet = true;
    }
    
    return modelMemberRules;
}


/** Get type spec */
template <typename treeType>
const RevLanguage::TypeSpec& RevLanguage::TreeTrace<treeType>::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
template <typename treeType>
void RevLanguage::TreeTrace<treeType>::printValue(std::ostream &o) const
{
    
    o << "TreeTrace";
}


/** Set a member variable */
template <typename treeType>
void RevLanguage::TreeTrace<treeType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "xxx") {
        
    } 
    else {
        RevObject::setConstParameter(name, var);
    }
}

#endif
