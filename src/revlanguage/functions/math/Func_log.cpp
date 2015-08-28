#include "LogFunction.h"
#include "Func_log.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_log::Func_log( void ) : TypedFunction<Real>( ) {
    
}


/** Clone object */
Func_log* Func_log::clone( void ) const {
    
    return new Func_log( *this );
}


RevBayesCore::TypedFunction<double>* Func_log::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<double>* a = static_cast<const RealPos &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<double>* b = static_cast<const RealPos &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::LogFunction* f = new RevBayesCore::LogFunction( a, b );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_log::getArgumentRules( void ) const {
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet ) {
        
        argumentRules.push_back( new ArgumentRule( "x"   , RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new ArgumentRule( "base", RealPos::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_log::getClassType(void) { 
    
    static std::string revType = "Func_log";
    
	return revType; 
}

/* Get class type spec describing type of object */
const TypeSpec& Func_log::getClassTypeSpec(void) { 
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


const TypeSpec& Func_log::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
