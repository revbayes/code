#ifndef Func_modelVector_H
#define Func_modelVector_H

#include "ModelVector.h"
#include "RlTypedFunction.h"

#include <string>

namespace RevLanguage {
    
    /**
     * @brief Func_modelVector: function creating model vectors
     *
     * This templated function constructs vectors and is used for language
     * constructs such as "v( x1, x2, ..., xn)" and "[ x1, x2, ..., xn ]" when
     * the elements are non-abstract model objects with non-abstract value types.
     */
    template <typename valType>
    class Func_modelVector : public TypedFunction< ModelVector< valType> > {
        
    public:
        Func_modelVector(void);                                                                 //!< Default constructor
        
        // Basic utility functions
        Func_modelVector*                                                                               clone(void) const;                                          //!< Clone the object
        static const std::string&                                                                       getClassType(void);                                         //!< Get Rev type
        static const TypeSpec&                                                                          getClassTypeSpec(void);                                     //!< Get class type spec
        const TypeSpec&                                                                                 getTypeSpec(void) const;                                    //!< Get language type of the object
        
        // Regular functions
        RevBayesCore::TypedFunction< RevBayesCore::RbVector<typename valType::valueType > >*            createFunction(void) const;                                 //!< Create a function object
        const ArgumentRules&                                                                            getArgumentRules(void) const;                               //!< Get argument rules
        
    };
    
}


#include "ArgumentRule.h"
#include "Ellipsis.h"
#include "ModelVector.h"
#include "RbUtil.h"
#include "RlDeterministicNode.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "VectorFunction.h"


/** Default constructor */
template <typename valType>
RevLanguage::Func_modelVector<valType>::Func_modelVector() :
    TypedFunction< ModelVector<valType> >()
{
}


/** Clone object */
template <typename valType>
RevLanguage::Func_modelVector<valType>* RevLanguage::Func_modelVector<valType>::clone( void ) const
{
    return new Func_modelVector( *this );
}


/** Execute function: create deterministic ModelVector<valType> object */
template <typename valType>
RevBayesCore::TypedFunction< RevBayesCore::RbVector< typename valType::valueType> >* RevLanguage::Func_modelVector<valType>::createFunction( void ) const
{
    std::vector<const RevBayesCore::TypedDagNode<typename valType::valueType>* > params;
    for ( size_t i = 0; i < this->args.size(); i++ )
    {
        const valType &val = static_cast<const valType &>( this->args[i].getVariable()->getRevObject() );
        params.push_back( val.getDagNode() );
    }
    
    RevBayesCore::VectorFunction<typename valType::valueType>* func = new RevBayesCore::VectorFunction<typename valType::valueType>( params );
    
    return func;
}


/** Get argument rules */
template <typename valType>
const RevLanguage::ArgumentRules& RevLanguage::Func_modelVector<valType>::getArgumentRules( void ) const
{
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        argumentRules.push_back( new ArgumentRule( "", valType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        argumentRules.push_back( new Ellipsis (     valType::getClassTypeSpec() ) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object (static version) */
template <typename valType>
const std::string& RevLanguage::Func_modelVector<valType>::getClassType( void )
{
    static std::string revType = "Func_modelVector<" + valType::getClassType() + ">";
    
	return revType;
}


/** Get Rev type spec of object (static version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVector<valType>::getClassTypeSpec( void )
{
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), &Function::getClassTypeSpec() );
    
	return revTypeSpec;
}


/** Get Rev type spec of object (dynamic version) */
template <typename valType>
const RevLanguage::TypeSpec& RevLanguage::Func_modelVector<valType>::getTypeSpec( void ) const
{
    return this->getClassTypeSpec();
}


#endif

