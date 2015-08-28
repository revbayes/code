#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "ConstantNode.h"
#include "RevObject.h"
#include "RbException.h"
#include "Real.h"
#include "RevNullObject.h"
#include "RlPathSampler.h"
#include "RlString.h"
#include "TypeSpec.h"


using namespace RevLanguage;

PathSampler::PathSampler() : WorkspaceToCoreWrapperObject<RevBayesCore::PathSampler>()
{

    ArgumentRules* marginalArgRules = new ArgumentRules();
    methods.addFunction("marginal", new MemberProcedure( Real::getClassTypeSpec(), marginalArgRules) );

}


/** Clone object */
PathSampler* PathSampler::clone(void) const {
    
	return new PathSampler(*this);
}


void PathSampler::constructInternalObject( void ) {
    // we free the memory first
    delete value;
    
    // get the parameter values
    const std::string&   fn      = static_cast<const RlString &>( filename->getRevObject() ).getValue();
    const std::string&   pn      = static_cast<const RlString &>( powerColumnName->getRevObject() ).getValue();
    const std::string&   ln      = static_cast<const RlString &>( likelihoodColumnName->getRevObject() ).getValue();
    const std::string&   del     = static_cast<const RlString &>( delimmiter->getRevObject() ).getValue();
    
    value = new RevBayesCore::PathSampler(fn, pn, ln, del);
    
}


/* Map calls to member methods */
RevPtr<RevVariable> PathSampler::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if (name == "marginal")
    {
        found = true;
        
        double ml = value->marginalLikelihood();
        
        return new RevVariable( new Real( ml ) );
    }
    
    return RevObject::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& PathSampler::getClassType(void)
{
    
    static std::string revType = "PathSampler";
    
	return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& PathSampler::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::PathSampler>::getClassTypeSpec() ) );
    
	return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& PathSampler::getParameterRules(void) const {
    
    static MemberRules samplerMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        samplerMemberRules.push_back( new ArgumentRule("filename"            , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        samplerMemberRules.push_back( new ArgumentRule("powerColumnName"     , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        samplerMemberRules.push_back( new ArgumentRule("likelihoodColumnName", RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        samplerMemberRules.push_back( new ArgumentRule("delimiter"           , RlString::getClassTypeSpec(), ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString( "\t" ) ) );
        
        rulesSet = true;
    }
    
    return samplerMemberRules;
}


/** Get type spec */
const TypeSpec& PathSampler::getTypeSpec( void ) const {
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Get type spec */
void PathSampler::printValue(std::ostream &o) const {
    
    o << "PathSampler";
}


/** Set a member variable */
void PathSampler::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var) {
    
    if ( name == "likelihoodColumnName")
    {
        likelihoodColumnName = var;
    }
    else if ( name == "powerColumnName")
    {
        powerColumnName = var;
    }
    else if ( name == "filename")
    {
        filename = var;
    }
    else if ( name == "delimiter")
    {
        delimmiter = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
}
