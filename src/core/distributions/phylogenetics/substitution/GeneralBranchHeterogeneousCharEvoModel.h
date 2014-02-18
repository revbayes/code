#ifndef GeneralBranchHeterogeneousCharEvoModel_H
#define GeneralBranchHeterogeneousCharEvoModel_H

#include "AbstractSiteHomogeneousMixtureCharEvoModel.h"
#include "DnaState.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class GeneralBranchHeterogeneousCharEvoModel : public AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType> {
        
    public:
        GeneralBranchHeterogeneousCharEvoModel(const TypedDagNode< treeType > *t, size_t nChars, bool c, size_t nSites);
        GeneralBranchHeterogeneousCharEvoModel(const GeneralBranchHeterogeneousCharEvoModel &n);                                                                                                //!< Copy constructor
        virtual                                            ~GeneralBranchHeterogeneousCharEvoModel(void);                                                                   //!< Virtual destructor
        
        // public member functions
        GeneralBranchHeterogeneousCharEvoModel*             clone(void) const;                                                                          //!< Create an independent clone
        void                                                setClockRate(const TypedDagNode< double > *r);
        void                                                setClockRate(const TypedDagNode< std::vector< double > > *r);
        void                                                setRateMatrix(const TypedDagNode< RateMatrix > *rm);
        void                                                setRateMatrix(const TypedDagNode< RbVector< RateMatrix > > *rm);
        void                                                setRootFrequencies(const TypedDagNode< std::vector< double > > *f);
        void                                                setSiteRates(const TypedDagNode< std::vector< double > > *r);
        void                                                swapParameter(const DagNode *oldP, const DagNode *newP);                                    //!< Implementation of swaping parameters
        
    protected:
        const std::vector<double>&                          getRootFrequencies(void);
// (not needed)        void                                                keepSpecialization(DagNode* affecter);
// (not needed)        void                                                restoreSpecialization(DagNode *restorer);
        void                                                touchSpecialization(DagNode *toucher);
        void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);
        
        
    private:        
        // members
        const TypedDagNode< double >*                       homogeneousClockRate;
        const TypedDagNode< std::vector< double > >*        heterogeneousClockRates;
        const TypedDagNode< RateMatrix >*                   homogeneousRateMatrix;
        const TypedDagNode< RbVector< RateMatrix > >*       heterogeneousRateMatrices;
        const TypedDagNode< std::vector< double > >*        rootFrequencies;
        const TypedDagNode< std::vector< double > >*        siteRates;
        
        
        // flags specifying which model variants we use
        bool                                                branchHeterogeneousClockRates;
        bool                                                branchHeterogeneousSubstitutionMatrices;
        bool                                                rateVariationAcrossSites;
    };
    
}


#include "ConstantNode.h"
#include "DiscreteCharacterState.h"
#include "RateMatrix_JC.h"
#include "RandomNumberFactory.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>

template<class charType, class treeType>
RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::GeneralBranchHeterogeneousCharEvoModel(const TypedDagNode<treeType> *t, size_t nChars, bool c, size_t nSites) : AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>(  t, nChars, 1, c, nSites ) {
    
    // initialize with default parameters
    homogeneousClockRate        = new ConstantNode<double>("clockRate", new double(1.0) );
    heterogeneousClockRates     = NULL;
    homogeneousRateMatrix       = new ConstantNode<RateMatrix>("rateMatrix", new RateMatrix_JC(charType().getNumberOfStates()) );
    heterogeneousRateMatrices   = NULL;
    rootFrequencies             = NULL;
    siteRates                   = NULL;
    
    
    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = false;
    branchHeterogeneousSubstitutionMatrices     = false;
    rateVariationAcrossSites                    = false;
    
    // add the parameters to the parents list
    this->addParameter( homogeneousClockRate );
    this->addParameter( homogeneousRateMatrix );
    
    this->redrawValue();

}


template<class charType, class treeType>
RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::GeneralBranchHeterogeneousCharEvoModel(const GeneralBranchHeterogeneousCharEvoModel &d) : AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>( d ) {
    // parameters are automatically copied
    // initialize with default parameters
    homogeneousClockRate        = d.homogeneousClockRate;
    heterogeneousClockRates     = d.heterogeneousClockRates;
    homogeneousRateMatrix       = d.homogeneousRateMatrix;
    heterogeneousRateMatrices   = d.heterogeneousRateMatrices;
    rootFrequencies             = d.rootFrequencies;
    siteRates                   = d.siteRates;
    
    
    // flags specifying which model variants we use
    branchHeterogeneousClockRates               = d.branchHeterogeneousClockRates;
    branchHeterogeneousSubstitutionMatrices     = d.branchHeterogeneousSubstitutionMatrices;
    rateVariationAcrossSites                    = d.rateVariationAcrossSites;

}


template<class charType, class treeType>
RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::~GeneralBranchHeterogeneousCharEvoModel( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
    
}


template<class charType, class treeType>
RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>* RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::clone( void ) const {
    
    return new GeneralBranchHeterogeneousCharEvoModel<charType, treeType>( *this );
}


template<class charType, class treeType>
const std::vector<double>& RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::getRootFrequencies( void ) {
    
    if ( branchHeterogeneousSubstitutionMatrices || rootFrequencies != NULL ) 
    {
        return rootFrequencies->getValue();
    } 
    else 
    {
        return homogeneousRateMatrix->getValue().getStationaryFrequencies();
    }

}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen) {
    
    // first, get the rate matrix for this branch
    const RateMatrix *rm;
    if ( this->branchHeterogeneousSubstitutionMatrices == true ) 
    {
        rm = &this->heterogeneousRateMatrices->getValue()[nodeIdx];
    } 
    else 
    {
        rm = &this->homogeneousRateMatrix->getValue();
    }
    
    // second, get the clock rate for the branch
    double branchTime;
    if ( this->branchHeterogeneousClockRates == true ) 
    {
        branchTime = this->heterogeneousClockRates->getValue()[nodeIdx] * brlen;
    } 
    else 
    {
        branchTime = this->homogeneousClockRate->getValue() * brlen;
    }
    
    // and finally compute the per site rate transition probability matrix
    if ( this->rateVariationAcrossSites == true ) 
    {
        const std::vector<double> &r = this->siteRates->getValue();
        for (size_t i = 0; i < this->numSiteRates; ++i)
        {
            rm->calculateTransitionProbabilities( branchTime * r[i], this->transitionProbMatrices[i] );
        }
    } 
    else 
    {
        rm->calculateTransitionProbabilities( branchTime, this->transitionProbMatrices[0] );
    }
    
}



template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setClockRate(const TypedDagNode< double > *r) {
    
    // remove the old parameter first
    if ( homogeneousClockRate != NULL ) 
    {
        this->removeParameter( homogeneousClockRate );
        homogeneousClockRate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneousClockRates );
        heterogeneousClockRates = NULL;
    }
    
    // set the value
    branchHeterogeneousClockRates = false;
    homogeneousClockRate = r;
    
    // add the parameter
    this->addParameter( homogeneousClockRate );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
    
}



template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setClockRate(const TypedDagNode< std::vector< double > > *r) {
    
    // remove the old parameter first
    if ( homogeneousClockRate != NULL ) 
    {
        this->removeParameter( homogeneousClockRate );
        homogeneousClockRate = NULL;
    }
    else // heterogeneousClockRate != NULL
    {
        this->removeParameter( heterogeneousClockRates );
        heterogeneousClockRates = NULL;
    }
    
    // set the value
    branchHeterogeneousClockRates = true;
    heterogeneousClockRates = r;
    
    // add the parameter
    this->addParameter( heterogeneousClockRates );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setRateMatrix(const TypedDagNode< RateMatrix > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMatrix != NULL ) 
    {
        this->removeParameter( homogeneousRateMatrix );
        homogeneousRateMatrix = NULL;
    }
    else // heterogeneousRateMatrix != NULL
    {
        this->removeParameter( heterogeneousRateMatrices );
        heterogeneousRateMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = false;
    homogeneousRateMatrix = rm;
    
    // add the parameter
    this->addParameter( homogeneousRateMatrix );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setRateMatrix(const TypedDagNode< RbVector< RateMatrix > > *rm) {
    
    // remove the old parameter first
    if ( homogeneousRateMatrix != NULL ) 
    {
        this->removeParameter( homogeneousRateMatrix );
        homogeneousRateMatrix = NULL;
    }
    else // heterogeneousRateMatrix != NULL
    {
        this->removeParameter( heterogeneousRateMatrices );
        heterogeneousRateMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousSubstitutionMatrices = true;
    heterogeneousRateMatrices = rm;
    
    // add the parameter
    this->addParameter( heterogeneousRateMatrices );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
    
}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setRootFrequencies(const TypedDagNode< std::vector< double > > *f) {
    
    // remove the old parameter first
    if ( rootFrequencies != NULL ) 
    {
        this->removeParameter( rootFrequencies );
        rootFrequencies = NULL;
    }
    
    if ( f != NULL )
    {
        // set the value
//        branchHeterogeneousSubstitutionMatrices = true;
        rootFrequencies = f;
    
        // add the parameter
        this->addParameter( rootFrequencies );
    }
    else
    {
        branchHeterogeneousSubstitutionMatrices = false;
    }
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
}


template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::setSiteRates(const TypedDagNode< std::vector< double > > *r) {
    
    // remove the old parameter first
    if ( siteRates != NULL ) 
    {
        this->removeParameter( siteRates );
        siteRates = NULL;
    }
    
    if ( r != NULL ) 
    {
        // set the value
        rateVariationAcrossSites = true;
        siteRates = r;
        this->numSiteRates = r->getValue().size();
        this->resizeLikelihoodVectors();
    }
    else
    {
        // set the value
        rateVariationAcrossSites = false;
        siteRates = NULL;
        this->numSiteRates = 1;
        this->resizeLikelihoodVectors();
        
    }
    
    // add the parameter
    this->addParameter( siteRates );
    
    // redraw the current value
    if ( this->dagNode != NULL && !this->dagNode->isClamped() ) 
    {
        this->redrawValue();
    }
}




template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::swapParameter(const DagNode *oldP, const DagNode *newP) {
    
    if (oldP == homogeneousClockRate) 
    {
        homogeneousClockRate = static_cast<const TypedDagNode< double >* >( newP );
    }
    else if (oldP == heterogeneousClockRates) 
    {
        heterogeneousClockRates = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else if (oldP == homogeneousRateMatrix) 
    {
        homogeneousRateMatrix = static_cast<const TypedDagNode< RateMatrix >* >( newP );
    }
    else if (oldP == heterogeneousRateMatrices) 
    {
        heterogeneousRateMatrices = static_cast<const TypedDagNode< RbVector< RateMatrix > >* >( newP );
    }
    else if (oldP == rootFrequencies) 
    {
        rootFrequencies = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else if (oldP == siteRates) 
    {
        siteRates = static_cast<const TypedDagNode< std::vector< double > >* >( newP );
    }
    else 
    {
        AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>::swapParameter(oldP,newP);
    }
    
}

template<class charType, class treeType>
void RevBayesCore::GeneralBranchHeterogeneousCharEvoModel<charType, treeType>::touchSpecialization( DagNode* affecter ) {
    
    // if the topology wasn't the culprit for the touch, then we just flag everything as dirty
    if ( affecter == heterogeneousClockRates ) 
    {
        const std::set<size_t> &indices = heterogeneousClockRates->getTouchedElementIndices();
        
        // maybe all of them have been touched or the flags haven't been set properly
        if ( indices.size() == 0 ) 
        {
            // just delegate the call
            AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>::touchSpecialization( affecter );
        } 
        else 
        {
            const std::vector<TopologyNode *> &nodes = this->tau->getValue().getNodes();
            // flag recomputation only for the nodes
            for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it) 
            {
                this->recursivelyFlagNodeDirty( *nodes[*it] );
            } 
        }
    }
    else if ( affecter == heterogeneousRateMatrices )
    {
        
        const std::set<size_t> &indices = heterogeneousRateMatrices->getTouchedElementIndices();
        
        // maybe all of them have been touched or the flags haven't been set properly
        if ( indices.size() == 0 ) 
        {
            // just delegate the call
            AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>::touchSpecialization( affecter );
        } 
        else 
        {
            const std::vector<TopologyNode *> &nodes = this->tau->getValue().getNodes();
            // flag recomputation only for the nodes
            for (std::set<size_t>::iterator it = indices.begin(); it != indices.end(); ++it) 
            {
                this->recursivelyFlagNodeDirty( *nodes[*it] );
            } 
        }
    }
    else if ( affecter == rootFrequencies )
    {
        
        const TopologyNode &root = this->tau->getValue().getRoot();
        this->recursivelyFlagNodeDirty( root );
    }
    else
    {
        AbstractSiteHomogeneousMixtureCharEvoModel<charType, treeType>::touchSpecialization( affecter );
    }
    
}


#endif
