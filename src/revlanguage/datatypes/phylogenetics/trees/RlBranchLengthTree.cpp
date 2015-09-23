#include "ModelVector.h"
#include "Natural.h"
#include "RbUtil.h"
#include "RlBranchLengthTree.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "RealPos.h"
#include "TypeSpec.h"

#include <sstream>

using namespace RevLanguage;

/** Default constructor */
BranchLengthTree::BranchLengthTree(void) : Tree()
{
    
    ArgumentRules* rerootArgRules = new ArgumentRules();
    rerootArgRules->push_back( new ArgumentRule("leaf"    , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    
    methods.addFunction("reroot", new MemberProcedure(RlUtils::Void,  rerootArgRules       ) );
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(RevBayesCore::Tree *t) : Tree( t )
{

    ArgumentRules* rerootArgRules = new ArgumentRules();
    rerootArgRules->push_back( new ArgumentRule("leaf"    , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    
    methods.addFunction("reroot", new MemberProcedure(RlUtils::Void,  rerootArgRules       ) );
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(const RevBayesCore::Tree &t) : Tree( new RevBayesCore::Tree( t ) )
{
    
    ArgumentRules* rerootArgRules = new ArgumentRules();
    rerootArgRules->push_back( new ArgumentRule("leaf"    , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    
    methods.addFunction("reroot", new MemberProcedure(RlUtils::Void,  rerootArgRules       ) );
    
}

/** Construct from bool */
BranchLengthTree::BranchLengthTree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n) : Tree( n )
{
    
    ArgumentRules* rerootArgRules = new ArgumentRules();
    rerootArgRules->push_back( new ArgumentRule("leaf"    , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE) );
    
    methods.addFunction("reroot", new MemberProcedure(RlUtils::Void,  rerootArgRules       ) );

}


/** Clone object */
BranchLengthTree* BranchLengthTree::clone(void) const
{
    
	return new BranchLengthTree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::RevVariable> BranchLengthTree::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "reroot")
    {
        
        const RevObject& st = args[0].getVariable()->getRevObject();
        if ( st.isType( RlString::getClassTypeSpec() ) )
        {
            std::string n = std::string( static_cast<const RlString&>( st ).getValue() );
            this->dagNode->getValue().reroot(n);
        }
        return NULL;
        
    }
    
    return Tree::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& BranchLengthTree::getClassType(void)
{
    
    static std::string revType = "BranchLengthTree";
    
	return revType; 
}

/** Get class type spec describing type of object */
const TypeSpec& BranchLengthTree::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Tree::getClassTypeSpec() ) );
    
	return revTypeSpec; 
}


/** Get type spec */
const TypeSpec& BranchLengthTree::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}

