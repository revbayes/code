#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ConstantRateBirthDeathProcess.h"
#include "Dist_bdp.h"
#include "ModelVector.h"
#include "Natural.h"
#include "Probability.h"
#include "Real.h"
#include "RealPos.h"
#include "RlClade.h"
#include "RlString.h"
#include "RlTimeTree.h"

using namespace RevLanguage;

/**
 * Default constructor.
 * 
 * The default constructor does nothing except allocating the object.
 */
Dist_bdp::Dist_bdp() : BirthDeathProcess() 
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process. 
 */
Dist_bdp* Dist_bdp::clone( void ) const 
{
    return new Dist_bdp(*this);
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
RevBayesCore::ConstantRateBirthDeathProcess* Dist_bdp::createDistribution( void ) const
{
    
    // get the parameters
    
    // the origin
    RevBayesCore::TypedDagNode<double>* o                   = NULL;
    if ( origin != NULL && origin->getRevObject() != RevNullObject::getInstance() )
    {
        o = static_cast<const RealPos &>( origin->getRevObject() ).getDagNode();
    }
    // the root age
    RevBayesCore::TypedDagNode<double>* ra                   = NULL;
    if ( rootAge != NULL && rootAge->getRevObject() != RevNullObject::getInstance() )
    {
        ra = static_cast<const RealPos &>( rootAge->getRevObject() ).getDagNode();
    }
    // speciation rate
    RevBayesCore::TypedDagNode<double>* s       = static_cast<const RealPos &>( lambda->getRevObject() ).getDagNode();
    // extinction rate
    RevBayesCore::TypedDagNode<double>* e       = static_cast<const RealPos &>( mu->getRevObject() ).getDagNode();
    // sampling probability
    RevBayesCore::TypedDagNode<double>* r       = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();
    // sampling strategy
    const std::string &strategy                 = static_cast<const RlString &>( samplingStrategy->getRevObject() ).getValue();
    // condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    // taxon names
    const std::vector<std::string> &names       = static_cast<const ModelVector<RlString> &>( taxonNames->getRevObject() ).getDagNode()->getValue();
    // clade constraints
    const std::vector<RevBayesCore::Clade> &c   = static_cast<const ModelVector<Clade> &>( constraints->getRevObject() ).getValue();
    
    std::vector<RevBayesCore::Taxon> taxa;
    for (size_t i = 0; i < names.size(); ++i)
    {
        taxa.push_back( RevBayesCore::Taxon( names[i] ) );
    }
    // create the internal distribution object
    RevBayesCore::ConstantRateBirthDeathProcess*   d = new RevBayesCore::ConstantRateBirthDeathProcess(o, ra, s, e, r, strategy, cond, taxa, c);
    
    return d;
}


/**
 * Get Rev type of object 
 *
 * \return The class' name.
 */
const std::string& Dist_bdp::getClassType( void ) 
{ 
    
    static std::string revType = "Dist_bdp";
    
	return revType; 
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Dist_bdp::getClassTypeSpec( void ) 
{ 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( BirthDeathProcess::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** 
 * Get the member rules used to create the constructor of this object.
 *
 * The member rules of the constant-rate birth-death process are:
 * (1) the speciation rate lambda which must be a positive real.
 * (2) the extinction rate mu that must be a positive real.
 * (3) all member rules specified by BirthDeathProcess.
 *
 * \return The member rules.
 */
const MemberRules& Dist_bdp::getParameterRules(void) const 
{
    
    static MemberRules distcBirthDeathMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        distcBirthDeathMemberRules.push_back( new ArgumentRule( "mu"    , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(0.0) ) );

        // add the rules from the base class
        const MemberRules &parentRules = BirthDeathProcess::getParameterRules();
        distcBirthDeathMemberRules.insert(distcBirthDeathMemberRules.end(), parentRules.begin(), parentRules.end());
        
        rulesSet = true;
    }
    
    return distcBirthDeathMemberRules;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Dist_bdp::getTypeSpec( void ) const 
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
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
void Dist_bdp::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) 
{
    
    if ( name == "lambda" ) 
    {
        lambda = var;
    }
    else if ( name == "mu" ) 
    {
        mu = var;
    }
    else {
        BirthDeathProcess::setConstParameter(name, var);
    }
    
}
