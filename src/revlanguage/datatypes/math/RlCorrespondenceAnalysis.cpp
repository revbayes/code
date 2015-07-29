
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "CorrespondenceAnalysis.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RealPos.h"
#include "RevObject.h"
#include "RbException.h"
#include "RlCorrespondenceAnalysis.h"
#include "RlMatrixReal.h"
#include "TypeSpec.h"

#include <vector>
#include <string>

using namespace RevLanguage;

CorrespondenceAnalysis::CorrespondenceAnalysis() : WorkspaceToCoreWrapperObject<RevBayesCore::CorrespondenceAnalysis>()
{
    
    // Add methods
    ArgumentRules* columnWeightsArgRules = new ArgumentRules();
    ArgumentRules* principalAxesArgRules = new ArgumentRules();
    
    methods.addFunction("columnWeights", new MemberProcedure(ModelVector<RealPos>::getClassTypeSpec(), columnWeightsArgRules ) );
    methods.addFunction("principalAxes", new MemberProcedure(MatrixReal::getClassTypeSpec()          , principalAxesArgRules ) );
}


/** Clone object */
CorrespondenceAnalysis* CorrespondenceAnalysis::clone(void) const
{
    
    return new CorrespondenceAnalysis(*this);
}


void CorrespondenceAnalysis::constructInternalObject( void )
{
    // we free the memory first
    delete value;
    
    // now allocate a CorrespondenceAnalysis
    RevBayesCore::MatrixReal d  = static_cast<const MatrixReal &>( data->getRevObject() ).getValue();
    unsigned int na             = static_cast<const Natural &>( num_axes->getRevObject() ).getValue();
    double tol                  = static_cast<const RealPos &>( tolerance->getRevObject() ).getValue();
    
    value = new RevBayesCore::CorrespondenceAnalysis( d, na, tol );
    
}

/* Map calls to member methods */
RevPtr<RevVariable> CorrespondenceAnalysis::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    if ( name == "columnWeights" )
    {
        found = true;
        
        const std::vector<double> &w = value->getColumnWeights();
        
        return new RevVariable( new ModelVector<RealPos>( w ) );
    }
    else if ( name == "principalAxes" )
    {
        found = true;
        
        const RevBayesCore::MatrixReal &m = value->getPrincipalAxes();
        
        return new RevVariable( new MatrixReal( m ) );
    }
    
    return RevObject::executeMethod( name, args, found );
}

/** Get Rev type of object */
const std::string& CorrespondenceAnalysis::getClassType(void)
{
    
    static std::string revType = "CorrespondenceAnalysis";
    
    return revType;
}

/** Get class type spec describing type of object */
const TypeSpec& CorrespondenceAnalysis::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( WorkspaceToCoreWrapperObject<RevBayesCore::CorrespondenceAnalysis>::getClassTypeSpec() ) );
    
    return revTypeSpec;
}



/** Return member rules (no members) */
const MemberRules& CorrespondenceAnalysis::getParameterRules(void) const
{
    
    static MemberRules CorrespondenceAnalysisMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet )
    {
        
        CorrespondenceAnalysisMemberRules.push_back( new ArgumentRule("data",       MatrixReal::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );
        CorrespondenceAnalysisMemberRules.push_back( new ArgumentRule("numAxes",    Natural::getClassTypeSpec(),    ArgumentRule::BY_VALUE ) );
        CorrespondenceAnalysisMemberRules.push_back( new ArgumentRule("tolerance",  RealPos::getClassTypeSpec(),    ArgumentRule::BY_VALUE , ArgumentRule::ANY, new RealPos(0.0000001) ) );
        
        rulesSet = true;
    }
    
    return CorrespondenceAnalysisMemberRules;
}


/** Get type spec */
const TypeSpec& CorrespondenceAnalysis::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/** Print a simplified representation of the CorrespondenceAnalysis for the user. */
void CorrespondenceAnalysis::printValue(std::ostream &o) const
{
    
    o << "CorrespondenceAnalysis";
    
}


/** Set a member variable */
void CorrespondenceAnalysis::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{
    
    if ( name == "data" )
    {
        data = var;
    }
    else if ( name == "numAxes" )
    {
        num_axes = var;
    }
    else if ( name == "tolerance" )
    {
        tolerance = var;
    }
    else
    {
        RevObject::setConstParameter(name, var);
    }
    
}

