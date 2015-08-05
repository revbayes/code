#include "TajimasDFunction.h"
#include "Func_TajimasD.h"
#include "RlAbstractDiscreteCharacterData.h"
#include "RlBoolean.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_TajimasD::Func_TajimasD( void ) : TypedFunction<Real>( )
{
    
}


/** Clone object */
Func_TajimasD* Func_TajimasD::clone( void ) const
{
    
    return new Func_TajimasD( *this );
}


RevBayesCore::TypedFunction< double >* Func_TajimasD::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::AbstractDiscreteCharacterData >* d = static_cast<const AbstractDiscreteCharacterData &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TajimasDFunction* f = new RevBayesCore::TajimasDFunction( d );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_TajimasD::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "data",    AbstractDiscreteCharacterData::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_TajimasD::getClassType(void)
{
    
    static std::string revType = "Func_TajimasD";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_TajimasD::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedFunction<Real>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


const TypeSpec& Func_TajimasD::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}
