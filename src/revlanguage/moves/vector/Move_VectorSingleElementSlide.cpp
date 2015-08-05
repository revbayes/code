#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "RlBoolean.h"
#include "ContinuousStochasticNode.h"
#include "MetropolisHastingsMove.h"
#include "ModelVector.h"
#include "Move_VectorSingleElementSlide.h"
#include "Natural.h"
#include "RbException.h"
#include "Real.h"
#include "RealPos.h"
#include "RevObject.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"
#include "VectorSingleElementSlideProposal.h"


using namespace RevLanguage;

Move_VectorSingleElementSlide::Move_VectorSingleElementSlide() : Move()
{
    
}

/** Clone object */
Move_VectorSingleElementSlide* Move_VectorSingleElementSlide::clone(void) const
{
    
	return new Move_VectorSingleElementSlide(*this);
}


void Move_VectorSingleElementSlide::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    double l = static_cast<const RealPos &>( lambda->getRevObject() ).getValue();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::TypedDagNode<RevBayesCore::RbVector<double> >* tmp = static_cast<const ModelVector<Real> &>( v->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *n = static_cast<RevBayesCore::StochasticNode<RevBayesCore::RbVector<double> > *>( tmp );
    bool t = static_cast<const RlBoolean &>( tune->getRevObject() ).getValue();
    
    RevBayesCore::Proposal *p = new RevBayesCore::VectorSingleElementSlideProposal(n,l);
    value = new RevBayesCore::MetropolisHastingsMove(p,w,t);
}


/** Get Rev type of object */
const std::string& Move_VectorSingleElementSlide::getClassType(void)
{
    
    static std::string revType = "Move_VectorSingleElementSlide";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_VectorSingleElementSlide::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}



/** Return member rules (no members) */
const MemberRules& Move_VectorSingleElementSlide::getParameterRules(void) const
{
    
    static MemberRules moveMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        moveMemberRules.push_back( new ArgumentRule( "x"     , ModelVector<Real>::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        moveMemberRules.push_back( new ArgumentRule( "lambda", RealPos::getClassTypeSpec()          , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new Real(1.0) ) );
        moveMemberRules.push_back( new ArgumentRule( "tune"  , RlBoolean::getClassTypeSpec()        , ArgumentRule::BY_VALUE    , ArgumentRule::ANY, new RlBoolean( true ) ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        moveMemberRules.insert( moveMemberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return moveMemberRules;
}

/** Get type spec */
const TypeSpec& Move_VectorSingleElementSlide::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void Move_VectorSingleElementSlide::printValue(std::ostream &o) const
{
    
    o << "Move_VectorSingleElementSlide(";
    if (v != NULL)
    {
        o << v->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a member variable */
void Move_VectorSingleElementSlide::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "x" )
    {
        v = var;
    }
    else if ( name == "lambda" )
    {
        lambda = var;
    }
    else if ( name == "weight" )
    {
        weight = var;
    }
    else if ( name == "tune" )
    {
        tune = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
}
