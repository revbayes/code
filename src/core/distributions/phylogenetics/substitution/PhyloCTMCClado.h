//
//  PhyloCTMCClado.h
//  revbayes-proj
//
//  Created by Michael Landis on 10/30/14.
//  Copyright (c) 2014 Michael Landis. All rights reserved.
//

#ifndef __revbayes_proj__PhyloCTMCClado__
#define __revbayes_proj__PhyloCTMCClado__

#include "AbstractPhyloCTMCSiteHomogeneous.h"
#include "BiogeographicCladoEvent.h"
#include "RateMatrix.h"
#include "RbVector.h"
#include "MatrixReal.h"
#include "TimeTree.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"
#include "TreeChangeEventListener.h"
#include "TypedDistribution.h"

namespace RevBayesCore {
    
    template<class charType, class treeType>
    class PhyloCTMCClado : public AbstractPhyloCTMCSiteHomogeneous<charType, treeType> {
        
    public:
        PhyloCTMCClado(const TypedDagNode< treeType > *t, size_t nChars, bool c, size_t nSites, bool amb);
        PhyloCTMCClado(const PhyloCTMCClado &n);
        virtual                                            ~PhyloCTMCClado(void);                                                                   //!< Virtual destructor
        
        // public member functions
        PhyloCTMCClado*                                     clone(void) const;                                                                          //!< Create an independent clone
        virtual std::vector<charType>						drawAncestralStatesForNode(const TopologyNode &n);
        virtual void                                        drawJointConditionalAncestralStates(std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates);
        virtual void                                        recursivelyDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates);

        void                                                setCladogenesisMatrix(const TypedDagNode< MatrixReal > *r);
        void                                                setCladogenesisMatrix(const TypedDagNode< RbVector< MatrixReal> >* r);
        void                                                setCladogenesisTimes(const TypedDagNode< RbVector< RbVector< double > > >* rm);
        
    protected:
        
        virtual void                                        resizeLikelihoodVectors(void);
        
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r);
        void                                                computeRootLikelihood(size_t root, size_t l, size_t r, size_t m);
		void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r);
		void                                                computeInternalNodeLikelihood(const TopologyNode &n, size_t nIdx, size_t l, size_t r, size_t m);
        void                                                computeTipLikelihood(const TopologyNode &node, size_t nIdx);
        void                                                updateTransitionProbabilities(size_t nodeIdx, double brlen);
        
        virtual void                                        computeMarginalNodeLikelihood(size_t nodeIdx, size_t parentIdx);
        virtual void                                        computeMarginalRootLikelihood();
        virtual std::vector< std::vector< double > >*       sumMarginalLikelihoods(size_t nodeIndex);
        
        virtual void                                        swapParameterInternal(const DagNode *oldP, const DagNode *newP);            //!< Swap a parameter
        
        // the likelihoods
        double*                                             cladoPartialLikelihoods;
		double*												cladoMarginalLikelihoods;

        // offsets for nodes
        size_t                                              cladoActiveLikelihoodOffset;
        size_t                                              cladoNodeOffset;
        size_t                                              cladoMixtureOffset;
        size_t                                              cladoSiteOffset;
        size_t                                              cladoNodeOffsetMarginal;
        size_t                                              cladoSiteOffsetMarginal;

        
    private:
        virtual void                                            simulate(const TopologyNode& node, std::vector< DiscreteTaxonData< charType > > &t, const std::vector<size_t> &perSiteRates);
        virtual double                                          sumRootLikelihood( void );
        
        const TypedDagNode< MatrixReal >*                       homogeneousCladogenesisMatrix;
        const TypedDagNode< RbVector< MatrixReal > >*           heterogeneousCladogenesisMatrices;
        const TypedDagNode< RbVector< RbVector< double > > >*   cladogenesisTimes;
        
        bool useObservedCladogenesis;
        bool useSampledCladogenesis;
        bool branchHeterogeneousCladogenesis;
    };
    
}


#include "ConstantNode.h"
#include "CladogenicStateFunction.h"
#include "DiscreteCharacterState.h"
#include "RateMatrix_JC.h"
#include "RandomNumberFactory.h"
#include "RbVector.h"
#include "TopologyNode.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <cstring>
#include <map>
#include <vector>

template<class charType, class treeType>
RevBayesCore::PhyloCTMCClado<charType, treeType>::PhyloCTMCClado(const TypedDagNode<treeType> *t, size_t nChars, bool c, size_t nSites, bool amb) : AbstractPhyloCTMCSiteHomogeneous<charType, treeType>(  t, nChars, 1, c, nSites, amb ),
    cladoPartialLikelihoods( new double[2*this->numNodes*this->numSiteRates*this->numSites*this->numChars*this->numChars] ),
    cladoMarginalLikelihoods( new double[this->numNodes*this->numSiteRates*this->numSites*this->numChars*this->numChars] ),
    useObservedCladogenesis(false),
    useSampledCladogenesis(false),
    branchHeterogeneousCladogenesis(false)
{
    unsigned numReducedChar = (unsigned)( log( nChars ) / log( 2 ) );
    
    homogeneousCladogenesisMatrix            = new DeterministicNode< MatrixReal >( "cladogenesisMatrix",
                                                   new CladogenicStateFunction( new ConstantNode<RbVector<double> >( "", new RbVector<double>(2, 0.5)),
                                                                                new ConstantNode<RbVector<double> >( "", new RbVector<double>(2, 0.5)),
                                                                                numReducedChar,
                                                                                2)
                                               );
    heterogeneousCladogenesisMatrices        = NULL;
    cladogenesisTimes                        = NULL;
    
    
    // initialize liklihood vectors to 0.0
	for (size_t i = 0; i < 2*this->numNodes*this->numSiteRates*this->numSites*this->numChars*this->numChars; i++)
    {
		cladoPartialLikelihoods[i] = 0.0;
	}
	for (size_t i = 0; i < this->numNodes*this->numSiteRates*this->numSites*this->numChars*this->numChars; i++)
    {
		cladoMarginalLikelihoods[i] = 0.0;
	}
    
    cladoActiveLikelihoodOffset      =  this->numNodes*this->numSiteRates*this->numPatterns*this->numChars*this->numChars;
    cladoNodeOffset                  =                 this->numSiteRates*this->numPatterns*this->numChars*this->numChars;
    cladoMixtureOffset               =                                    this->numPatterns*this->numChars*this->numChars;
    cladoSiteOffset                  =                                                      this->numChars*this->numChars;
    
    
    // add the parameters to our set (in the base class)
    // in that way other class can easily access the set of our parameters
    // this will also ensure that the parameters are not getting deleted before we do
    this->addParameter( homogeneousCladogenesisMatrix );
    this->addParameter( heterogeneousCladogenesisMatrices );
    this->addParameter( cladogenesisTimes );
    
}

template<class charType, class treeType>
RevBayesCore::PhyloCTMCClado<charType, treeType>::PhyloCTMCClado(const PhyloCTMCClado &n) :
    AbstractPhyloCTMCSiteHomogeneous<charType, treeType>( n ),
    cladoPartialLikelihoods( new double[2*this->numNodes*this->numSiteRates*this->numSites*this->numChars*this->numChars] ),
    cladoMarginalLikelihoods( new double[this->numNodes*this->numSiteRates*this->numSites*this->numChars*this->numChars] ),
    useObservedCladogenesis(n.useObservedCladogenesis),
    useSampledCladogenesis(n.useSampledCladogenesis),
    branchHeterogeneousCladogenesis(n.branchHeterogeneousCladogenesis)
{
    // initialize with default parameters
    homogeneousCladogenesisMatrix       = n.homogeneousCladogenesisMatrix;
    heterogeneousCladogenesisMatrices   = n.heterogeneousCladogenesisMatrices;
    cladogenesisTimes                   = n.cladogenesisTimes;
        
    // copy the partial likelihoods
	memcpy(cladoPartialLikelihoods, n.cladoPartialLikelihoods, 2*this->numNodes*this->numSiteRates*this->numPatterns*this->numChars*this->numChars*sizeof(double));
    
    // copy the marginal likelihoods
    memcpy(cladoMarginalLikelihoods, n.cladoMarginalLikelihoods, this->numNodes*this->numSiteRates*this->numPatterns*this->numChars*this->numChars*sizeof(double));
	
    cladoActiveLikelihoodOffset      =  this->numNodes*this->numSiteRates*this->numPatterns*this->numChars*this->numChars;
    cladoNodeOffset                  =                 this->numSiteRates*this->numPatterns*this->numChars*this->numChars;
    cladoMixtureOffset               =                                    this->numPatterns*this->numChars*this->numChars;
    cladoSiteOffset                  =                                                      this->numChars*this->numChars;
}



template<class charType, class treeType>
RevBayesCore::PhyloCTMCClado<charType, treeType>::~PhyloCTMCClado( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!

    delete [] cladoPartialLikelihoods;
    delete [] cladoMarginalLikelihoods;
}


template<class charType, class treeType>
RevBayesCore::PhyloCTMCClado<charType, treeType>* RevBayesCore::PhyloCTMCClado<charType, treeType>::clone( void ) const {
    
    return new PhyloCTMCClado<charType, treeType>( *this );
}



template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::computeRootLikelihood( size_t root, size_t left, size_t right)
{
//    std::cout << "root\n";
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();

    // get cladogenesis values
//    const MatrixReal& cp =
    ( branchHeterogeneousCladogenesis ? heterogeneousCladogenesisMatrices->getValue()[root] : homogeneousCladogenesisMatrix->getValue() );
    
    
    // get cladogenesis event map (sparse transition probability matrix)
    const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( homogeneousCladogenesisMatrix );
    const TypedFunction<MatrixReal>& tf = cpn->getFunction();
    const CladogenicStateFunction* csf = static_cast<const CladogenicStateFunction*>( &tf );
    std::map<std::vector<unsigned>, double> eventMapProbs = csf->getEventMapProbs();
    
    // get the pointers to the partial likelihoods of the left and right subtree
    double* p_node         = this->partialLikelihoods + this->activeLikelihood[root]  * this->activeLikelihoodOffset + root  * this->nodeOffset;
    const double* p_left   = this->partialLikelihoods + this->activeLikelihood[left]  * this->activeLikelihoodOffset + left  * this->nodeOffset;
    const double* p_right  = this->partialLikelihoods + this->activeLikelihood[right] * this->activeLikelihoodOffset + right * this->nodeOffset;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {

        // get the pointers to the likelihood for this mixture category
        size_t offset = mixture*this->mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_right    = p_right + offset;
        
        // compute the per site probabilities
        for (size_t site = 0; site < this->numPatterns ; ++site)
        {
            // first compute clado probs at younger end of branch
            std::map<std::vector<unsigned>, double>::iterator it;
            size_t old_c1 = 0;
            double sum_clado = 0.0;
            for ( it = eventMapProbs.begin(); it != eventMapProbs.end(); ++it)
            {
                // sparse elements from map
                const std::vector<unsigned>& idx = it->first;
                const size_t c1 = idx[0];
                const size_t c2 = idx[1];
                const size_t c3 = idx[2];
                
                // if key's ancestral state changes, update the resp. partial likelihood
                if (c1 != old_c1)
                {
                    p_site_mixture[old_c1] = sum_clado * f[old_c1];
                    sum_clado = 0.0;
                }
                
                const double pl = p_site_mixture_left[c2];
                const double pr = p_site_mixture_right[c3];
                const double pcl = it->second;
                
//                std::cout << c1 << "->" << c2 << "," << c3 << " pl=" << pl << " pr=" << pr << " pcl=" << pcl << "\n";
                
                sum_clado += pl * pr * pcl;
                
                old_c1 = c1;
            }
//            for (size_t k = 0; k < this->numChars; k++)
//            {
//                std::cout << k << ":" << p_site_mixture[k] << " ";
//            }
//            std::cout << "\n";

            // increment the pointers to the next site
            p_site_mixture_left  += this->siteOffset;
            p_site_mixture_right += this->siteOffset;
            p_site_mixture       += this->siteOffset;
        } // end-for over all sites (=patterns)
        
    } // end-for over all mixtures (=rate-categories)    
}


template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::computeRootLikelihood( size_t root, size_t left, size_t right, size_t middle)
{
    computeRootLikelihood(root, left, right);
}


template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right, size_t middle)
{
	computeInternalNodeLikelihood(node, nodeIndex, left, right);
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::computeInternalNodeLikelihood(const TopologyNode &node, size_t nodeIndex, size_t left, size_t right)
{
    
    // get cladogenesis values
//    const MatrixReal& cp =
    ( branchHeterogeneousCladogenesis ? heterogeneousCladogenesisMatrices->getValue()[nodeIndex] : homogeneousCladogenesisMatrix->getValue() );
    
    
    // get cladogenesis event map (sparse transition probability matrix)
    const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( homogeneousCladogenesisMatrix );
    const TypedFunction<MatrixReal>& tf = cpn->getFunction();
    const CladogenicStateFunction* csf = static_cast<const CladogenicStateFunction*>( &tf );
    std::map<std::vector<unsigned>, double> eventMapProbs = csf->getEventMapProbs();

    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );

    // get the pointers to the partial likelihoods for this node and the two descendant subtrees
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    double*         p_clado_node  = this->cladoPartialLikelihoods + this->activeLikelihood[nodeIndex]*this->cladoActiveLikelihoodOffset + nodeIndex*this->cladoNodeOffset;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*    tp_begin                = this->transitionProbMatrices[mixture].theMatrix;
        
        // get the pointers to the likelihood for this mixture category
        size_t offset = mixture*this->mixtureOffset;
        double*          p_site_mixture          = p_node + offset;
        double*          p_clado_site_mixture    = p_clado_node + mixture * this->cladoMixtureOffset;
        const double*    p_site_mixture_left     = p_left + offset;
        const double*    p_site_mixture_right    = p_right + offset;
        
        // compute the per site probabilities
        for (size_t site = 0; site < this->numPatterns ; ++site)
        {
            // get the pointers for this mixture category and this site
            const double*       tp_a    = tp_begin;
            
            // start state at older end of branch
            for (size_t c0 = 0; c0 < this->numChars; ++c0)
            {
                
                // variable to marginalizing over ana+clado events
                double sum_ana = 0.0;
    
                // first compute clado probs at younger end of branch
                std::map<std::vector<unsigned>, double>::iterator it;
                size_t old_c1 = 0;
                double sum_clado = 0.0;
                for ( it = eventMapProbs.begin(); it != eventMapProbs.end(); ++it)
                {
                    // sparse elements from map
                    const std::vector<unsigned>& idx = it->first;
                    const size_t c1 = idx[0];
                    const size_t c2 = idx[1];
                    const size_t c3 = idx[2];
                 
                    // if key's ancestral state changes, update the resp. partial likelihood
                    if (c1 != old_c1)
                    {
                        p_clado_site_mixture[old_c1] = sum_clado;
                        sum_ana += sum_clado * tp_a[old_c1];
                        sum_clado = 0.0;
                    }

                    const double pl = p_site_mixture_left[c2];
                    const double pr = p_site_mixture_right[c3];
                    const double pcl = it->second;
                    sum_clado += pl * pr * pcl;
                    
//                    std::cout << c1 << "->" << c2 << "," << c3 << " pl=" << pl << " pr=" << pr << " pcl=" << pcl << "\n";
                    old_c1 = c1;
                }

                // store the likelihood for this starting state
                p_site_mixture[c0] = sum_ana;
                
                // increment the pointers to the next starting state
                tp_a+=this->numChars;
            }
            
            // increment the pointers to the next site
            p_site_mixture_left  += this->siteOffset;
            p_site_mixture_right += this->siteOffset;
            p_site_mixture       += this->siteOffset;
            p_clado_site_mixture += this->cladoSiteOffset;
            
        } // end-for over all sites (=patterns)
        
    } // end-for over all mixtures (=rate-categories)
}


template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::computeMarginalNodeLikelihood( size_t nodeIndex, size_t parentNodeIndex )
{
    // get cladogenic transition probs
//    const MatrixReal& cp =
    ( branchHeterogeneousCladogenesis ? heterogeneousCladogenesisMatrices->getValue()[nodeIndex] : homogeneousCladogenesisMatrix->getValue() );
    
    
    const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( homogeneousCladogenesisMatrix );
    const TypedFunction<MatrixReal>& tf = cpn->getFunction();
    const CladogenicStateFunction* csf = static_cast<const CladogenicStateFunction*>( &tf );
    std::map<std::vector<unsigned>, double> eventMapProbs = csf->getEventMapProbs();
    
    // compute the transition probability matrix
    this->updateTransitionProbabilities( nodeIndex, this->tau->getValue().getBranchLength(nodeIndex) );
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    const double*   p_node                          = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    const double*   p_parent_node_marginal          = this->marginalLikelihoods + parentNodeIndex*this->nodeOffset;
    double*         p_node_marginal                 = this->marginalLikelihoods + nodeIndex*this->nodeOffset;
    const double*   p_clado_node                    = this->cladoPartialLikelihoods + this->activeLikelihood[nodeIndex]*this->cladoActiveLikelihoodOffset + nodeIndex*this->cladoNodeOffset;
    const double*   p_clado_parent_node_marginal    = this->cladoMarginalLikelihoods + parentNodeIndex*this->cladoNodeOffset;
    double*         p_clado_node_marginal           = this->cladoMarginalLikelihoods + nodeIndex*this->cladoNodeOffset;
   
    
    // get pointers the likelihood for both subtrees
    const double*   p_mixture                       = p_node;
    const double*   p_parent_mixture_marginal       = p_parent_node_marginal;
    double*         p_mixture_marginal              = p_node_marginal;
    const double*   p_clado_mixture                 = p_clado_node;
    const double*   p_clado_parent_mixture_marginal = p_clado_parent_node_marginal;
    double*         p_clado_mixture_marginal        = p_clado_node_marginal;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*    tp_begin                = this->transitionProbMatrices[mixture].theMatrix;
        
        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture                          = p_mixture;
        const double*   p_parent_site_mixture_marginal          = p_parent_mixture_marginal;
        double*         p_site_mixture_marginal                 = p_mixture_marginal;
        const double*   p_clado_site_mixture                    = p_clado_mixture;
        const double*   p_clado_parent_site_mixture_marginal    = p_clado_parent_mixture_marginal;
        double*         p_clado_site_mixture_marginal           = p_clado_mixture_marginal;
        
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // get the pointers to the likelihoods for this site and mixture category
            const double*   p_site_j                    = p_site_mixture;
            double*         p_site_marginal_j           = p_site_mixture_marginal;
    
            // iterate over all end states, after anagenesis
            for (size_t j=0; j<this->numChars; ++j)
            {
                const double*   p_parent_site_marginal_k    = p_parent_site_mixture_marginal;
                *p_site_marginal_j = 0.0;

                // iterator over all start states, before anagenesis
                for (size_t k=0; k<this->numChars; ++k)
                {
                    // transition probability for k->j
                    const double tp_kj = *p_parent_site_marginal_k * tp_begin[ k * this->numChars + j ];
                    
                    // add the probability of starting from this state
                    *p_site_marginal_j += *p_site_j * tp_kj;
                    
                    // next parent state
                    ++p_parent_site_marginal_k;
                }
                
                // increment pointers
                ++p_site_j; ++p_site_marginal_j;
            }
            
            // iterate over all (X_L, X_R) states, after cladogenesis
            std::map< std::vector<unsigned>, double >::iterator it;
            for (it = eventMapProbs.begin(); it != eventMapProbs.end(); it++)
            {
                ;
            }
            
            
            // increment the pointers to the next site
            p_site_mixture                          += this->siteOffset;
            p_site_mixture_marginal                 += this->siteOffset;
            p_parent_site_mixture_marginal          += this->siteOffset;
            p_clado_site_mixture                    += this->cladoSiteOffset;
            p_clado_site_mixture_marginal           += this->cladoSiteOffset;
            p_clado_parent_site_mixture_marginal    += this->cladoSiteOffset;

            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture                           += this->mixtureOffset;
        p_mixture_marginal                  += this->mixtureOffset;
        p_parent_mixture_marginal           += this->mixtureOffset;
        p_clado_mixture                     += this->cladoMixtureOffset;
        p_clado_mixture_marginal            += this->cladoMixtureOffset;
        p_clado_parent_mixture_marginal     += this->cladoMixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
}



template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::computeMarginalRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = this->tau->getValue().getRoot();
    
    // get the index of the root node
    size_t nodeIndex = root.getIndex();
    
    // get the root frequencies
    const std::vector<double> &f                    = this->getRootFrequencies();
    std::vector<double>::const_iterator f_end       = f.end();
    std::vector<double>::const_iterator f_begin     = f.begin();
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    const double*   p_node           = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    double*         p_node_marginal  = this->marginalLikelihoods + nodeIndex*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    const double*   p_mixture           = p_node;
    double*         p_mixture_marginal  = p_node_marginal;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
        const double*   p_site_mixture          = p_mixture;
        double*         p_site_mixture_marginal = p_mixture_marginal;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // get the pointer to the stationary frequencies
            std::vector<double>::const_iterator f_j             = f_begin;
            // get the pointers to the likelihoods for this site and mixture category
            const double*   p_site_j            = p_site_mixture;
            double*         p_site_marginal_j   = p_site_mixture_marginal;
            // iterate over all starting states
            for (; f_j != f_end; ++f_j)
            {
                // add the probability of starting from this state
                *p_site_marginal_j = *p_site_j * *f_j;
                
                // increment pointers
                ++p_site_j; ++p_site_marginal_j;
            }
            
            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset; p_site_mixture_marginal+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset; p_mixture_marginal+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
}



template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::computeTipLikelihood(const TopologyNode &node, size_t nodeIndex)
{
    
    double* p_node = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    
    const std::vector<bool> &gap_node = this->gapMatrix[nodeIndex];
    const std::vector<unsigned long> &char_node = this->charMatrix[nodeIndex];
    
    
    // compute the transition probabilities
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    
    double*   p_mixture      = p_node;
    
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        // the transition probability matrix for this mixture category
        const double*                       tp_begin    = this->transitionProbMatrices[mixture].theMatrix;
        
        // get the pointer to the likelihoods for this site and mixture category
        double*     p_site_mixture      = p_mixture;
        
        // iterate over all sites
        for (size_t site = 0; site != this->numPatterns; ++site)
        {
            
            // is this site a gap?
            if ( gap_node[site] )
            {
                // since this is a gap we need to assume that the actual state could have been any state
                
                // iterate over all initial states for the transitions
                for (size_t c1 = 0; c1 < this->numChars; ++c1)
                {
                    
                    // store the likelihood
                    p_site_mixture[c1] = 1.0;
                    
                }
            }
            else // we have observed a character
            {
                
                // get the original character
                unsigned long org_val = char_node[site];
                
                // iterate over all possible initial states
                for (size_t c1 = 0; c1 < this->numChars; ++c1)
                {
                    
                    if ( this->usingAmbiguousCharacters )
                    {
                        // compute the likelihood that we had a transition from state c1 to the observed state org_val
                        // note, the observed state could be ambiguous!
                        unsigned long val = org_val;
                        
                        // get the pointer to the transition probabilities for the terminal states
                        const double* d  = tp_begin+(this->numChars*c1);
                        
                        double tmp = 0.0;
                        
                        while ( val != 0 ) // there are still observed states left
                        {
                            // check whether we observed this state
                            if ( (val & 1) == 1 )
                            {
                                // add the probability
                                tmp += *d;
                            }
                            
                            // remove this state from the observed states
                            val >>= 1;
                            
                            // increment the pointer to the next transition probability
                            ++d;
                        } // end-while over all observed states for this character
                        
                        // store the likelihood
                        p_site_mixture[c1] = tmp;
                        
                    }
                    else // no ambiguous characters in use
                    {
                        
                        // store the likelihood
                        p_site_mixture[c1] = tp_begin[c1*this->numChars+org_val];
                        
                    }
                    
                } // end-for over all possible initial character for the branch
                
            } // end-if a gap state
            
            // increment the pointers to next site
            p_site_mixture+=this->siteOffset;
            
        } // end-for over all sites/patterns in the sequence
        
        // increment the pointers to next mixture category
        p_mixture+=this->mixtureOffset;
        
    } // end-for over all mixture categories
    
}


/**
 * Draw a vector of ancestral states from the marginal distribution (non-conditional of the other ancestral states).
 * Here we assume that the marginal likelihoods have been updated.
 */
template<class charType, class treeType>
std::vector<charType> RevBayesCore::PhyloCTMCClado<charType, treeType>::drawAncestralStatesForNode(const TopologyNode &node)
{
	
	size_t nodeIndex = node.getIndex();
	
	// get the marginal likelihoods
    std::vector< std::vector<double> >* marginals = sumMarginalLikelihoods(nodeIndex);
    
	RandomNumberGenerator* rng = GLOBAL_RNG;
	std::vector< charType > ancestralSeq = std::vector<charType>();
	
    for ( size_t i = 0; i < this->numSites; ++i )
    {
		size_t pattern = i;
		// if the matrix is compressed use the pattern for this site
		if (this->compressed) {
			pattern = this->sitePattern[i];
		}
		
        // create the character
        charType c;
        c.setToFirstState();
        
		// sum the likelihoods for each character state
		const std::vector<double> siteMarginals = (*marginals)[pattern];
		double sumMarginals = 0.0;
		for (int j = 0; j < siteMarginals.size(); j++)
        {
			sumMarginals += siteMarginals[j];
		}
		
		double u = rng->uniform01();
		if (sumMarginals == 0.0)
        {
			
			// randomly draw state if all states have 0 probability
			c.setState((size_t)(u*c.getNumberOfStates()));
			
		}
        else
        {
			
			// the marginals don't add up to 1, so rescale u
			u *= sumMarginals;
			
			// draw the character state
			size_t stateIndex = 0;
			while ( true )
            {
				
				u -= siteMarginals[stateIndex];
				
				if ( u > 0.0 )
                {
					
					c++;
					stateIndex++;
					
					if ( stateIndex == c.getNumberOfStates() )
                    {
						stateIndex = 0;
						c.setToFirstState();
					}
					
				}
                else
                {
					break;
				}
			}
		}
		
        // add the character to the sequence
        ancestralSeq.push_back( c );
    }
	
    // we need to free the vector
    delete marginals;
    
	return ancestralSeq;
}


/**
 * Draw a vector of ancestral states from the joint-conditional distribution of states.
 */
template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::drawJointConditionalAncestralStates(std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates)
{
    
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    const TopologyNode &root = this->tau->getValue().getRoot();
    size_t nodeIndex = root.getIndex();
    size_t right = root.getChild(0).getIndex();
    size_t left = root.getChild(1).getIndex();
    
    // get working variables
    const std::vector<double> &f = this->getRootFrequencies();
    std::vector<double> siteProbVector(1,1.0);
    if (this->siteRatesProbs != NULL)
        siteProbVector = this->siteRatesProbs->getValue();
    
    // get cladogenesis values
//    const MatrixReal& cp =
    ( branchHeterogeneousCladogenesis ? heterogeneousCladogenesisMatrices->getValue()[nodeIndex] : homogeneousCladogenesisMatrix->getValue() );
    
    
    // get cladogenesis event map (sparse transition probability matrix)
    const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( homogeneousCladogenesisMatrix );
    const TypedFunction<MatrixReal>& tf = cpn->getFunction();
    const CladogenicStateFunction* csf = static_cast<const CladogenicStateFunction*>( &tf );
    std::map<std::vector<unsigned>, double> eventMapProbs = csf->getEventMapProbs();
    std::map<std::vector<unsigned>, double> sampleProbs;
    std::map<std::vector<unsigned>, double>::iterator it_s;
    std::map<std::vector<unsigned>, double>::iterator it_p;
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    const double*   p_site           = p_node;
    const double*   p_left_site      = p_left;
    const double*   p_right_site     = p_right;
    
    
    // sample root states
//    std::vector<double> p( this->numSiteRates*this->numChars, 0.0);
    std::vector<size_t> sampledSiteRates(this->numSites,0);
    for (size_t i = 0; i < this->numSites; i++)
    {
        // sum to sample
        double sum = 0.0;
        
		// if the matrix is compressed use the pattern for this site
        size_t pattern = i;
		if (this->compressed) {
			pattern = this->sitePattern[i];
		}
        
        // get ptr to first mixture cat for site
        p_site          = p_node  + pattern * this->siteOffset;
        p_left_site     = p_left  + pattern * this->siteOffset;
        p_right_site    = p_right + pattern * this->siteOffset;
        
        // iterate over all mixture categories
        for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
        {
            // get pointers to the likelihood for this mixture category
            const double* p_site_mixture_j       = p_site;
            const double* p_left_site_mixture_j  = p_left_site;
            const double* p_right_site_mixture_j = p_right_site;
            
            // iterate over possible end-anagenesis states for each site given start-anagenesis state
            for (it_p = eventMapProbs.begin(); it_p != eventMapProbs.end(); it_p++)
            {
                // triplet of (A,L,R) states
                const std::vector<unsigned>& v = it_p->first;
                
                p_site_mixture_j       = p_site       + v[0];
                p_left_site_mixture_j  = p_left_site  + v[1];
                p_right_site_mixture_j = p_right_site + v[2];
                
//                size_t k = this->numChars*mixture + v[0];
                std::vector<unsigned> key = it_p->first;
                key.push_back((unsigned)mixture);
                sampleProbs[ key ] = *p_site_mixture_j * *p_left_site_mixture_j * *p_right_site_mixture_j * f[v[0]] * siteProbVector[mixture] * it_p->second;
                sum += sampleProbs[ key ];
                
                // increment the pointers to the next state for (site,rate)
            }
            
            // increment the pointers to the next mixture category for given site
            p_site       += this->mixtureOffset;
            p_left_site  += this->mixtureOffset;
            p_right_site += this->mixtureOffset;
            
        } // end-for over all mixtures (=rate categories)
        
        // sample char from p
        bool stop = false;
        charType ca, cl, cr;
        
        double u = rng->uniform01() * sum;
        
        for (it_s = sampleProbs.begin(); it_s != sampleProbs.end(); it_s++)
        {
            u -= it_s->second;
            if (u < 0.0)
            {
                const std::vector<unsigned>& v = it_s->first;
                ca += v[0];
                cl += v[1];
                cr += v[2];
                endStates[nodeIndex][i] = ca;
                startStates[nodeIndex][i] = ca;
                startStates[left][i] = cl;
                startStates[right][i] = cr;
                sampledSiteRates[i] = v[3];
                stop = true;
                break;
            }
        }
    }
    
    // recurse
    std::vector<TopologyNode*> children = root.getChildren();
    for (size_t i = 0; i < children.size(); i++)
    {        
        // recurse towards tips
        if (!children[i]->isTip())
            recursivelyDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
        else
            AbstractPhyloCTMCSiteHomogeneous<charType,treeType>::tipDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
    }
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::recursivelyDrawJointConditionalAncestralStates(const TopologyNode &node, std::vector<std::vector<charType> >& startStates, std::vector<std::vector<charType> >& endStates, const std::vector<size_t>& sampledSiteRates)
{
    RandomNumberGenerator* rng = GLOBAL_RNG;
    
    // get working variables
    size_t nodeIndex = node.getIndex();
    size_t left = node.getChild(0).getIndex();
    size_t right = node.getChild(1).getIndex();
    //    size_t parentIndex = node.getParent().getIndex();
    
    // get cladogenesis values
//    const MatrixReal& cp =
    ( branchHeterogeneousCladogenesis ? heterogeneousCladogenesisMatrices->getValue()[nodeIndex] : homogeneousCladogenesisMatrix->getValue() );
    
    
    // get cladogenesis event map (sparse transition probability matrix)
    const DeterministicNode<MatrixReal>* cpn = static_cast<const DeterministicNode<MatrixReal>* >( homogeneousCladogenesisMatrix );
    const TypedFunction<MatrixReal>& tf = cpn->getFunction();
    const CladogenicStateFunction* csf = static_cast<const CladogenicStateFunction*>( &tf );
    std::map<std::vector<unsigned>, double> eventMapProbs = csf->getEventMapProbs();
    std::map<std::vector<unsigned>, double> sampleProbs;
    std::map<std::vector<unsigned>, double>::iterator it_s;
    std::map<std::vector<unsigned>, double>::iterator it_p;
    
    // get transition probabilities
    this->updateTransitionProbabilities( nodeIndex, node.getBranchLength() );
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    //    double*         p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex]*this->activeLikelihoodOffset + nodeIndex*this->nodeOffset;
    const double*   p_left  = this->partialLikelihoods + this->activeLikelihood[left]*this->activeLikelihoodOffset + left*this->nodeOffset;
    const double*   p_right = this->partialLikelihoods + this->activeLikelihood[right]*this->activeLikelihoodOffset + right*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    //    const double*   p_site           = p_node;
    
    // sample characters conditioned on start states, going to end states
    std::vector<double> p(this->numChars, 0.0);
    for (size_t i = 0; i < this->numSites; i++)
    {
        size_t cat = sampledSiteRates[i];
        size_t k = startStates[nodeIndex][i].getStateIndex();
        
        // sum to sample
        double sum = 0.0;
        
		// if the matrix is compressed use the pattern for this site
        size_t pattern = i;
		if (this->compressed) {
			pattern = this->sitePattern[i];
		}

        const double* p_left_site_mixture  = p_left  + cat * this->mixtureOffset + pattern * this->siteOffset;
        const double* p_right_site_mixture = p_right + cat * this->mixtureOffset + pattern * this->siteOffset;
        
        // iterate over possible end-anagenesis states for each site given start-anagenesis state        
        for (it_p = eventMapProbs.begin(); it_p != eventMapProbs.end(); it_p++)
        {
            // triplet of (A,L,R) states
            const std::vector<unsigned>& v = it_p->first;
            
            const double* p_left_site_mixture_j  = p_left_site_mixture  + v[1];
            const double* p_right_site_mixture_j = p_right_site_mixture + v[2];
            
            // anagenesis prob
            size_t j = v[0];
            double tp_kj = this->transitionProbMatrices[cat][k][j];
            
            // anagenesis + cladogenesis prob
            sampleProbs[ it_p->first ] = it_p->second * tp_kj * *p_left_site_mixture_j * *p_right_site_mixture_j;
            sum += sampleProbs[ it_p->first ];
            
        }
        
        // sample char from p
        charType ca, cl, cr;
        ca.setToFirstState();
        cl.setToFirstState();
        cr.setToFirstState();
        double u = rng->uniform01() * sum;
        for (it_s = sampleProbs.begin(); it_s != sampleProbs.end(); it_s++)
        {
            u -= it_s->second;
            if (u < 0.0)
            {
                const std::vector<unsigned>& v = it_s->first;
                ca += v[0];
                cl += v[1];
                cr += v[2];
                endStates[nodeIndex][i] = ca;
                startStates[left][i] = cl;
                startStates[right][i] = cr;
                break;
            }
        }
    }
    
    // recurse
    std::vector<TopologyNode*> children = node.getChildren();
    for (size_t i = 0; i < children.size(); i++)
    {
        // recurse towards tips
        if (!children[i]->isTip())
            recursivelyDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
        else
            AbstractPhyloCTMCSiteHomogeneous<charType,treeType>::tipDrawJointConditionalAncestralStates(*children[i], startStates, endStates, sampledSiteRates);
    }
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::resizeLikelihoodVectors( void )
{
    // call base resize
    RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::resizeLikelihoodVectors();
    
    // we resize the partial likelihood vectors to the new dimensions
    delete [] cladoPartialLikelihoods;
    delete [] cladoMarginalLikelihoods;
    
    size_t n = this->numNodes*this->numSiteRates*this->numPatterns*this->numChars*this->numChars;
    cladoPartialLikelihoods = new double[2*n];
    cladoMarginalLikelihoods = new double[n];
    
	// reinitialize likelihood vectors
	for (size_t i = 0; i < n; i++) {
		cladoPartialLikelihoods[i] = 0.0;
	}
	for (size_t i = 0; i < n; i++) {
		cladoMarginalLikelihoods[i] = 0.0;
	}
	
    // set the offsets for easier iteration through the likelihood vector
    cladoActiveLikelihoodOffset      =  this->numNodes*this->numSiteRates*this->numPatterns*this->numChars*this->numChars;
    cladoNodeOffset                  =                 this->numSiteRates*this->numPatterns*this->numChars*this->numChars;
    cladoMixtureOffset               =                                    this->numPatterns*this->numChars*this->numChars;
    cladoSiteOffset                  =                                                      this->numChars*this->numChars;

}


template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::setCladogenesisMatrix(const TypedDagNode< MatrixReal > *cm) {
    
    // remove the old parameter first
    if ( homogeneousCladogenesisMatrix != NULL )
    {
        this->removeParameter( homogeneousCladogenesisMatrix );
        homogeneousCladogenesisMatrix = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousCladogenesisMatrices );
        heterogeneousCladogenesisMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousCladogenesis = false;
    useObservedCladogenesis = true;
    homogeneousCladogenesisMatrix = cm;
    
    // add the new parameter
    this->addParameter( homogeneousCladogenesisMatrix );
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::setCladogenesisMatrix(const TypedDagNode< RbVector< MatrixReal > > *cm) {
    
    // remove the old parameter first
    if ( homogeneousCladogenesisMatrix != NULL )
    {
        this->removeParameter( homogeneousCladogenesisMatrix );
        homogeneousCladogenesisMatrix = NULL;
    }
    else
    {
        this->removeParameter( heterogeneousCladogenesisMatrices );
        heterogeneousCladogenesisMatrices = NULL;
    }
    
    // set the value
    branchHeterogeneousCladogenesis = true;
    useObservedCladogenesis = true;
    heterogeneousCladogenesisMatrices = cm;
    
    // add the new parameter
    this->addParameter( heterogeneousCladogenesisMatrices );
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::setCladogenesisTimes(const TypedDagNode< RbVector< RbVector< double > > > *ct)
{
    
    if (cladogenesisTimes != NULL)
    {
        this->removeParameter( cladogenesisTimes );
        cladogenesisTimes = NULL;
    }
    
    // set the value
    useSampledCladogenesis = true;
    cladogenesisTimes = ct;
    
    // add the new parameter
    this->addParameter( cladogenesisTimes );
    
    // redraw the current value
    if ( this->dagNode == NULL || !this->dagNode->isClamped() )
    {
        this->redrawValue();
    }
    
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::simulate( const TopologyNode &node, std::vector< DiscreteTaxonData< charType > > &taxa, const std::vector<size_t> &perSiteRates)
{
    
    // get the children of the node
    const std::vector<TopologyNode*>& children = node.getChildren();
    
    // get the sequence of this node
    size_t nodeIndex = node.getIndex();
    const DiscreteTaxonData< charType > &parent = taxa[ nodeIndex ];
    
    // simulate the sequence for each child
    RandomNumberGenerator* rng = GLOBAL_RNG;
    for (std::vector< TopologyNode* >::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        const TopologyNode &child = *(*it);
        
        // update the transition probability matrix
        updateTransitionProbabilities( child.getIndex(), child.getBranchLength() );
        
        DiscreteTaxonData< charType > &taxon = taxa[ child.getIndex() ];
        for ( size_t i = 0; i < this->numSites; ++i )
        {
            // get the ancestral character for this site
            unsigned long parentState = parent.getCharacter( i ).getStateIndex();
            
            double *freqs = this->transitionProbMatrices[ perSiteRates[i] ][ parentState ];
            
            // create the character
            charType c;
            c.setToFirstState();
            // draw the state
            double u = rng->uniform01();
            size_t stateIndex = 0;
            while ( true )
            {
                u -= *freqs;
                ++stateIndex;
                
                if ( u > 0.0 && stateIndex < this->numChars)
                {
                    ++c;
                    ++freqs;
                }
                else
                {
                    break;
                }
                
            }
            
            // add the character to the sequence
            taxon.addCharacter( c );
        }
        
        if ( child.isTip() )
        {
            taxon.setTaxonName( child.getName() );
        }
        else
        {
            // recursively simulate the sequences
            simulate( child, taxa, perSiteRates );
        }
        
    }
    
}

template<class charType, class treeType>
std::vector< std::vector<double> >* RevBayesCore::PhyloCTMCClado<charType, treeType>::sumMarginalLikelihoods( size_t nodeIndex )
{
    
    std::vector< std::vector<double> >* per_mixture_Likelihoods = new std::vector< std::vector<double> >(this->numPatterns, std::vector<double>(this->numChars, 0.0) );
    
    // get the pointers to the partial likelihoods and the marginal likelihoods
    double*         p_node_marginal         = this->marginalLikelihoods + nodeIndex*this->nodeOffset;
    
    // get pointers the likelihood for both subtrees
    double*         p_mixture_marginal          = p_node_marginal;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
        double*         p_site_mixture_marginal         = p_mixture_marginal;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // get the pointers to the likelihoods for this site and mixture category
            double*         p_site_marginal_j           = p_site_mixture_marginal;
            // iterate over all starting states
            for (size_t j=0; j<this->numChars; ++j)
            {
                // add the probability of being in this state
                (*per_mixture_Likelihoods)[site][j] += *p_site_marginal_j;
                
                // increment pointers
                ++p_site_marginal_j;
            }
            
            // increment the pointers to the next site
            p_site_mixture_marginal+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture_marginal+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
    return per_mixture_Likelihoods;
}


template<class charType, class treeType>
double RevBayesCore::PhyloCTMCClado<charType, treeType>::sumRootLikelihood( void )
{
    // get the root node
    const TopologyNode &root = this->tau->getValue().getRoot();
    
    // get the index of the root node
    size_t nodeIndex = root.getIndex();
    
    // get the pointers to the partial likelihoods of the left and right subtree
    double*   p_node  = this->partialLikelihoods + this->activeLikelihood[nodeIndex] * this->activeLikelihoodOffset  + nodeIndex*this->nodeOffset;
    
    // create a vector for the per mixture likelihoods
    // we need this vector to sum over the different mixture likelihoods
    std::vector<double> per_mixture_Likelihoods = std::vector<double>(this->numPatterns,0.0);
    
    // get pointers the likelihood for both subtrees
    double*   p_mixture     = p_node;
    // iterate over all mixture categories
    for (size_t mixture = 0; mixture < this->numSiteRates; ++mixture)
    {
        
        // get pointers to the likelihood for this mixture category
        double*   p_site_mixture     = p_mixture;
        // iterate over all sites
        for (size_t site = 0; site < this->numPatterns; ++site)
        {
            // temporary variable storing the likelihood
            double tmp = 0.0;
            // get the pointers to the likelihoods for this site and mixture category
            double* p_site_j   = p_site_mixture;
            // iterate over all starting states
            for (size_t i=0; i<this->numChars; ++i)
            {
                // add the probability of starting from this state
                tmp += *p_site_j;
                
                // increment pointers
                ++p_site_j;
            }
            // add the likelihood for this mixture category
            per_mixture_Likelihoods[site] += tmp;
            
            // increment the pointers to the next site
            p_site_mixture+=this->siteOffset;
            
        } // end-for over all sites (=patterns)
        
        // increment the pointers to the next mixture category
        p_mixture+=this->mixtureOffset;
        
    } // end-for over all mixtures (=rate categories)
    
    // sum the log-likelihoods for all sites together
    double sumPartialProbs = 0.0;
    // get the root frequencies
    const std::vector<double> &f = this->getRootFrequencies();
    
    double p_inv = this->pInv->getValue();
    double oneMinusPInv = 1.0 - p_inv;
    std::vector< size_t >::const_iterator patterns = this->patternCounts.begin();
    if ( p_inv > 0.0 )
    {
        for (size_t site = 0; site < this->numPatterns; ++site, ++patterns)
        {
            
            if ( this->useScaling == true )
            {
                
                if ( this->siteInvariant[site] )
                {
                    sumPartialProbs += log( p_inv * f[ this->invariantSiteIndex[site] ] * exp(this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site]) + oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                else
                {
                    sumPartialProbs += log( oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                sumPartialProbs -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] * *patterns;
                
            }
            else // no scaling
            {
                
                if ( this->siteInvariant[site] )
                {
                    sumPartialProbs += log( p_inv * f[ this->invariantSiteIndex[site] ]  + oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                else
                {
                    sumPartialProbs += log( oneMinusPInv * per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
                }
                
            }
        }
    }
    else
    {
        
        for (size_t site = 0; site < this->numPatterns; ++site, ++patterns)
        {
            
            sumPartialProbs += log( per_mixture_Likelihoods[site] / this->numSiteRates ) * *patterns;
            
            if ( this->useScaling == true )
            {
                
                sumPartialProbs -= this->perNodeSiteLogScalingFactors[this->activeLikelihood[nodeIndex]][nodeIndex][site] * *patterns;
            }
            
        }
        
        
    }
    
    return sumPartialProbs;
}



/** Swap a parameter of the distribution */
template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == homogeneousCladogenesisMatrix)
    {
        homogeneousCladogenesisMatrix = static_cast<const TypedDagNode< MatrixReal >* >( newP );
    }
    else if (oldP == heterogeneousCladogenesisMatrices)
    {
        heterogeneousCladogenesisMatrices = static_cast<const TypedDagNode< RbVector< MatrixReal > >* >( newP );
    }
    else if (oldP == cladogenesisTimes)
    {
        cladogenesisTimes = static_cast<const TypedDagNode< RbVector< RbVector< double > > >* >( newP );
    }
    else
    {
        RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::swapParameterInternal(oldP, newP);
    }
}

template<class charType, class treeType>
void RevBayesCore::PhyloCTMCClado<charType, treeType>::updateTransitionProbabilities(size_t nodeIdx, double brlen)
{
 
    
    
    // get cladogenesis probs for this branch
    const MatrixReal *cp;
    if ( branchHeterogeneousCladogenesis )
    {
        cp = &this->heterogeneousCladogenesisMatrices->getValue()[nodeIdx];
    }
    else
    {
        cp = &this->homogeneousCladogenesisMatrix->getValue();
    }
    
    // get sampled cladogenic events for this branch
    if (useSampledCladogenesis)
    {
        const std::vector<double>& times = cladogenesisTimes->getValue()[nodeIdx];
        if (times.size() == 0)
        {
            RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::updateTransitionProbabilities(nodeIdx, brlen);
        }
        else
        {
            // ... complete later
            
            // each interval
            double t = 0.0;
            double dt = 0.0;
            TransitionProbabilityMatrix tp(this->numChars);
            
            // go from present to past
            for (int i = (int)times.size() - 1; i >= 0; --i)
            {
                dt = (times[i] - t) * brlen;
                RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::updateTransitionProbabilities(nodeIdx, dt);

                if (i == 0)
                    tp = this->transitionProbMatrices[0];
                else
                    tp *= this->transitionProbMatrices[0];
                
                t = times[i];
            }
            
            // last interval
            dt = (1.0 - t) * brlen;
//            rm->calculateTransitionProbabilities( branchTime, this->transitionProbMatrices[0] );
        }
    }
    else
    {
        RevBayesCore::AbstractPhyloCTMCSiteHomogeneous<charType, treeType>::updateTransitionProbabilities(nodeIdx, brlen);
    }
}


#endif /* defined(__revbayes_proj__PhyloCTMCClado__) */
