#ifndef Dist_phyloDistanceGamma_H
#define Dist_phyloDistanceGamma_H

#include "RlTypedDistribution.h"
#include "TimeTree.h"
#include "MatrixReal.h"

namespace RevLanguage {
    
    template <class treeType>
    class Dist_phyloDistanceGamma :  public TypedDistribution< MatrixReal > {
        
    public:
        Dist_phyloDistanceGamma( void );
        virtual ~Dist_phyloDistanceGamma();
        
        // Basic utility functions
        Dist_phyloDistanceGamma*                                 clone(void) const;                                                              //!< Clone the object
        static const std::string&                       getClassType(void);                                                             //!< Get Rev type
        static const TypeSpec&                          getClassTypeSpec(void);                                                         //!< Get class type spec
        const TypeSpec&                                 getTypeSpec(void) const;                                                        //!< Get the type spec of the instance
        const MemberRules&                              getParameterRules(void) const;                                                     //!< Get member rules (const)
        void                                            printValue(std::ostream& o) const;                                              //!< Print the general information on the function ('usage')
        
        
        // Distribution functions you have to override
        RevBayesCore::TypedDistribution< RevBayesCore::MatrixReal >*      createDistribution(void) const;
        
    protected:
        
        void                                            setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        
        
    private:
        
        RevPtr<const RevVariable>                          tree;
        RevPtr<const RevVariable>                          varianceMatrix;
        RevPtr<const RevVariable>                          distanceMatrix;
		RevPtr<const RevVariable>                          names;
		

		
    };
    
}


#include "PhyloDistanceGamma.h"
#include "OptionRule.h"
#include "RevNullObject.h"
#include "RlBoolean.h"
#include "RlRateGenerator.h"
#include "RlString.h"

template <class treeType>
RevLanguage::Dist_phyloDistanceGamma<treeType>::Dist_phyloDistanceGamma() : TypedDistribution< MatrixReal >()
{
    
}


template <class treeType>
RevLanguage::Dist_phyloDistanceGamma<treeType>::~Dist_phyloDistanceGamma()
{
    
}



template <class treeType>
RevLanguage::Dist_phyloDistanceGamma<treeType>* RevLanguage::Dist_phyloDistanceGamma<treeType>::clone( void ) const
{
  
    return new Dist_phyloDistanceGamma(*this);
}


template <class treeType>
RevBayesCore::TypedDistribution< RevBayesCore::MatrixReal >* RevLanguage::Dist_phyloDistanceGamma<treeType>::createDistribution( void ) const
{
    
    // get the parameters tau, nam, varianceNode and distanceNode that will be used to create the actual distribution.
    RevBayesCore::TypedDagNode<typename treeType::valueType>* tau = static_cast<const treeType &>( tree->getRevObject() ).getDagNode();

	const std::vector<std::string>      &nam  = static_cast<const ModelVector<RlString> &>( names->getRevObject() ).getDagNode()->getValue();

    RevBayesCore::TypedDagNode< RevBayesCore::MatrixReal >* varianceNode = NULL;
	varianceNode = static_cast<const MatrixReal &>( varianceMatrix->getRevObject() ).getDagNode();
	
	RevBayesCore::TypedDagNode< RevBayesCore::MatrixReal >* distanceNode = NULL;
	distanceNode = static_cast<const MatrixReal &>( distanceMatrix->getRevObject() ).getDagNode();
	
	RevBayesCore::PhyloDistanceGamma< typename treeType::valueType >* d = new RevBayesCore::PhyloDistanceGamma< typename treeType::valueType>( tau );
		
	d->setNames( nam );
	
	d->setVarianceMatrix( varianceNode );
	
	d->setDistanceMatrix( distanceNode );
	
	d->redrawValue();

    return d;
}



/* Get Rev type of object */
template <class treeType>
const std::string& RevLanguage::Dist_phyloDistanceGamma<treeType>::getClassType(void) { 
    
	static std::string revType = "Dist_phyloDistanceGamma <" + treeType::getClassType() + ">";
    
	return revType; 
}

/* Get class type spec describing type of object */
template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloDistanceGamma<treeType>::getClassTypeSpec(void) { 
    
	static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( TypedDistribution<treeType>::getClassTypeSpec() ) );

	return revTypeSpec;
}




/** Return member rules (no members) */
template <class treeType>
const RevLanguage::MemberRules& RevLanguage::Dist_phyloDistanceGamma<treeType>::getParameterRules(void) const {
    
    static MemberRules distMemberRules;
    static bool rulesSet = false;
    
    if ( !rulesSet ) 
    {
        distMemberRules.push_back( new ArgumentRule( "tree"           , treeType::getClassTypeSpec(), ArgumentRule::BY_CONSTANT_REFERENCE ) );
        
		distMemberRules.push_back( new ArgumentRule( "distanceMatrix"              , MatrixReal::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE ) );

        distMemberRules.push_back( new ArgumentRule( "varianceMatrix"              , MatrixReal::getClassTypeSpec()             , ArgumentRule::BY_CONSTANT_REFERENCE ) );

		distMemberRules.push_back( new ArgumentRule( "names"  , ModelVector<RlString>::getClassTypeSpec(), ArgumentRule::BY_VALUE ) );

        rulesSet = true;
    }
    
    return distMemberRules;
}


template <class treeType>
const RevLanguage::TypeSpec& RevLanguage::Dist_phyloDistanceGamma<treeType>::getTypeSpec( void ) const
{
    
    static TypeSpec ts = getClassTypeSpec();
    
    return ts;
}


/** Print value for user */
template <class treeType>
void RevLanguage::Dist_phyloDistanceGamma<treeType>::printValue(std::ostream& o) const
{
    
    o << "Distance-Matrix-Generation-Along-Tree Process(tree=";
    if ( tree != NULL ) {
        o << tree->getName();
    } else {
        o << "?";
    }
    o << ", distanceMatrix=";
    if ( distanceMatrix != NULL ) {
        o << distanceMatrix->getName();
    } else {
        o << "?";
    }
    o << ", varianceMatrix=";
    if ( varianceMatrix != NULL ) {
        o << varianceMatrix->getName();
    } else {
        o << "?";
    }
    o << ", names=";
    if ( names != NULL ) {
        o << names->getName();
    } else {
        o << "?";
    }
        o << ")";
    
}


/** Set a member variable */
template <class treeType>
void RevLanguage::Dist_phyloDistanceGamma<treeType>::setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var)
{

	if ( name == "tree" )
	{
		tree = var;
	}
	else if ( name == "distanceMatrix" )
	{
		distanceMatrix = var;
	}
	else if ( name == "varianceMatrix" )
	{
		varianceMatrix = var;
	}
	else if ( name == "names" )
	{
		names = var;
	}
	else
	{
		TypedDistribution< MatrixReal >::setConstParameter(name, var);
	}
 
}


#endif
