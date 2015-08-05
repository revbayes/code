#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "SoftBoundUniformNormalDistribution.h"
#include "ContinuousStochasticNode.h"
#include "Dist_softBoundUniformNormal.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"

using namespace RevLanguage;


/**
 * Default constructor.
 *
 * The default constructor does nothing except allocating the object.
 */
Dist_SoftBoundUniformNormal::Dist_SoftBoundUniformNormal() : ContinuousDistribution()
{
    
}


/**
 * Create a new internal distribution object.
 *
 * This function simply dynamically allocates a new internal distribution object that can be
 * associated with the variable. The internal distribution object is created by calling its
 * constructor and passing the distribution-parameters (other DAG nodes) as arguments of the
 * constructor. The distribution constructor takes care of the proper hook-ups.
 *
 * \return A new internal distribution object.
 */
RevBayesCore::SoftBoundUniformNormalDistribution* Dist_SoftBoundUniformNormal::createDistribution( void ) const
{
    
    // get the parameters
    RevBayesCore::TypedDagNode<double>* mi          = static_cast<const Real &>( min->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* ma          = static_cast<const Real &>( max->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* s           = static_cast<const RealPos &>( sd->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* p           = static_cast<const Probability &>( prob->getRevObject() ).getDagNode();
    RevBayesCore::SoftBoundUniformNormalDistribution*   d    = new RevBayesCore::SoftBoundUniformNormalDistribution(mi, ma, s, p);
    
    return d;
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Dist_SoftBoundUniformNormal* Dist_SoftBoundUniformNormal::clone( void ) const
{
    
    return new Dist_SoftBoundUniformNormal(*this);
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Dist_SoftBoundUniformNormal::getClassType(void)
{
    
    static std::string revType = "Dist_SoftBoundUniformNormal";
    
    return revType;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_SoftBoundUniformNormal::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( ContinuousDistribution::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the branch rate jump process are:
 * (1) the min of the uniform distribution.
 * (2) the max of the uniform distribution.
 * (3) the standard deviation of the normal.
 * (4) the probability of belonging to distribution one.
 *
 * \return The member rules.
 */
const MemberRules& Dist_SoftBoundUniformNormal::getParameterRules(void) const
{
    
    static MemberRules distNormMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        distNormMemberRules.push_back( new ArgumentRule( "min", Real::getClassTypeSpec()       , ArgumentRule::BY_CONSTANT_REFERENCE  ) );
        distNormMemberRules.push_back( new ArgumentRule( "max", Real::getClassTypeSpec()       , ArgumentRule::BY_CONSTANT_REFERENCE  ) );
        distNormMemberRules.push_back( new ArgumentRule( "sd" , RealPos::getClassTypeSpec()    , ArgumentRule::BY_CONSTANT_REFERENCE  ) );
        distNormMemberRules.push_back( new ArgumentRule( "p"  , Probability::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return distNormMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_SoftBoundUniformNormal::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
void Dist_SoftBoundUniformNormal::printValue(std::ostream& o) const {
    
    o << "unifSBN(min=";
    if ( min != NULL ) {
        o << min->getName();
    }
    else
    {
        o << "?";
    }
    o << ", max=";
    if ( max != NULL )
    {
        o << max->getName();
    }
    else
    {
        o << "?";
    }
    o << ", sd=";
    if ( sd != NULL )
    {
        o << sd->getName();
    }
    else
    {
        o << "?";
    }
    o << ", p=";
    if ( prob != NULL )
    {
        o << prob->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/**
 * Set a member variable.
 *
 * Sets a member variable with the given name and store the pointer to the variable.
 * The value of the variable might still change but this function needs to be called again if the pointer to
 * the variable changes. The current values will be used to create the distribution object.
 *
 * \param[in]    name     Name of the member variable.
 * \param[in]    var      Pointer to the variable.
 */
void Dist_SoftBoundUniformNormal::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "min" )
    {
        min = var;
    }
    else if ( name == "max" )
    {
        max = var;
    }
    else if ( name == "sd" )
    {
        sd = var;
    }
    else if ( name == "p" )
    {
        prob = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}
