//
//  Func_DECRateMatrix.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 3/16/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//


#include "ConstantNode.h"
#include "DECRateMatrixFunction.h"
#include "Func_DECRateMatrix.h"
#include "Integer.h"
#include "Natural.h"
#include "RbConstants.h"
#include "Real.h"
#include "RealPos.h"
#include "RlDeterministicNode.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"
#include "TypedDagNode.h"

using namespace RevLanguage;

/** default constructor */
Func_DECRateMatrix::Func_DECRateMatrix( void ) : TypedFunction<RateGenerator>( )
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Func_DECRateMatrix* Func_DECRateMatrix::clone( void ) const
{
    
    return new Func_DECRateMatrix( *this );
}


RevBayesCore::TypedFunction< RevBayesCore::RateGenerator >* Func_DECRateMatrix::createFunction( void ) const
{
    
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<RevBayesCore::RbVector<double> > >* dr;
    dr = static_cast<const ModelVector<ModelVector<RealPos> > &>( this->args[0].getVariable()->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* er;
    er = static_cast<const ModelVector<RealPos> &>( this->args[1].getVariable()->getRevObject() ).getDagNode();
    
    size_t num_statesEr = er->getValue().size();
    std::vector<size_t> num_statesDr;
    for (size_t i = 0; i < dr->getValue().size(); i++)
    {
        num_statesDr.push_back( dr->getValue()[i].size() );
        if (num_statesDr[i] != num_statesEr)
        {
            throw RbException("The dimension between dispersal and extirpation rates does not match.");
        }
        for (size_t j = 0; j < i; j++)
        {
            if (num_statesDr[i] != num_statesDr[j])
            {
                throw RbException("The dispersal matrix is not square.");
            }
        }
    }
    if (dr->getValue().size() != num_statesEr)
    {
        throw RbException("The dimension between dispersal and extirpation rates does not match.");
    }

    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* rs = NULL;
    if ( this->args[2].getVariable() != NULL && this->args[2].getVariable()->getRevObject() != RevNullObject::getInstance()) {
        rs = static_cast<const Simplex&>( this->args[2].getVariable()->getRevObject() ).getDagNode();
    }
    else {
        size_t n = num_statesEr+1;
        double p = 1.0 / n;
        rs = new RevBayesCore::ConstantNode<RevBayesCore::RbVector<double> >("", new RevBayesCore::RbVector<double>(n,p));
    }
//    RevBayesCore::TypedDagNode<int>* mrs = static_cast<const Natural&>( this->args[3].getVariable()->getRevObject() ).getDagNode();
    
    RevBayesCore::DECRateMatrixFunction* f = new RevBayesCore::DECRateMatrixFunction( dr, er, rs );
    
    return f;
}


/* Get argument rules */
const ArgumentRules& Func_DECRateMatrix::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool          rulesSet = false;
    
    if ( !rulesSet )
    {
        
        argumentRules.push_back( new ArgumentRule( "dispersalRates"  , ModelVector<ModelVector<RealPos> >::getClassTypeSpec(), "Matrix of dispersal rates between areas.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "extirpationRates", ModelVector<RealPos>::getClassTypeSpec(), "The per are extinction rates.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "rangeSize",        Simplex::getClassTypeSpec(), "Range size ...", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
//        argumentRules.push_back( new ArgumentRule( "maxRangeSize", Natural::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(RbConstants::Integer::max) ) );
        
        
        rulesSet = true;
    }
    
    return argumentRules;
}


const std::string& Func_DECRateMatrix::getClassType(void)
{
    
    static std::string revType = "Func_DECRateMatrix";
    
	return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& Func_DECRateMatrix::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
	return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_DECRateMatrix::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "fnDECRateMatrix";
    
    return f_name;
}


const TypeSpec& Func_DECRateMatrix::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}