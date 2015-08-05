
//
//  FossilSafeSlideMove.cpp


#include "ContinuousStochasticNode.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "FossilSafeSlideMove.h"
#include "StochasticNode.h"
#include "TreeUtilities.h"
#include "TypedDagNode.h"

#include <cmath>
#include <vector>

using namespace RevBayesCore;

FossilSafeSlideMove::FossilSafeSlideMove(std::vector<DagNode*> n, std::vector<double> ta, double l, double w, bool t) : MoveOld( n, w, t ),
    lambda(l),
    tipAges(ta),
    tol(1E-4),
    storedValue(1.0)
{
    tree   = static_cast< StochasticNode<TimeTree>* >( n[0] );
    scaler = static_cast< ContinuousStochasticNode* >( n[1] ) ;
    
    storedTipAges = std::vector<double>( tree->getValue().getNumberOfTips(), 0.0 );
    
    for (size_t i = 0; i < tipAges.size(); i++)
    {
        if (tipAges[i] > tol)
        {
            fossilIdx.push_back(i);
        }
    }
    changed = false;
    
}


FossilSafeSlideMove::~FossilSafeSlideMove( void ) {
    
}


void FossilSafeSlideMove::acceptMove( void ) {
    // nothing to do
    changed = false;

    // do stuff ...
    tree->touch();

}

/* Clone object */
FossilSafeSlideMove* FossilSafeSlideMove::clone( void ) const {

    return new FossilSafeSlideMove( *this );
}

double FossilSafeSlideMove::doMove( void ) {

    // Get random number generator
    RandomNumberGenerator* rng     = GLOBAL_RNG;

    double val = scaler->getValue();
    storedValue = val;

    double min = scaler->getMin();
    double max = scaler->getMax();

    double size = max - min;

    double u      = rng->uniform01();
    double delta  = ( lambda * ( u - 0.5 ) );

    if ( fabs(delta) > 2.0*size )
    {
        delta -= floor(delta / (2.0*size)) * (2.0*size);
    }
    double newVal = val + delta;

    /* reflect the new value */
    do {
        if ( newVal < min )
        {
            newVal = 2.0 * min - newVal;
        }
        else if ( newVal > max )
        {
            newVal = 2.0 * max - newVal;
        }
    } while ( newVal < min || newVal > max );

    val = newVal;
    scaler->setValue( new double(val) );
    
    TimeTree& t = tree->getValue();
//    double rescale = storedValue / val;

    bool failed = false;
    std::vector<TopologyNode*> nodes = t.getNodes();
   
    for (size_t i = 0; i < fossilIdx.size(); i++)
    {
        const size_t nodeIdx = nodes[ fossilIdx[i] ]->getIndex();
        storedTipAges[nodeIdx] = t.getAge(nodeIdx);
        t.setAge( nodeIdx, tipAges[nodeIdx] / val );
        if ( tipAges[ nodeIdx ] > nodes[ nodeIdx ]->getParent().getAge() * val )
        {
            // reject: rescaled tree has negative branch lengths!
            // std::cout << "reject, negative brlen\n";
            failed = true;
        }
    }
    
    if (failed)
        return RbConstants::Double::neginf;
        
	return 0.0;
}

const std::string& FossilSafeSlideMove::getMoveName( void ) const {
    static std::string name = "FossilSafeSlideMove";

    return name;
}

double FossilSafeSlideMove::performMove( double &probRatio ) {
    
    if (changed)
    {
        throw RbException("Trying to execute a simple move twice without accept/reject in the meantime.");
    }
    changed = true;
    
    double hr = doMove();
    
    if ( hr != hr || hr == RbConstants::Double::inf )
    {
        return RbConstants::Double::neginf;
    }
    
    // touch the node
    scaler->touch();
    tree->touch();
    
    // calculate the probability ratio for the node we just changed
    probRatio = scaler->getLnProbabilityRatio();
    probRatio += tree->getLnProbabilityRatio();
    
    if ( probRatio != RbConstants::Double::inf && probRatio != RbConstants::Double::neginf )
    {
        
        std::set<DagNode* > affectedNodes;
        scaler->getAffectedNodes(affectedNodes);
        tree->getAffectedNodes(affectedNodes);
        for (std::set<DagNode* >::iterator i=affectedNodes.begin(); i!=affectedNodes.end(); ++i)
        {
            DagNode* theAffectedNode = *i;
            if (theAffectedNode != tree && theAffectedNode != scaler) {
            //std::cout << theAffectedNode->getName() << "  " << theAffectedNode->getLnProbabilityRatio() << " " << theAffectedNode->getLnProbability() << "\n";
                probRatio += theAffectedNode->getLnProbabilityRatio();
            }
        }
    }
    
    return hr;
}


void FossilSafeSlideMove::rejectMove( void )
{
    
    changed = false;
    
    // delegate to the derived class. The derived class needs to restore the value(s).

    // undo the proposal
//    double rescale = scaler->getValue() / storedValue;
    scaler->setValue( new double(storedValue) );

    TimeTree& t = tree->getValue();
    std::vector<TopologyNode*> nodes = t.getNodes();
    
    for (size_t i = 0; i < fossilIdx.size(); i++)
    {
        const size_t nodeIdx = nodes[ fossilIdx[i] ]->getIndex();
        t.setAge(nodeIdx, storedTipAges[nodeIdx]);
    }

    // touch the node
    tree->touch();
    scaler->touch();
    
}


void FossilSafeSlideMove::swapNode(DagNode *oldN, DagNode *newN) {
    // call the parent method
    MoveOld::swapNode(oldN, newN);
    
    if (oldN == tree)
    {
        tree = static_cast<StochasticNode<TimeTree>* >( newN ) ;
    }
    else if (oldN == scaler)
    {
        scaler = static_cast<ContinuousStochasticNode* >( newN ) ;
    }
}

void FossilSafeSlideMove::tune(void) {

    double rate = numAccepted / double(numTried);

    if ( rate > 0.44 )
    {
        lambda *= (1.0 + ((rate-0.44)/0.56) );
    }
    else
    {
        lambda /= (2.0 - rate/0.44 );
    }

    lambda = fmin(10000, lambda);

}
