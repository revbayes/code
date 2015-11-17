#include "Func_convertToPhylowood.h"
#include "ArgumentRule.h"
#include "ConstantNode.h"
#include "ModelVector.h"
#include "NexusWriter.h"
#include "OptionRule.h"
#include "PhylowoodConverter.h"
#include "Probability.h"
#include "RbException.h"
#include "RevNullObject.h"
#include "RlBranchLengthTree.h"
#include "RlString.h"
#include "RlTimeTree.h"
#include "RlTreeTrace.h"
#include "RlAncestralStateTrace.h"
#include "RlUtils.h"
#include "StringUtilities.h"
#include "TreeSummary.h"
#include "TreeTrace.h"
#include "AncestralStateTrace.h"
#include "WorkspaceVector.h"

#include <map>
#include <set>
#include <sstream>

using namespace RevLanguage;


/** Clone object */
Func_convertToPhylowood* Func_convertToPhylowood::clone( void ) const
{
    
    return new Func_convertToPhylowood( *this );
}


/** Execute function */
RevPtr<RevVariable> Func_convertToPhylowood::execute( void )
{
    
    
    const std::string& stateFilename = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
    const std::string& treeFilename  = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    const std::string& geoFilename   = static_cast<const RlString&>( args[2].getVariable()->getRevObject() ).getValue();
    const std::string& phwFilename   = static_cast<const RlString&>( args[3].getVariable()->getRevObject() ).getValue();
    double burn = static_cast<const Probability&>( args[4].getVariable()->getRevObject() ).getValue();
    const std::string& chartype      = static_cast<const RlString&>( args[5].getVariable()->getRevObject() ).getValue();
    const std::string& bgtype      = static_cast<const RlString&>( args[6].getVariable()->getRevObject() ).getValue();
    
    RevBayesCore::PhylowoodConverter phw(stateFilename, treeFilename, geoFilename, phwFilename, burn, chartype, bgtype);
    //    RevBayesCore::PhylowoodConverter phw;
    
    return NULL;
}



/** Get argument rules */
const ArgumentRules& Func_convertToPhylowood::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        argumentRules.push_back( new ArgumentRule( "statefile", RlString::getClassTypeSpec()           , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "treefile",  RlString::getClassTypeSpec()           , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "geofile",   RlString::getClassTypeSpec()           , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "outfile",   RlString::getClassTypeSpec()           , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "burnin",    Probability::getClassTypeSpec()            , "", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Integer(-1) ) );
        
        std::vector<std::string> options;
        options.push_back( "NaturalNumbers" );
        options.push_back( "Standard" );
        argumentRules.push_back( new OptionRule( "chartype", new RlString("NaturalNumbers"), options, "" ) );
        
        std::vector<std::string> options2;
        options2.push_back( "Range" );
        options2.push_back( "Area" );
        argumentRules.push_back( new OptionRule( "bgtype", new RlString("Area"), options2, "" ) );
        
        rulesSet = true;
    }
    
    return argumentRules;
}


/** Get Rev type of object */
const std::string& Func_convertToPhylowood::getClassType(void)
{
    
    static std::string revType = "Func_convertToPhylowood";
    
    return revType;
}


/** Get class type spec describing type of object */
const TypeSpec& Func_convertToPhylowood::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_convertToPhylowood::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "convertToPhylowood";
    
    return f_name;
}


/** Get type spec */
const TypeSpec& Func_convertToPhylowood::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get return type */
const TypeSpec& Func_convertToPhylowood::getReturnType( void ) const {
    
    static TypeSpec returnTypeSpec = Tree::getClassTypeSpec();
    return returnTypeSpec;
}