#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "ArgumentRule.h"
#include "RlMatrixReal.h"
#include "MemberProcedure.h"
#include "ModelVector.h"
#include "Natural.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "RlDiscreteTaxonData.h"
#include "RlSimplex.h"


using namespace RevLanguage;

AbstractHomologousDiscreteCharacterData::AbstractHomologousDiscreteCharacterData(void) :
    HomologousCharacterData( NULL ),
    dagNode( NULL )
{
    
    initMethods();

}


AbstractHomologousDiscreteCharacterData::AbstractHomologousDiscreteCharacterData( const RevBayesCore::AbstractHomologousDiscreteCharacterData &d) :
    HomologousCharacterData( NULL ),
    dagNode( new ConstantNode<valueType>("",d.clone()) )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    initMethods();
    
}


AbstractHomologousDiscreteCharacterData::AbstractHomologousDiscreteCharacterData( RevBayesCore::AbstractHomologousDiscreteCharacterData *d) :
    HomologousCharacterData( NULL ),
    dagNode( new ConstantNode<valueType>("",d) )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    initMethods();

}


AbstractHomologousDiscreteCharacterData::AbstractHomologousDiscreteCharacterData( RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData> *d) :
    HomologousCharacterData( NULL ),
    dagNode( d )
{
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
    // set the internal value pointer
    setCharacterDataObject( &this->getDagNode()->getValue() );
    
    initMethods();

}


AbstractHomologousDiscreteCharacterData::AbstractHomologousDiscreteCharacterData(const AbstractHomologousDiscreteCharacterData &d) :
    HomologousCharacterData( d ),
    dagNode( NULL )
{
    
    if ( d.dagNode != NULL )
    {
        
        dagNode = d.dagNode->clone();
        
        // increment the reference count to the value
        dagNode->incrementReferenceCount();
    }
    
}


AbstractHomologousDiscreteCharacterData::~AbstractHomologousDiscreteCharacterData()
{
    
    // free the old value
    if ( dagNode != NULL )
    {
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
    }
}


AbstractHomologousDiscreteCharacterData& AbstractHomologousDiscreteCharacterData::operator=(const AbstractHomologousDiscreteCharacterData &v) {
    
    if ( this != &v )
    {
        // delegate to base class
        HomologousCharacterData::operator=( v );
        
        // free the old value
        if ( dagNode != NULL )
        {
            if ( dagNode->decrementReferenceCount() == 0 )
            {
                delete dagNode;
            }
            
            dagNode = NULL;
        }
        
        // create own copy
        if ( v.dagNode != NULL )
        {
            dagNode = v.dagNode->clone();
            
            // increment the reference count to the value
            dagNode->incrementReferenceCount();
        }
    }
    
    return *this;
}



AbstractHomologousDiscreteCharacterData* AbstractHomologousDiscreteCharacterData::concatenate(const RevObject &d) const
{
    const AbstractHomologousDiscreteCharacterData* tmp = dynamic_cast<const AbstractHomologousDiscreteCharacterData*>( &d );
    if ( tmp != NULL )
    {
        return concatenate( *tmp );
    }
    else
    {
        throw RbException("Cannot add an object of type '" + d.getType() + "' to a character data object.");
    }
}



AbstractHomologousDiscreteCharacterData* AbstractHomologousDiscreteCharacterData::concatenate(const AbstractHomologousDiscreteCharacterData &d) const
{
    AbstractHomologousDiscreteCharacterData* cloneObj = clone();

    // we need to make this a constant DAG node so that we can actually modify the value
    // otherwise the value might be overwritten again, e.g., if this is a deterministic node.
    cloneObj->makeConstantValue();
    
    // now concatenate
    cloneObj->getDagNode()->getValue().concatenate( d.getValue() );
    
    // return the copy
    return cloneObj;
}



AbstractHomologousDiscreteCharacterData* AbstractHomologousDiscreteCharacterData::clone() const
{
    return new AbstractHomologousDiscreteCharacterData( *this );
}


/* Map calls to member methods */
RevPtr<RevVariable> AbstractHomologousDiscreteCharacterData::executeMethod(std::string const &name, const std::vector<Argument> &args, bool &found)
{
    
    RevPtr<RevVariable> retVal = dynamic_cast<RevMemberObject *>( dagNode )->executeMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    
    if ( this->getDagNode() != NULL )
    {
        // set the internal value pointer
        setCharacterDataObject( &this->getDagNode()->getValue() );
    }
    
    retVal = executeCharacterDataMethod(name, args, found);
    
    if ( found == true )
    {
        return retVal;
    }
    else if (name == "computeStateFrequencies")
    {
        found = true;
        
        RevBayesCore::MatrixReal sf = this->dagNode->getValue().computeStateFrequencies();
        
        return new RevVariable( new MatrixReal(sf) );
    }
    else if (name == "setCodonPartition")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        RevBayesCore::AbstractHomologousDiscreteCharacterData &v = dagNode->getValue();
        size_t nChars = v.getNumberOfCharacters();
        
        // e.g. data.setCodonPartition(sites=v(3))
        if ( argument.isType( Natural::getClassTypeSpec() ) )
        {
            size_t n = size_t( static_cast<const Natural&>( argument ).getValue() );
            for (size_t i = 0; i < nChars; i++)
            {
                
                if (i % 3 == (n-1))
                {
                    v.includeCharacter(i);
                }
                else
                {
                    v.excludeCharacter(i);
                }
                
            }
        }
        
        // e.g. data.setCodonPartition(sites=v(1,2))
        else if ( argument.isType( ModelVector<Natural>::getClassTypeSpec() ) )
        {
            const ModelVector<Natural>& x = static_cast<const ModelVector<Natural>&>( argument );
            if (x.size() == 0)
            {
                return NULL;
            }
            
            for (size_t i = 0; i < nChars; i++)
            {
                v.excludeCharacter(i);
            }
            
            for (size_t i = 0; i < x.size(); i++)
            {
                size_t n = x[i];
                for (size_t j = 0; j < nChars; j++)
                {
                    
                    if (j % 3 == (n-1))
                    {
                        v.includeCharacter(j);
                    }
                    
                }
            }
        }
        return NULL;
    }
    else if (name == "setNumStatesPartition")
    {
        found = true;
        
        const RevObject& argument = args[0].getVariable()->getRevObject();
        RevBayesCore::AbstractHomologousDiscreteCharacterData &v = dagNode->getValue();
        size_t nChars = v.getNumberOfCharacters();
        size_t nTaxa = v.getNumberOfTaxa();
        
        // e.g. data.setNumStatesPartition(2)
        size_t n = size_t( static_cast<const Natural&>( argument ).getValue() );
        for (size_t i = 0; i < nChars; i++)
        {
            int max = 0;
            for (size_t j = 0; j < nTaxa; j++)
            {
                const RevBayesCore::AbstractDiscreteTaxonData& td = v.getTaxonData(j);
                if (!td.getCharacter(i).isAmbiguous() && !td.getCharacter(i).isGapState())
                {
                    int k = int(td.getCharacter(i).getStateIndex()) + 1;
                    if (k > max)
                    {
                        max = k;
                    }
                }
            }

            if (max == n)
            {
                v.includeCharacter(i);
            }
            else
            {
                v.excludeCharacter(i);
            }
        }
        return NULL;
    }
    else if (name == "isHomologous")
    {
        found = true;
        
        bool ih = this->dagNode->getValue().isHomologyEstablished();
        
        return new RevVariable( new RlBoolean(ih) );
    }
    else if (name == "getEmpiricalBaseFrequencies")
    {
        found = true;
        
        std::vector<double> ebf = this->dagNode->getValue().getEmpiricalBaseFrequencies();
        
        return new RevVariable( new Simplex(ebf) );
    }
    else if (name == "getNumInvariantSites")
    {
        found = true;
        
        size_t n = this->dagNode->getValue().getNumberOfInvariantSites();
        
        return new RevVariable( new Natural(n) );
    }
    
    return HomologousCharacterData::executeMethod( name, args, found );
}


/* Get Rev type of object */
const std::string& AbstractHomologousDiscreteCharacterData::getClassType(void)
{
    
    static std::string revType = "AbstractHomologousDiscreteCharacterData";
    
    return revType;
}

/* Get class type spec describing type of object */
const TypeSpec& AbstractHomologousDiscreteCharacterData::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( HomologousCharacterData::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>* AbstractHomologousDiscreteCharacterData::getDagNode( void ) const
{
    
    return dagNode;
}


/** Get the type spec of this class. We return a member variable because instances might have different element types. */
const TypeSpec& AbstractHomologousDiscreteCharacterData::getTypeSpec(void) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    return typeSpec;
}


const RevBayesCore::AbstractHomologousDiscreteCharacterData& AbstractHomologousDiscreteCharacterData::getValue( void ) const
{
    
    if ( dagNode == NULL )
    {
        throw RbException( "Invalid attempt to get value from an object with NULL DAG node" );
    }
    
    return dagNode->getValue();
}


RevBayesCore::AbstractHomologousDiscreteCharacterData& AbstractHomologousDiscreteCharacterData::getValue( void )
{
    
    if ( dagNode == NULL )
    {
        throw RbException( "Invalid attempt to get value from an object with NULL DAG node" );
    }
    
    return dagNode->getValue();
}


/**
 * Is the object or any of its upstream members or elements
 * modifiable by the user through assignment? We simply ask
 * our DAG node.
 */
bool AbstractHomologousDiscreteCharacterData::isAssignable( void ) const
{
    if ( dagNode == NULL )
    {
        return false;
    }
    
    return dagNode->isAssignable();
}


bool AbstractHomologousDiscreteCharacterData::isConstant( void ) const
{
    
    return dagNode->isConstant();
}


bool AbstractHomologousDiscreteCharacterData::isModelObject( void ) const
{
    
    return true;
}


void AbstractHomologousDiscreteCharacterData::initMethods( void )
{

    // add the DAG node member methods
    // note that this is a sage case because all DAG nodes are member objects
    if ( dagNode != NULL )
    {
        const MethodTable &dagMethods = dynamic_cast<RevMemberObject*>( dagNode )->getMethods();
        methods.insertInheritedMethods( dagMethods );
    }
    
    // insert the character data specific methods
    MethodTable charDataMethods = getCharacterDataMethods();
    methods.insertInheritedMethods( charDataMethods );
    
    ArgumentRules* chartypeArgRules                 = new ArgumentRules();
    ArgumentRules* compStateFreqArgRules            = new ArgumentRules();
    ArgumentRules* ishomologousArgRules             = new ArgumentRules();
    ArgumentRules* empiricalBaseArgRules            = new ArgumentRules();
    ArgumentRules* invSitesArgRules                 = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules        = new ArgumentRules();
    ArgumentRules* setCodonPartitionArgRules2       = new ArgumentRules();
    ArgumentRules* setNumStatesPartitionArgRules    = new ArgumentRules();
    
    setCodonPartitionArgRules->push_back(  new ArgumentRule("", Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    setCodonPartitionArgRules2->push_back( new ArgumentRule("", ModelVector<Natural>::getClassTypeSpec() , ArgumentRule::BY_VALUE) );
    setNumStatesPartitionArgRules->push_back(  new ArgumentRule("", Natural::getClassTypeSpec()              , ArgumentRule::BY_VALUE) );
    
    
    methods.addFunction("chartype",                     new MemberProcedure(RlString::getClassTypeSpec(),   chartypeArgRules                ) );
    methods.addFunction("computeStateFrequencies",      new MemberProcedure(RlString::getClassTypeSpec(),   compStateFreqArgRules           ) );
    methods.addFunction("setCodonPartition",            new MemberProcedure(RlUtils::Void,                  setCodonPartitionArgRules       ) );
    methods.addFunction("setCodonPartition",            new MemberProcedure(RlUtils::Void,                  setCodonPartitionArgRules2      ) );
    methods.addFunction("setNumStatesPartition",        new MemberProcedure(RlUtils::Void,                  setNumStatesPartitionArgRules   ) );
    methods.addFunction("isHomologous",                 new MemberProcedure(RlBoolean::getClassTypeSpec(),  ishomologousArgRules            ) );
    methods.addFunction("getEmpiricalBaseFrequencies",  new MemberProcedure(Simplex::getClassTypeSpec(),    empiricalBaseArgRules           ) );
    methods.addFunction("getNumInvariantSites",         new MemberProcedure(Natural::getClassTypeSpec(),    invSitesArgRules                ) );
}


void AbstractHomologousDiscreteCharacterData::makeConstantValue( void )
{
    
    if ( dagNode == NULL )
    {
        throw RbException("Cannot convert a variable without value to a constant value.");
    }
    else if ( dagNode->getDagNodeType() != RevBayesCore::DagNode::CONSTANT )
    {
        RevBayesCore::ConstantNode<valueType>* newNode = new ConstantNode<valueType>(dagNode->getName(), RevBayesCore::Cloner<valueType, IsDerivedFrom<valueType, RevBayesCore::Cloneable>::Is >::createClone( dagNode->getValue() ) );
        dagNode->replace(newNode);
        
        // delete the value if there are no other references to it.
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
        
        dagNode = newNode;
        
        // increment the reference counter
        dagNode->incrementReferenceCount();
    }
    
}


/**
 * Make an indirect reference to the variable. This is appropriate for the contexts
 * where the object occurs on the righ-hand side of expressions like a := b
 */
AbstractHomologousDiscreteCharacterData* AbstractHomologousDiscreteCharacterData::makeIndirectReference(void)
{
    
    RevBayesCore::IndirectReferenceFunction< valueType > *func = new RevBayesCore::IndirectReferenceFunction<valueType>( this->getDagNode() );
    RevBayesCore::DeterministicNode< valueType >* newNode = new RevBayesCore::DeterministicNode< valueType >( "", func );
    
    AbstractHomologousDiscreteCharacterData* newObj = this->clone();
    
    const std::set<RevBayesCore::Move*>& mvs = newObj->getDagNode()->getMoves();
    while ( !mvs.empty() )
    {
        newObj->getDagNode()->removeMove( *mvs.begin() );
    }
    
    newObj->setDagNode( newNode );
    
    return newObj;
}


/** Convert a model object to a deterministic object, the value of which is determined by a user-defined Rev function */
void AbstractHomologousDiscreteCharacterData::makeUserFunctionValue( UserFunction* fxn )
{
    UserFunctionNode< AbstractHomologousDiscreteCharacterData >*  detNode = new UserFunctionNode< AbstractHomologousDiscreteCharacterData >( "", fxn );
    
    // Signal replacement and delete the value if there are no other references to it.
    if ( dagNode != NULL )
    {
        dagNode->replace( detNode );
        if ( dagNode->decrementReferenceCount() == 0 )
            delete dagNode;
    }
    
    // Shift the actual node
    dagNode = detNode;
    
    // Increment the reference counter
    dagNode->incrementReferenceCount();
}


/**
 * Print value for user. The DAG node pointer may be NULL, in which
 * case we print "NA".
 */
void AbstractHomologousDiscreteCharacterData::printValue(std::ostream &o) const
{
    if ( dagNode == NULL )
    {
        o << "NA";
    }
    else
    {
        dagNode->printValue( o );
    }
    
}


/** Copy name of variable onto DAG node, if it is not NULL */
void AbstractHomologousDiscreteCharacterData::setName(std::string const &n)
{
    if ( dagNode != NULL )
    {
        dagNode->setName( n );
    }
    
}


/**
 * Set dag node. We also accommodate the possibility of setting the DAG node to null.
 */
void AbstractHomologousDiscreteCharacterData::setDagNode(RevBayesCore::DagNode* newNode)
{
    
    // Take care of the old value node
    if ( dagNode != NULL )
    {
        if ( newNode != NULL )
        {
            newNode->setName( dagNode->getName() );
        }
        
        dagNode->replace(newNode);
        
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
        
    }
    
    // Set the new value node
    dagNode = static_cast< RevBayesCore::TypedDagNode<valueType>* >( newNode );
    
    // Increment the reference count to the new value node
    if ( dagNode != NULL )
    {
        dagNode->incrementReferenceCount();
    }
    
}


void AbstractHomologousDiscreteCharacterData::setValue(valueType *x)
{
    
    RevBayesCore::ConstantNode<valueType>* newNode;
    
    if ( dagNode == NULL )
    {
        newNode = new ConstantNode<valueType>("",x);
    }
    else
    {
        newNode = new ConstantNode<valueType>(dagNode->getName(),x);
        dagNode->replace(newNode);
        
        if ( dagNode->decrementReferenceCount() == 0 )
        {
            delete dagNode;
        }
        
    }
    
    dagNode = newNode;
    
    // increment the reference count to the value
    dagNode->incrementReferenceCount();
    
}
