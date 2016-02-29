#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "Dist_CharacterDependentBirthDeathProcess.h"
#include "ModelVector.h"
#include "Natural.h"
#include "OptionRule.h"
#include "Probability.h"
#include "RateGenerator.h"
#include "Real.h"
#include "RealPos.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlDistributionMemberFunction.h"
#include "RlRateGenerator.h"
#include "RlSimplex.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RlTimeTree.h"
#include "StochasticNode.h"
#include "CharacterDependentBirthDeathProcess.h"

using namespace RevLanguage;


Dist_CharacterDependentBirthDeathProcess::Dist_CharacterDependentBirthDeathProcess() : TypedDistribution<TimeTree>()
{
    
}


Dist_CharacterDependentBirthDeathProcess::~Dist_CharacterDependentBirthDeathProcess()
{
    
}



Dist_CharacterDependentBirthDeathProcess* Dist_CharacterDependentBirthDeathProcess::clone( void ) const
{
    
    return new Dist_CharacterDependentBirthDeathProcess( *this );
}


RevBayesCore::CharacterDependentBirthDeathProcess* Dist_CharacterDependentBirthDeathProcess::createDistribution( void ) const
{
    
    // Get the parameters
    RevBayesCore::TypedDagNode<double>* ra   = static_cast<const RealPos &>( root_age->getRevObject() ).getDagNode();
//    RevBayesCore::TypedDagNode<int>*    rs   = static_cast<const Natural &>( root_state->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* hsp  = static_cast<const ModelVector<Real> &>( hidden_speciation_rates->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* osp  = static_cast<const ModelVector<Real> &>( observed_speciation_rates->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* hex  = static_cast<const ModelVector<Real> &>( hidden_extinction_rates->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* oex  = static_cast<const ModelVector<Real> &>( observed_extinction_rates->getRevObject() ).getDagNode();
    // rate matrix
    RevBayesCore::TypedDagNode<RevBayesCore::RateGenerator>* q      = static_cast<const RateGenerator &>( event_rate_matrix->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>*                      r      = static_cast<const RealPos &>( event_rate->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* bf = static_cast<const Simplex &>( root_frequencies->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* rh   = static_cast<const Probability &>( rho->getRevObject() ).getDagNode();
    std::vector<RevBayesCore::Taxon> t = static_cast<const ModelVector<Taxon> &>( taxa->getRevObject() ).getValue();
    // condition
    const std::string& cond                     = static_cast<const RlString &>( condition->getRevObject() ).getValue();
    
    
    
    RevBayesCore::CharacterDependentBirthDeathProcess*   d = new RevBayesCore::CharacterDependentBirthDeathProcess( ra, osp, hsp, oex, hex, q, r, bf, rh, cond, t );
    
    return d;
}



/* Get Rev type of object */
const std::string& Dist_CharacterDependentBirthDeathProcess::getClassType(void)
{
    
    static std::string revType = "Dist_CharacterDependentBirthDeathProcess";
    
    return revType;
}


/* Get class type spec describing type of object. TODO: Check if the correct parent is TypedDistribution or Distribution */
const TypeSpec& Dist_CharacterDependentBirthDeathProcess::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<TimeTree>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the Rev name for the distribution.
 * This name is used for the constructor and the distribution functions,
 * such as the density and random value function
 *
 * \return Rev name of constructor function.
 */
std::string Dist_CharacterDependentBirthDeathProcess::getDistributionFunctionName( void ) const
{
    // create a distribution name variable that is the same for all instance of this class
    std::string d_name = "CDBDP";
    
    return d_name;
}


MethodTable Dist_CharacterDependentBirthDeathProcess::getDistributionMethods( void ) const
{
    
    MethodTable methods = TypedDistribution<TimeTree>::getDistributionMethods();
    
    
    ArgumentRules* clampCharDataArgRules = new ArgumentRules();
    clampCharDataArgRules->push_back( new ArgumentRule( "value", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The observed value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    //    methods.addFunction("clampAt", new DistributionMemberFunction<TimeTree,RealPos>(this, clampAtArgRules   ) );
    methods.addFunction( new MemberProcedure( "clampCharData", RlUtils::Void, clampCharDataArgRules ) );

    
    return methods;
}



/* Return member rules */
const MemberRules& Dist_CharacterDependentBirthDeathProcess::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "rootAge"   , RealPos::getClassTypeSpec()              , "The age of the root."                        , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "hiddenSpeciationRates"     , ModelVector<Real>::getClassTypeSpec() , "The vector of speciation rates for the hidden states."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "observedSpeciationRates"   , ModelVector<Real>::getClassTypeSpec() , "The vector of speciation rates for the observed states."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "hiddenExtinctionRates"     , ModelVector<Real>::getClassTypeSpec() , "The vector of extinction rates for the hidden states."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "observedExtinctionRates"   , ModelVector<Real>::getClassTypeSpec() , "The vector of extinction rates for the observed states."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "Q"         , RateGenerator::getClassTypeSpec()        , "The rate matrix of jumping between rate categories.", ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "delta"     , RealPos::getClassTypeSpec()              , "The rate-factor of jumping between rate categories.", ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "pi"        , Simplex::getClassTypeSpec(), "State frequencies at the root.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        memberRules.push_back( new ArgumentRule( "rho"       , Probability::getClassTypeSpec()          , "The taxon sampling probability."             , ArgumentRule::BY_CONSTANT_REFERENCE   , ArgumentRule::ANY, new RealPos(1.0) ) );
        
        std::vector<std::string> optionsCondition;
        optionsCondition.push_back( "time" );
        optionsCondition.push_back( "survival" );
        memberRules.push_back( new OptionRule( "condition"    , new RlString("survival"), optionsCondition, "The condition of the birth-death process." ) );
        
        memberRules.push_back( new ArgumentRule( "taxa"      , ModelVector<Taxon>::getClassTypeSpec()   , "The taxa used for simulation."               , ArgumentRule::BY_VALUE                , ArgumentRule::ANY ) );
        
        rulesSet = true;
    }
    
    return memberRules;
}


const TypeSpec& Dist_CharacterDependentBirthDeathProcess::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Set a member variable */
void Dist_CharacterDependentBirthDeathProcess::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "rootAge" )
    {
        root_age = var;
    }
    else if ( name == "pi" )
    {
        root_frequencies = var;
    }
    else if ( name == "hiddenSpeciationRates" )
    {
        hidden_speciation_rates = var;
    }
    else if ( name == "observedSpeciationRates" )
    {
        observed_speciation_rates = var;
    }
    else if ( name == "hiddenExtinctionRates" )
    {
        hidden_extinction_rates = var;
    }
    else if ( name == "observedExtinctionRates" )
    {
        observed_extinction_rates = var;
    }
    else if ( name == "Q" )
    {
        event_rate_matrix = var;
    }
    else if ( name == "delta" )
    {
        event_rate = var;
    }
    else if ( name == "rho" )
    {
        rho = var;
    }
    else if ( name == "condition" )
    {
        condition = var;
    }
    else if ( name == "taxa" )
    {
        taxa = var;
    }
    else
    {
        Distribution::setConstParameter(name, var);
    }
}

