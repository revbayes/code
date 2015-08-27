#include "Func__mod.h"

#include "ArgumentRule.h"
#include "ModuloFunction.h"
#include "Natural.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"

using namespace RevLanguage;

Func__mod::Func__mod() : TypedFunction<Natural>()
{
    
}

/* Clone object */
Func__mod* Func__mod::clone( void ) const
{
    
    return new Func__mod( *this );
}


RevBayesCore::TypedFunction<int>* Func__mod::createFunction( void ) const
{
    
    const RevBayesCore::TypedDagNode<int>* leftVal = static_cast<const Natural &>( args[0].getVariable()->getRevObject() ).getDagNode();
    const RevBayesCore::TypedDagNode<int>* rightVal = static_cast<const Natural &>( args[1].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::ModuloFunction *func = new RevBayesCore::ModuloFunction( leftVal, rightVal );
    
    return func;
    
}


/** Get argument rules */
const ArgumentRules& Func__mod::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "x", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "y", Natural::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func__mod::getClassType(void)
{
    
    static std::string revType = "Func__mod";
    
	return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func__mod::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/** Get type spec */
const TypeSpec& Func__mod::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

