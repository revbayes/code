#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Dist_lnorm.h"
#include "LognormalDistribution.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;

Dist_lnorm::Dist_lnorm() : PositiveContinuousDistribution() {
    
}


Dist_lnorm* Dist_lnorm::clone( void ) const {
    return new Dist_lnorm(*this);
}


RevBayesCore::LognormalDistribution* Dist_lnorm::createDistribution( void ) const
{

    // get the parameters
    RevBayesCore::TypedDagNode<double>* m   = static_cast<const Real &>( mean->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* s   = static_cast<const RealPos &>( sd->getRevObject() ).getDagNode();
    RevBayesCore::LognormalDistribution* d  = new RevBayesCore::LognormalDistribution(m, s);
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_lnorm::getClassType(void) { 
    
    static std::string revType = "Dist_lnorm";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Dist_lnorm::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( PositiveContinuousDistribution::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}




/** Return member rules (no members) */
const MemberRules& Dist_lnorm::getParameterRules(void) const {
    
    static MemberRules distLnormMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        distLnormMemberRules.push_back( new ArgumentRule( "mean",   Real::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE,    ArgumentRule::ANY ) );
        distLnormMemberRules.push_back( new ArgumentRule( "sd"  ,   RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return distLnormMemberRules;
}


const TypeSpec& Dist_lnorm::getTypeSpec( void ) const {
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_lnorm::printValue(std::ostream& o) const {
    
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
    o << ")";
}


/** Set a member variable */
void Dist_lnorm::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "mean" ) 
    {
        mean = var;
    }
    else if ( name == "sd" ) 
    {
        sd = var;
    }
    else 
    {
        PositiveContinuousDistribution::setConstParameter(name, var);
    }
}
