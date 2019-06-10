#include "FloorFunction.h"
#include <cmath>

using namespace RevBayesCore;
/**
 * FloorFunction of a TypedDagNode holding a value of type double
 *
 * @param x a value of type double
 */

FloorFunction::FloorFunction(const TypedDagNode<double> *x) : TypedFunction<long>( new long(0) ),
    a( x )
{
    addParameter( x );
    
}


FloorFunction* FloorFunction::clone( void ) const
{
    
    return new FloorFunction(*this);

}


void FloorFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) 
{
    
    if (oldP == a) 
    {
        a = static_cast<const TypedDagNode<double>* >( newP );
    }
    
}

void FloorFunction::update( void ) 
{
    
    *value = int( floor( a->getValue() ) );

}


