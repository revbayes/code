#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "MetropolisHastingsMove.h"
#include "Move_TipTimeSlideUniform.h"
#include "Natural.h"
#include "TipTimeSlideUniformProposal.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_TipTimeSlideUniform::Move_TipTimeSlideUniform() : Move()
{
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Move_TipTimeSlideUniform* Move_TipTimeSlideUniform::clone(void) const
{
    
	return new Move_TipTimeSlideUniform(*this);
}


void Move_TipTimeSlideUniform::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a new move
    RevBayesCore::TypedDagNode<RevBayesCore::Tree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    RevBayesCore::StochasticNode<RevBayesCore::Tree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::Tree> *>( tmp );
    
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<double> *d = NULL;
    if ( origin != NULL && origin->getRevObject() != RevNullObject::getInstance() )
    {
        RevBayesCore::TypedDagNode<double> *tmp = static_cast<const RealPos &>( origin->getRevObject() ).getDagNode();
        d = static_cast<RevBayesCore::StochasticNode<double> *>( tmp );
    }

    RevBayesCore::TipTimeSlideUniformProposal *p = new RevBayesCore::TipTimeSlideUniformProposal( t, d);

    if ( node_index != NULL && node_index->getRevObject() != RevNullObject::getInstance() )
    {
        size_t ti = static_cast<const Natural &>( node_index->getRevObject() ).getValue();

        p->useIndex(ti);
    }

    value = new RevBayesCore::MetropolisHastingsMove(p, w, false);
}


/** Get Rev type of object */
const std::string& Move_TipTimeSlideUniform::getClassType(void)
{
    
    static std::string rev_type = "Move_TipTimeSlideUniform";
    
	return rev_type; 
}

/** Get class type spec describing type of object */
const TypeSpec& Move_TipTimeSlideUniform::getClassTypeSpec(void)
{
    
    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
	return rev_type_spec; 
}


/**
 * Get the Rev name for the constructor function.
 *
 * \return Rev name of constructor function.
 */
std::string Move_TipTimeSlideUniform::getMoveName( void ) const
{
    // create a constructor function name variable that is the same for all instance of this class
    std::string c_name = "TipTimeSlideUniform";
    
    return c_name;
}

std::vector<std::string> Move_TipTimeSlideUniform::getMoveAliases(void) const
{
    std::vector<std::string> aliases;

    aliases.push_back("FossilTimeSlideUniform");

    return aliases;
}


/** Return member rules (no members) */
const MemberRules& Move_TipTimeSlideUniform::getParameterRules(void) const
{
    
    static MemberRules memberRules;
    static bool rules_set = false;
    
    if ( !rules_set )
    {
        
        memberRules.push_back( new ArgumentRule( "tree", TimeTree::getClassTypeSpec(), "The tree on which this move operates.", ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        memberRules.push_back( new ArgumentRule( "origin", RealPos::getClassTypeSpec() , "The variable for the origin of the process giving a maximum age.", ArgumentRule::BY_REFERENCE, ArgumentRule::ANY, NULL) );
        memberRules.push_back( new ArgumentRule( "index", Natural::getClassTypeSpec() , "Only propose changes to the tip with this index.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, NULL) );

        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rules_set = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Move_TipTimeSlideUniform::getTypeSpec( void ) const
{
    
    static TypeSpec type_spec = getClassTypeSpec();
    
    return type_spec;
}



/** Get type spec */
void Move_TipTimeSlideUniform::printValue(std::ostream &o) const
{
    
    o << "Move_TipTimeSlideUniform(";
    if (tree != NULL)
    {
        o << tree->getName();
    }
    else
    {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_TipTimeSlideUniform::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "tree" )
    {
        tree = var;
    }
    else if (name == "origin")
    {
        origin = var;
    }
    else if (name == "index")
    {
        node_index = var;
    }
    else
    {
        Move::setConstParameter(name, var);
    }
}
