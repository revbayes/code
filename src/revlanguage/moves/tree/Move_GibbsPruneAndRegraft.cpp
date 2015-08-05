#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "GibbsPruneAndRegraft.h"
#include "Move_GibbsPruneAndRegraft.h"
#include "RbException.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RlTimeTree.h"
#include "TypedDagNode.h"
#include "TypeSpec.h"


using namespace RevLanguage;

Move_GibbsPruneAndRegraft::Move_GibbsPruneAndRegraft() : Move() {
    
}


/** Clone object */
Move_GibbsPruneAndRegraft* Move_GibbsPruneAndRegraft::clone(void) const {
    
    return new Move_GibbsPruneAndRegraft(*this);
}


void Move_GibbsPruneAndRegraft::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // now allocate a new sliding move
    RevBayesCore::TypedDagNode<RevBayesCore::TimeTree> *tmp = static_cast<const TimeTree &>( tree->getRevObject() ).getDagNode();
    double w = static_cast<const RealPos &>( weight->getRevObject() ).getValue();
    RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *t = static_cast<RevBayesCore::StochasticNode<RevBayesCore::TimeTree> *>( tmp );
    value = new RevBayesCore::GibbsPruneAndRegraft(t, w);
}


/** Get Rev type of object */
const std::string& Move_GibbsPruneAndRegraft::getClassType(void) {
    
    static std::string revType = "Move_GibbsPruneAndRegraft";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& Move_GibbsPruneAndRegraft::getClassTypeSpec(void) {
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Move::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& Move_GibbsPruneAndRegraft::getParameterRules(void) const {
    
    static MemberRules memberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        memberRules.push_back( new ArgumentRule( "tree", TimeTree::getClassTypeSpec(), ArgumentRule::BY_REFERENCE, ArgumentRule::STOCHASTIC ) );
        
        /* Inherit weight from Move, put it after variable */
        const MemberRules& inheritedRules = Move::getParameterRules();
        memberRules.insert( memberRules.end(), inheritedRules.begin(), inheritedRules.end() );
        
        rulesSet = true;
    }
    
    return memberRules;
}

/** Get type spec */
const TypeSpec& Move_GibbsPruneAndRegraft::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}



/** Get type spec */
void Move_GibbsPruneAndRegraft::printValue(std::ostream &o) const {
    
    o << "GibbsPruneAndRegraft(";
    if (tree != NULL) {
        o << tree->getName();
    }
    else {
        o << "?";
    }
    o << ")";
}


/** Set a NearestNeighborInterchange variable */
void Move_GibbsPruneAndRegraft::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "tree" ) {
        tree = var;
    }
    else {
        Move::setConstParameter(name, var);
    }
}
