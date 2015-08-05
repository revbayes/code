#include "Func_FreeK.h"
#include "FreeKRateMatrixFunction.h"
#include "Natural.h"
#include "RateMatrix_JC.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TransitionProbabilityMatrix.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_FreeK::Func_FreeK( void ) : TypedFunction<RateGenerator>( )
{
    
}


/** Clone object */
Func_FreeK* Func_FreeK::clone( void ) const
{
    
    return new Func_FreeK( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_FreeK::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* tr = static_cast<const Simplex &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* sf = static_cast<const Simplex &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::FreeKRateMatrixFunction* f = new RevBayesCore::FreeKRateMatrixFunction( tr,sf );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_FreeK::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "transitionRates", Simplex::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "stationaryFrequencies", Simplex::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_FreeK::getClassType(void) {
    
    static std::string revType = "Func_FreeK";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_FreeK::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


const TypeSpec& Func_FreeK::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
