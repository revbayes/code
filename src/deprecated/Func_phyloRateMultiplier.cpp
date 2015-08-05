#include "Func_phyloRateMultiplier.h"
#include "ModelVector.h"
#include "RateMultiplierPhyloFunction.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlTimeTree.h"

using namespace RevLanguage;

/** default constructor */
Func_phyloRateMultiplier::Func_phyloRateMultiplier( void ) : TypedFunction< ModelVector<RealPos> >( ) {
    
}


/** Clone object */
Func_phyloRateMultiplier* Func_phyloRateMultiplier::clone( void ) const 
{
    
    return new Func_phyloRateMultiplier( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RbVector<double> >* Func_phyloRateMultiplier::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode< RevBayesCore::TimeTree >* tree = static_cast<const TimeTree &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< RevBayesCore::RbVector<double> >* rates = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode< double >* baseRate = static_cast<const RealPos &>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::RateMultiplierPhyloFunction* f = new RevBayesCore::RateMultiplierPhyloFunction( tree, rates, baseRate );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_phyloRateMultiplier::getArgumentRules( void ) const 
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "tree"    , TimeTree::getClassTypeSpec()            , ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "rates"   , ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "baseRate", RealPos::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY, new RealPos(1.0) ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_phyloRateMultiplier::getClassType(void) { 
    
    static std::string revType = "Func_phyloRateMultiplier";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_phyloRateMultiplier::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


const TypeSpec& Func_phyloRateMultiplier::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
