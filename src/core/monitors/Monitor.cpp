

#include "DagNode.h"
#include "Monitor.h"
#include "RbException.h"
#include "StringUtilities.h"

#include <algorithm>

using namespace RevBayesCore;



Monitor::Monitor(unsigned long g) :
    printgen( g ),
    model( NULL )
{
    
}

Monitor::Monitor(unsigned long g, DagNode *n) :
    printgen( g ),
    model( NULL )
{
    
    nodes.push_back( n );
    
    // add myself to the set of monitors
    n->addMonitor( this );
    
    // tell the node that we have a reference to it (avoids deletion)
    n->incrementReferenceCount();
}


Monitor::Monitor(unsigned long g, const std::vector<DagNode *> &n) :
    printgen( g ),
    nodes( n ),
    model( NULL )
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *theNode = *it;
        
        // add myself to the set of monitors
        theNode->addMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        theNode->incrementReferenceCount();
    }
    
    sortNodesByName();
    
}

Monitor::Monitor(const Monitor &m) :
    nodes( m.nodes ),
    model( m.model )
{
    
    // set the print gen
    printgen = m.printgen;
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *theNode = *it;
        
        // add myself to the set of monitors
        theNode->addMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        theNode->incrementReferenceCount();
    }
    
}


Monitor::~Monitor( void )
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *theNode = *it;
        
        // remove myself to the set of monitors
        theNode->removeMonitor( this );
        
        
        // tell the node that we have a reference to it (avoids deletion)
        if ( theNode->decrementReferenceCount() == 0 )
        {
            delete *it;
        }
    }

}


Monitor& Monitor::operator=(const Monitor &i)
{
    
    // check for self-assignment
    if ( &i != this )
    {
        
        for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            DagNode *theNode = *it;
            
            // remove myself to the set of monitors
            theNode->removeMonitor( this );

            // tell the node that we have a reference to it (avoids deletion)
            if ( theNode->decrementReferenceCount() == 0 )
            {
                delete *it;
            }
        }
        
        // set the nodes (we don't own them)
        nodes = i.nodes;
        
        for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
        {
            
            DagNode *theNode = *it;
            
            // add myself to the set of monitors
            theNode->addMonitor( this );

            // tell the node that we have a reference to it (avoids deletion)
            theNode->incrementReferenceCount();
        }
        
        // set the printgen
        printgen = i.printgen;
        
        model = i.model;
    }
    
    return *this;
}


void Monitor::addVariable(DagNode *n)
{
        
    nodes.push_back( n );
    
    // add myself to the set of monitors
    n->addMonitor( this );
        
    // tell the node that we have a reference to it (avoids deletion)
    n->incrementReferenceCount();
    
    // now we need to sort again
    sortNodesByName();
    
}

void Monitor::closeStream(void)
{
    ; // dummy fn
}

void Monitor::openStream(void)
{
    ; // dummy fn
}

void Monitor::printHeader(void)
{
    ; // dummy fn
}


const std::vector<DagNode*>& Monitor::getDagNodes( void ) const
{
    
    return nodes;
}


void Monitor::removeVariable(DagNode *n)
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        if ( *it == n )
        {
            // move myself to the set of monitors
            n->removeMonitor( this );
            
            // tell the node that we have a reference to it (avoids deletion)
            if ( n->decrementReferenceCount() == 0 )
            {
                delete *it;
            }
            
            nodes.erase( it );
        }
    }
    
    // now we need to sort again
    sortNodesByName();
    
}


//void Monitor::setDagNodes( const std::set<DagNode *> &args)
//{
//    
//    for (std::set<DagNode*>::iterator it = args.begin(); it != args.end(); it++)
//    {
//        nodes.push_back(*it);
//    }
//    
//    sortNodesByName();
//}

void Monitor::setDagNodes( const std::vector<DagNode *> &args)
{
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        DagNode *theNode = *it;
        
        // remove myself to the set of monitors
        theNode->removeMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        if ( theNode->decrementReferenceCount() == 0 )
        {
            delete *it;
        }
    }
    
    // set the nodes (we don't own them)
    nodes = args;
    
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        
        DagNode *theNode = *it;
        
        // add myself to the set of monitors
        theNode->addMonitor( this );
        
        // tell the node that we have a reference to it (avoids deletion)
        theNode->incrementReferenceCount();
    }
    
    sortNodesByName();

}

void Monitor::setModel(Model *m)
{
    model = m;
    
}

void Monitor::setMcmc(Mcmc *m)
{
    mcmc = m;
}


/**
 * This is the dummy method for the setting of the replicate index.
 * Overwrite this method if necessary.
 */
void Monitor::setReplicateIndex(size_t idx)
{
    // nothing to do here
}


/**
 * This is the dummy method for the setting of the stone index.
 * Overwrite this method if necessary.
 */
void Monitor::setStoneIndex(size_t idx)
{
    // nothing to do here
}


/**
 * Sort the nodes by name so that the order is guaranteed of replicated runs.
 */
void Monitor::sortNodesByName( void )
{
    
    std::vector<std::string> names;
    std::map<std::string,DagNode*> nodesMap;
    for (std::vector<DagNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        DagNode *node = *it;
        std::string name = node->getName();
        StringUtilities::toLower(name);
        nodesMap.insert( std::pair<std::string,DagNode*>(name,node) );
        names.push_back(name);
    }
    
    nodes.clear();
    std::sort (names.begin(), names.end());
    
    for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); ++it)
    {
        DagNode *node = nodesMap[*it];
        nodes.push_back( node );
    }
    
}


void Monitor::swapNode(DagNode *oldN, DagNode *newN) 
{
    // error catching
    std::vector<DagNode*>::iterator it = find(nodes.begin(), nodes.end(), oldN);

    if (it == nodes.end())
    {
        throw RbException("Cannot replace DAG node with name\"" + oldN->getName() + "\" in this monitor because the monitor doesn't hold this DAG node.");
    }
    
    // remove myself from the old node and add myself to the new node
    oldN->removeMonitor( this );
    newN->addMonitor( this );
    
    // increment and decrement the reference counts
    newN->incrementReferenceCount();
    if ( oldN->decrementReferenceCount() == 0 )
    {
        throw RbException("Memory leak in monitor. Please report this bug to Sebastian.");
    }
    
    it = nodes.insert( it, newN );
    nodes.erase( it + 1 );
}


void Monitor::reset(size_t numCycles)
{
    // dummy implementation
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const Monitor& x)
{
    o << "Monitor";
    
    return o;
}

