#include <sstream>
#include <cstddef>
#include <string>
#include <vector>

#include "ModelVector.h"
#include "Natural.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlBoolean.h"
#include "RlClade.h"
#include "RlTree.h"
#include "RlMemberFunction.h"
#include "RlString.h"
#include "RlTaxon.h"
#include "RealPos.h"
#include "TopologyNode.h"
#include "TreeUtilities.h"
#include "TypeSpec.h"
#include "Argument.h"
#include "ArgumentRule.h"
#include "ArgumentRules.h"
#include "Clade.h"
#include "ConstantNode.h"
#include "DagNode.h"
#include "DeterministicNode.h"
#include "DynamicNode.h"
#include "IndirectReferenceFunction.h"
#include "MemberFunction.h"
#include "MemberProcedure.h"
#include "MethodTable.h"
#include "ModelObject.h"
#include "RbBoolean.h"
#include "RbVector.h"
#include "RbVectorImpl.h"
#include "Real.h"
#include "RevObject.h"
#include "RevPtr.h"
#include "RevVariable.h"
#include "RlConstantNode.h"
#include "RlDeterministicNode.h"
#include "RlTypedFunction.h"
#include "RlUtils.h"
#include "Taxon.h"
#include "Tree.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "UserFunctionNode.h"

using namespace RevLanguage;

/** Default constructor */
Tree::Tree(void) : ModelObject<RevBayesCore::Tree>()
{

    initMethods();

}

/** Construct from core pointer */
Tree::Tree(RevBayesCore::Tree *t) : ModelObject<RevBayesCore::Tree>( t )
{

    initMethods();

}

/** Construct from core reference */
Tree::Tree(const RevBayesCore::Tree &t) : ModelObject<RevBayesCore::Tree>( new RevBayesCore::Tree( t ) )
{

    initMethods();

}

/** Construct from core DAG node */
Tree::Tree(RevBayesCore::TypedDagNode<RevBayesCore::Tree> *n) : ModelObject<RevBayesCore::Tree>( n )
{

    initMethods();

}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the process.
 */
Tree* Tree::clone(void) const
{

    return new Tree(*this);
}


/* Map calls to member methods */
RevLanguage::RevPtr<RevLanguage::RevVariable> Tree::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{

    if (name == "dropTip")
    {
        found = true;

        const RevObject &taxon = args[0].getVariable()->getRevObject();
        std::string taxon_name = "";
        if ( taxon.isType( RlString::getClassTypeSpec() ) )
        {
            taxon_name = static_cast<const RlString&>( taxon ).getValue();
        }
        else
        {
            taxon_name = static_cast<const Taxon&>( taxon ).getValue().getSpeciesName();
        }

        this->dag_node->getValue().dropTipNodeWithName( taxon_name );

        return NULL;
    }
    else if (name == "getClade")
    {
        found = true;
        
        const std::vector<RevBayesCore::Taxon> &taxa = static_cast<const ModelVector<Taxon>&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::Clade tmp = RevBayesCore::Clade( taxa );
        tmp.resetTaxonBitset( this->dag_node->getValue().getTaxonBitSetMap() );
        RevBayesCore::Clade c = this->dag_node->getValue().getMrca( tmp ).getClade();
        return new RevVariable( new Clade( c ) );
    }
    else if (name == "isBinary")
    {
        found = true;

        bool tf = this->dag_node->getValue().isBinary();
        return new RevVariable( new RlBoolean( tf ) );
    }
    else if (name == "isInternal")
    {
        found = true;

        long index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() ).getValue() - 1;

        bool tf = this->dag_node->getValue().getNode((size_t)index).isInternal();
        return new RevVariable( new RlBoolean( tf ) );
    }
    else if (name == "names" || name == "taxa")
    {
        found = true;

        std::vector<RevBayesCore::Taxon> t = this->dag_node->getValue().getTaxa();
        return new RevVariable( new ModelVector<Taxon>( t ) );
    }
    else if (name == "setBranchLength")
    {
        found = true;

        const RevObject& current = args[0].getVariable()->getRevObject();
        if ( current.isType( Natural::getClassTypeSpec() ) )
        {
          size_t index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() ).getValue() - 1;
          const RevObject& new_value = args[1].getVariable()->getRevObject();
          if ( new_value.isType( RealPos::getClassTypeSpec() ) )
          {
            double value = static_cast<const RealPos&>( new_value ).getValue();
            RevBayesCore::Tree &tree = dag_node->getValue();
            RevBayesCore::TreeUtilities::setBranchLength( &tree, index ,value );
          }
        }
        return NULL;
    }
    else if (name == "setTaxonName")
    {
        found = true;
        
        const RevObject& current = args[0].getVariable()->getRevObject();
        if ( current.isType( RlString::getClassTypeSpec() ) )
        {
            std::string n = std::string( static_cast<const RlString&>( current ).getValue() );
            const RevObject& new_name = args[1].getVariable()->getRevObject();
            if ( new_name.isType( RlString::getClassTypeSpec() ) )
            {
                std::string name = std::string( static_cast<const RlString&>( new_name ).getValue() );
                getDagNode()->getValue().setTaxonName( n ,name );
                // std::cout << "new name: "<< dagNode->getValue().getTaxonData( n ).getTaxonName() << std::endl;
            }
        }
        return NULL;
    }
    else if (name == "nodeName")
    {
        found = true;

        long index = static_cast<const Natural&>( args[0].getVariable()->getRevObject() ).getValue() - 1;
        const std::string& n = this->dag_node->getValue().getNode((size_t)index).getName();
        return new RevVariable( new RlString( n ) );
    }
    else if (name == "removeDuplicateTaxa")
    {
        found = true;

        RevBayesCore::Tree &tree = dag_node->getValue();
        tree.removeDuplicateTaxa();
        
        return NULL;
    }
    else if (name == "rescale")
    {
        found = true;
        
        double f = static_cast<const RealPos&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::Tree &tree = dag_node->getValue();
        RevBayesCore::TreeUtilities::rescaleTree(&tree, &tree.getRoot(), f);
        
        return NULL;
    }
    else if (name == "offset")
    {
        found = true;

        double f = static_cast<const RealPos&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::Tree &tree = dag_node->getValue();
        RevBayesCore::TreeUtilities::offsetTree(&tree, &tree.getRoot(), f);

        return NULL;
    }
    else if (name == "setNegativeConstraint")
    {
        found = true;

        double tf = static_cast<const RlBoolean&>( args[0].getVariable()->getRevObject() ).getValue();
        RevBayesCore::Tree &tree = dag_node->getValue();
        tree.setNegativeConstraint(tf);
//        RevBayesCore::TreeUtilities::rescaleTree(&tree, &tree.getRoot(), f);

        return NULL;
    }
    else if (name == "tipIndex")
    {
        found = true;
        
        std::string tip_name = "";
        if ( args[0].getVariable()->getRevObject().getType() == RlString::getClassType() )
        {
            tip_name = static_cast<const RlString&>( args[0].getVariable()->getRevObject() ).getValue();
        }
        else if ( args[0].getVariable()->getRevObject().getType() == Taxon::getClassType() )
        {
            tip_name = static_cast<const Taxon&>( args[0].getVariable()->getRevObject() ).getValue().getSpeciesName();
        }
        long index = this->dag_node->getValue().getTipNodeWithName( tip_name ).getIndex() + 1;
        return new RevVariable( new Natural( index ) );
    }
    else if (name == "makeUltrametric")
    {

        found = true;

        RevBayesCore::Tree &tree = dag_node->getValue();
        RevBayesCore::TreeUtilities::makeUltrametric(&tree);

        return NULL;
    }
    else if ( name == "getPSSP" )
    {
        found = true;
        const AbstractHomologousDiscreteCharacterData c = static_cast<const AbstractHomologousDiscreteCharacterData& >( args[0].getVariable()->getRevObject() ).getValue();
        size_t state_index = static_cast<const Natural&>( args[1].getVariable()->getRevObject() ).getValue();
        
        std::vector<double> bl = RevBayesCore::TreeUtilities::getPSSP( dag_node->getValue(), c.getValue(), state_index );
        ModelVector<RealPos> *n = new ModelVector<RealPos>( bl );
        return new RevVariable( n );
    }
    else if ( name == "calculateEDR" )
    {
        found = true;
        std::vector<double> edr = RevBayesCore::TreeUtilities::calculateEDR( dag_node->getValue() );
        ModelVector<RealPos> *n = new ModelVector<RealPos>( edr );
        return new RevVariable( n );
    }
    else if ( name == "getInverseES" )
    {
        found = true;
        std::vector<double> es = RevBayesCore::TreeUtilities::getInverseES( dag_node->getValue() );
        ModelVector<RealPos> *n = new ModelVector<RealPos>( es );
        return new RevVariable( n );
    }

    return ModelObject<RevBayesCore::Tree>::executeMethod( name, args, found );
}


/** Get Rev type of object */
const std::string& Tree::getClassType(void)
{

    static std::string rev_type = "Tree";

    return rev_type;
}


/** Get class type spec describing type of object */
const TypeSpec& Tree::getClassTypeSpec(void)
{

    static TypeSpec rev_type_spec = TypeSpec( getClassType(), new TypeSpec( RevObject::getClassTypeSpec() ) );

    return rev_type_spec;
}



/** Get type spec */
const TypeSpec& Tree::getTypeSpec( void ) const
{

    static TypeSpec type_spec = getClassTypeSpec();

    return type_spec;
}


/**
 * Initialize the member methods.
 */
void Tree::initMethods( void )
{
    ArgumentRules* isBinaryArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "isBinary", RlBoolean::getClassTypeSpec(), isBinaryArgRules ) );

    ArgumentRules* isInternalArgRules = new ArgumentRules();
    isInternalArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "isInternal", RlBoolean::getClassTypeSpec(), isInternalArgRules ) );

    ArgumentRules* same_topology_arg_rules = new ArgumentRules();
    same_topology_arg_rules->push_back(        new ArgumentRule("tree"    , Tree::getClassTypeSpec(), "The reference tree.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RlBoolean>( "hasSameTopology", this, same_topology_arg_rules ) );
    
    ArgumentRules* nnodesArgRules = new ArgumentRules();
    methods.addFunction( new MemberFunction<Tree, Natural>( "nnodes", this, nnodesArgRules ) );

    ArgumentRules* ntipsArgRules = new ArgumentRules();
    methods.addFunction( new MemberFunction<Tree, Natural>( "ntips", this, ntipsArgRules ) );
    
    ArgumentRules* fitchArgRules = new ArgumentRules();
    fitchArgRules->push_back( new ArgumentRule( "characters", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The character alignment from which to compute the Fitch Score.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, Natural>( "fitchScore", this, fitchArgRules ) );
    
    ArgumentRules* psArgRules = new ArgumentRules();
    psArgRules->push_back( new ArgumentRule( "characters", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The character alignment to use when computing the Parsimoniously Same State Paths (PSSP).", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    psArgRules->push_back( new ArgumentRule( "stateIndex", Natural::getClassTypeSpec(), "The state index.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "getPSSP", ModelVector<RealPos>::getClassTypeSpec(), psArgRules ) );
    
    ArgumentRules* edrArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "calculateEDR", ModelVector<RealPos>::getClassTypeSpec(), edrArgRules ) );
    
    ArgumentRules* esArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "getInverseES", ModelVector<RealPos>::getClassTypeSpec(), esArgRules ) );
    
    ArgumentRules* meanInverseESArgRules = new ArgumentRules();
    meanInverseESArgRules->push_back( new ArgumentRule( "characters", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The character alignment from which to compute the mean inverse ES metric.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    meanInverseESArgRules->push_back( new ArgumentRule( "stateIndex", Natural::getClassTypeSpec(), "The state index.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RealPos>( "meanInverseES", this, meanInverseESArgRules ) );
    
    ArgumentRules* nriArgRules = new ArgumentRules();
    nriArgRules->push_back( new ArgumentRule( "characters", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The character alignment from which to compute the Mean Phylogenetic Distance.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    nriArgRules->push_back( new ArgumentRule( "stateIndex", Natural::getClassTypeSpec(), "The index of the character state.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    nriArgRules->push_back( new ArgumentRule( "site", Natural::getClassTypeSpec(), "The index of the site in the alignment.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
    nriArgRules->push_back( new ArgumentRule( "zScore", RlBoolean::getClassTypeSpec(), "Calculate the MPD z-score or the observed MPD?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
    nriArgRules->push_back( new ArgumentRule( "useBranchLengths", RlBoolean::getClassTypeSpec(), "Should MPD use branch length or nodal distances?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
    nriArgRules->push_back( new ArgumentRule( "randomizations", Natural::getClassTypeSpec(),  "How many randomizations should be performed when calculating z-score?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1000) ) );
    methods.addFunction( new MemberFunction<Tree, Real>( "calculateMPD", this, nriArgRules ) );
    
    ArgumentRules* ntiArgRules = new ArgumentRules();
    ntiArgRules->push_back( new ArgumentRule( "characters", AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The character alignment from which to compute the Mean Nearest Taxon Distance.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    ntiArgRules->push_back( new ArgumentRule( "stateIndex", Natural::getClassTypeSpec(), "The index of the character state.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    ntiArgRules->push_back( new ArgumentRule( "site", Natural::getClassTypeSpec(), "The index of the site in the alignment.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1) ) );
    ntiArgRules->push_back( new ArgumentRule( "zScore", RlBoolean::getClassTypeSpec(), "Calculate the MNTD z-score or the observed MNTD?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
    ntiArgRules->push_back( new ArgumentRule( "useBranchLengths", RlBoolean::getClassTypeSpec(), "Should MNTD use branch length or nodal distances?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlBoolean(true) ) );
    ntiArgRules->push_back( new ArgumentRule( "randomizations", Natural::getClassTypeSpec(),  "How many randomizations should be performed when calculating z-score?", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(1000) ) );
    methods.addFunction( new MemberFunction<Tree, Real>( "calculateMNTD", this, ntiArgRules ) );

    ArgumentRules* namesArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "names", ModelVector<RlString>::getClassTypeSpec(), namesArgRules ) );

    ArgumentRules* taxaArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "taxa", ModelVector<Taxon>::getClassTypeSpec(), taxaArgRules ) );
    
    ArgumentRules* setBranchLengthArgRules         = new ArgumentRules();
    setBranchLengthArgRules->push_back(        new ArgumentRule("index"    , Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    setBranchLengthArgRules->push_back(        new ArgumentRule("value"        , RealPos::getClassTypeSpec(), "The new branch length value.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "setBranchLength", RlUtils::Void, setBranchLengthArgRules ) );

    ArgumentRules* setTaxonNameArgRules         = new ArgumentRules();
    setTaxonNameArgRules->push_back(        new ArgumentRule("current"    , RlString::getClassTypeSpec(), "The old name.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    setTaxonNameArgRules->push_back(        new ArgumentRule("new"        , RlString::getClassTypeSpec(), "The new name.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "setTaxonName", RlUtils::Void, setTaxonNameArgRules ) );

    ArgumentRules* nodeNameArgRules = new ArgumentRules();
    nodeNameArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "nodeName", RlString::getClassTypeSpec(),  nodeNameArgRules ) );
    
    ArgumentRules* tip_index_arg_rules = new ArgumentRules();
    std::vector<TypeSpec> tip_index_arg_types;
    tip_index_arg_types.push_back( RlString::getClassTypeSpec() );
    tip_index_arg_types.push_back( Taxon::getClassTypeSpec() );
    tip_index_arg_rules->push_back( new ArgumentRule( "name", tip_index_arg_types, "The name of the tip/taxon.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "tipIndex", Natural::getClassTypeSpec(),  tip_index_arg_rules ) );

    ArgumentRules* drop_tip_arg_rules = new ArgumentRules();
    std::vector<TypeSpec> tip_types;
    tip_types.push_back( RlString::getClassTypeSpec() );
    tip_types.push_back( Taxon::getClassTypeSpec() );
    drop_tip_arg_rules->push_back( new ArgumentRule( "node", tip_types, "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "dropTip", RlUtils::Void,  drop_tip_arg_rules ) );


    ArgumentRules* rescaleArgRules = new ArgumentRules();
    rescaleArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), "The scaling factor.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "rescale", RlUtils::Void, rescaleArgRules ) );
    
    
    ArgumentRules* remove_duplicate_taxa_arg_rules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "removeDuplicateTaxa", RlUtils::Void, remove_duplicate_taxa_arg_rules ) );
    

    ArgumentRules* offsetArgRules = new ArgumentRules();
    offsetArgRules->push_back( new ArgumentRule( "factor", RealPos::getClassTypeSpec(), "The offset factor.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "offset", RlUtils::Void, offsetArgRules ) );

    ArgumentRules* setNegativeConstraint = new ArgumentRules();
    setNegativeConstraint->push_back( new ArgumentRule( "flag", RlBoolean::getClassTypeSpec(), "Is the tree a negative constraint?.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "setNegativeConstraint", RlUtils::Void, setNegativeConstraint ) );

    ArgumentRules* makeUltraArgRules = new ArgumentRules();
    methods.addFunction( new MemberProcedure( "makeUltrametric", RlUtils::Void, makeUltraArgRules ) );

    ArgumentRules* get_clade_arg_rules = new ArgumentRules();
    get_clade_arg_rules->push_back( new ArgumentRule( "clade", ModelVector<Taxon>::getClassTypeSpec(), "Vector of some of the taxa included in the clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberProcedure( "getClade", Clade::getClassTypeSpec(), get_clade_arg_rules ) );


    // member functions
    ArgumentRules* parentArgRules = new ArgumentRules();
    parentArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, Natural>( "parent", this, parentArgRules   ) );

    ArgumentRules* childArgRules = new ArgumentRules();
    childArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    childArgRules->push_back( new ArgumentRule( "index", Natural::getClassTypeSpec(), "The index of the child of this node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, Natural>( "child", this, childArgRules   ) );

    ArgumentRules* branchLengthArgRules = new ArgumentRules();
    branchLengthArgRules->push_back( new ArgumentRule( "node", Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RealPos>( "branchLength", this, branchLengthArgRules   ) );

    ArgumentRules* contained_in_clade_arg_rules = new ArgumentRules();
    contained_in_clade_arg_rules->push_back( new ArgumentRule( "node" , Natural::getClassTypeSpec(), "The index of the node.", ArgumentRule::BY_CONSTANT_REFERENCE, ArgumentRule::ANY ) );
    contained_in_clade_arg_rules->push_back( new ArgumentRule( "clade", Clade::getClassTypeSpec()  , "The embracing clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RlBoolean>( "isContainedInClade", this, contained_in_clade_arg_rules ) );

    ArgumentRules* contains_clade_arg_rules = new ArgumentRules();
    contains_clade_arg_rules->push_back( new ArgumentRule( "clade", Clade::getClassTypeSpec()  , "The embracing clade.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
    methods.addFunction( new MemberFunction<Tree, RlBoolean>( "containsClade", this, contains_clade_arg_rules ) );

    ArgumentRules* treeLengthArgRules = new ArgumentRules();
    methods.addFunction( new MemberFunction<Tree, RealPos>( "treeLength", this, treeLengthArgRules   ) );


}
