#include "Clade.h"
#include "ConstantRateBirthDeathProcessTopology.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbMathCombinatorialFunctions.h"
#include "TopologyNode.h"
#include "Topology.h"

#include <algorithm>
#include <cmath>

using namespace RevBayesCore;

ConstantRateBirthDeathProcessTopology::ConstantRateBirthDeathProcessTopology(const TypedDagNode<double> *org, const TypedDagNode<double> *ra, const TypedDagNode<double> *s, const TypedDagNode<double> *e,
															 const TypedDagNode<double> *r, const std::string& ss, const std::string &cdt,
															 const std::vector<Taxon> &tn, const std::vector<Clade> &c) : BirthDeathProcess( org, ra, r, ss, cdt, tn, c ),
speciation( s ),
extinction( e )
{
	addParameter( speciation );
	addParameter( extinction );
	
	simulateTree();
	logTreeTopologyProb = logProbTreeShape();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
ConstantRateBirthDeathProcessTopology* ConstantRateBirthDeathProcessTopology::clone( void ) const
{
	
	return new ConstantRateBirthDeathProcessTopology( *this );
}



double ConstantRateBirthDeathProcessTopology::lnSpeciationRate(double t) const
{
	
	return log( speciation->getValue() );
}


double ConstantRateBirthDeathProcessTopology::pSurvival(double start, double end) const
{
	
	// compute the rate
	double mu = extinction->getValue();
	double lambda = speciation->getValue();
	double rate = mu - lambda;
	
	// do the integration of int_{t_low}^{t_high} ( mu(s) exp(rate(t,s)) ds )
	// where rate(t,s) = int_{t}^{s} ( mu(x)-lambda(x) dx )
	
	double den = 1.0 + ( exp(-rate*start) * mu / rate ) * ( exp(rate*end) - exp(rate*start) );
	
	return (1.0 / den);
}


ConstantRateBirthDeathProcessTopology::TypeTreeRanking ConstantRateBirthDeathProcessTopology::logNumberRankingsRec(TopologyNode& n /*int n, TypeTree *tree*/) {
	ConstantRateBirthDeathProcessTopology::TypeTreeRanking  rl, rr, res;
	if( n.isTip() ) {
		res.rank = 0.;
		res.leaf = 1;
		return res;
	}
	
	rl = logNumberRankingsRec( n.getChild(0) );
	rr = logNumberRankingsRec( n.getChild(1) );
	res.leaf = rl.leaf+rr.leaf;
	//res.rank = rl.rank+rr.rank+log(2)+logBinomial((unsigned int) (rl.leaf-1), (unsigned int) (res.leaf-2));
	res.rank = rl.rank+rr.rank+log(2) + RbMath::lnChoose((unsigned int) (rl.leaf-1), (unsigned int) (res.leaf-2));

	return res;
}

double ConstantRateBirthDeathProcessTopology::logNumberRankings(TimeTree *tree) {
	ConstantRateBirthDeathProcessTopology::TypeTreeRanking r = logNumberRankingsRec( tree->getRoot() );
	return r.rank;
}

double ConstantRateBirthDeathProcessTopology::logProbTreeShape() {
	return logProbSubTreeShape( value->getRoot() );
}

double ConstantRateBirthDeathProcessTopology::logProbSubTreeShape(TopologyNode& n) {
	ConstantRateBirthDeathProcessTopology::TypeTreeRanking  r = logNumberRankingsRec(n);
//	return r.rank-2*gsl_sf_lnfact(r.leaf-1)-log(r.leaf);
	return r.rank-2*RbMath::lnFactorial(r.leaf-1)-log(r.leaf);
}



/**
 * Compute the log-transformed probability of the current value under the current parameter values.
 *
 */
double ConstantRateBirthDeathProcessTopology::computeLnProbability( void )
{
	// prepare probability computation
	prepareProbComputation();
	
	// variable declarations and initialization
	double lnProbTimes = 0;
	
	// first check if the current tree matches the clade constraints
	if ( !matchesConstraints() )
	{
		return RbConstants::Double::neginf;
	}
	
	// present time
	double ra = value->getRoot().getAge();
	double presentTime = 0.0;
	
	// test that the time of the process is larger or equal to the present time
	if ( startsAtRoot == false )
	{
		double org = origin->getValue();
		
		// test that the time of the process is larger or equal to the present time
		if ( ra > org )
		{
			return RbConstants::Double::neginf;
		}
		
		presentTime = org;
		
	}
	else
	{
		presentTime = ra;
		
		if ( ra != rootAge->getValue() )
		{
			return RbConstants::Double::neginf;
		}
		
		const std::vector<TopologyNode*> &c = value->getRoot().getChildren();
		
		for (std::vector<TopologyNode*>::const_iterator it = c.begin(); it != c.end(); ++it)
		{
			if ( ra < (*it)->getAge() )
			{
				return RbConstants::Double::neginf;
			}
		}
	}
	
	const std::vector<TopologyNode*>& nodes = value->getNodes();
	for (std::vector<TopologyNode*>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
	{
		if ( !(*it)->isRoot() )
		{
			if ( (*it)->getAge() >= (*it)->getParent().getAge() )
			{
				return RbConstants::Double::neginf;
			}
		}
	}
	
	
	// what do we condition on?
	// did we condition on survival?
	if ( condition == "survival" )
	{
		lnProbTimes = - log( pSurvival(0,presentTime) );
	}
	
	// if we started at the root then we square the survival prob
	if ( startsAtRoot == true )
	{
		lnProbTimes *= 2.0;
	}
	
	// multiply the probability of a descendant of the initial species
	lnProbTimes += computeLnProbabilityTimes();
	
	// Now we compute the topology part.
	logTreeTopologyProb = logProbTreeShape();
	
	return lnProbTimes + logTreeTopologyProb;
	
}



double ConstantRateBirthDeathProcessTopology::rateIntegral(double t_low, double t_high) const {
	
	double rate = (speciation->getValue() - extinction->getValue()) * (t_low - t_high);
	
	return rate;
}



std::vector<double>* ConstantRateBirthDeathProcessTopology::simSpeciations(size_t n, double origin, double r) const
{
	
	// Get the rng
	RandomNumberGenerator* rng = GLOBAL_RNG;
	
	std::vector<double>* times = new std::vector<double>(n, 0.0);
	
	for (size_t i = 0; i < n; ++i)
	{
		double u = rng->uniform01();
		
		// get the parameters
		double lambda = speciation->getValue()*r;
		double mu = extinction->getValue() - speciation->getValue()*(1.0-r);
		double div = lambda - mu;
		
		double t = 1.0/div * log((lambda - mu * exp((-div)*origin) - mu * (1.0 - exp((-div) * origin)) * u )/(lambda - mu * exp((-div) * origin) - lambda * (1.0 - exp(( -div ) * origin)) * u ) );
		
		(*times)[i] = t;
	}
	
	// finally sort the times
	std::sort(times->begin(), times->end());
	
	return times;
}


/** Swap a parameter of the distribution */
void ConstantRateBirthDeathProcessTopology::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
	
	if (oldP == speciation)
	{
		speciation = static_cast<const TypedDagNode<double>* >( newP );
	}
	else if (oldP == extinction)
	{
		extinction = static_cast<const TypedDagNode<double>* >( newP );
	}
	else
	{
		// delegate the super-class
		BirthDeathProcess::swapParameterInternal(oldP, newP);
	}
	
}
