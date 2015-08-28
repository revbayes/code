#include "SumFunction.h"
#include "Func_sumPositive.h"
#include "ModelVector.h"
#include "Real.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_sumPositive::Func_sumPositive( void ) : TypedFunction<RealPos>( )
{
    
}


/** Clone object */
Func_sumPositive* Func_sumPositive::clone( void ) const
{
    
    return new Func_sumPositive( *this );
}


RevBayesCore::TypedFunction<double>* Func_sumPositive::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* arg = static_cast<const ModelVector<Real> &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::SumFunction* f = new RevBayesCore::SumFunction( arg );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_sumPositive::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", ModelVector<RealPos>::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_sumPositive::getClassType(void)
{
    
    static std::string revType = "Func_sum";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_sumPositive::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_sumPositive::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
