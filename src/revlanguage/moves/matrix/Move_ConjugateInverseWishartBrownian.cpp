#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConjugateInverseWishartBrownianMove.h"
#include "ModelVector.h"
#include "Move_ConjugateInverseWishartBrownian.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlBoolean.h"
#include "RlMatrixReal.h"
#include "RlMatrixRealSymmetric.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_ConjugateInverseWishartBrownian::Move_ConjugateInverseWishartBrownian() : Move()
{
    
}


/** Clone object */
Move_ConjugateInverseWishartBrownian* Move_ConjugateInverseWishartBrownian::clone(void) const
{
    
    return new Move_ConjugateInverseWishartBrownian(*this);
}


void Move_ConjugateInverseWishartBrownian::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<double>* k = static_cast<const Real&>( kappa->getRevObject() ).getDagNode();
    RevBayesCore::TypedDagNode<int>* d = static_cast<const Natural&>( df->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();

    RevBayesCore::TypedDagNode< RevBayesCore::MatrixReal >* tmp = static_cast<const MatrixRealSymmetric &>( x->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode< RevBayesCore::MatrixReal > *z = static_cast< RevBayesCore::StochasticNode< RevBayesCore::MatrixReal >* >( tmp );
    
    value = new RevBayesCore::ConjugateInverseWishartBrownianMove(z, k, d, w);
}


/** Get Rev type of object */
const std::string& Move_ConjugateInverseWishartBrownian::getClassType(void)
{
    
    static std::string revType = "Move_ConjugateInverseWishartBrownian";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_ConjugateInverseWishartBrownian::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Move_ConjugateInverseWishartBrownian::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        moveMemberRules.push_back( new ArgumentRule( "x"     , MatrixRealSymmetric::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "kappa" , Real::getClassTypeSpec()               , ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        moveMemberRules.push_back( new ArgumentRule( "df"    , Natural::getClassTypeSpec()            , ArgumentRule::BY_REFERENCE, ArgumentRule::ANY ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_ConjugateInverseWishartBrownian::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_ConjugateInverseWishartBrownian::printValue(std::ostream &o) const
{
    
    o << "ConjugateInverseWishartBrownian(";
    if (x != NULL)
    {
        o << x->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_ConjugateInverseWishartBrownian::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "x" )
    {
        x = var;
    }
    else if ( name == "kappa" )
    {
        kappa = var;
    }
    else if ( name == "df" )
    {
        df = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
    
}
