#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_lnormOffset.h"
#include "LognormalWithOffsetDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_lnormOffset::Dist_lnormOffset() : ContinuousDistribution()
{
    
}



Dist_lnormOffset* Dist_lnormOffset::clone( void ) const
{
    return new Dist_lnormOffset(*this);
}


RevBayesCore::LognormalWithOffsetDistribution* Dist_lnormOffset::createDistribution( void ) const
{
    // get the parameters
    RevBayesCore::TypedDagNode<double>* m   = static_cast<const Real &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* s   = static_cast<const RealPos &>( sd->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* o   = static_cast<const Real &>( offset->getRevObject() ).getDagNode();
    RevBayesCore::LognormalWithOffsetDistribution* d  = new RevBayesCore::LognormalWithOffsetDistribution(m, s, o);
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_lnormOffset::getClassType(void) {
    
    static std::string revType = "Dist_lnormOffset";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_lnormOffset::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( ContinuousDistribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_lnormOffset::getParameterRules(void) const {
    
    static MemberRules distLnormMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distLnormMemberRules.push_back( new ArgumentRule( "mean",   Real::getClassTypeSpec()   , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distLnormMemberRules.push_back( new ArgumentRule( "sd"  ,   RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        distLnormMemberRules.push_back( new ArgumentRule( "offset", Real::getClassTypeSpec()   , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distLnormMemberRules;
}


const TypeSpec& Dist_lnormOffset::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_lnormOffset::printValue(std::ostream& o) const {
    
    o << "lognormal(mean=";
    if ( mean != NULL ) {
        o << mean->getName();
    } else {
        o << "?";
    }
    o << ", sd=";
    if ( sd != NULL ) {
        o << sd->getName();
    } else {
        o << "?";
    }
    o << ", offset=";
    if ( offset != NULL ) {
        o << offset->getName();
    } else {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Dist_lnormOffset::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "mean" ) 
    {
        mean = var;
    }
    else if ( name == "sd" ) 
    {
        sd = var;
    }
    else if ( name == "offset" )
    {
        offset = var;
    }
    else 
    {
        ContinuousDistribution::setConstParameter(name, var);
    }
}
