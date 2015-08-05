#ifndef SpeciesNodeTimeSlideUniform_H
#define SpeciesNodeTimeSlideUniform_H

#include <string>

#include "Proposal.h"
#include "StochasticNode.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    /**
     * The species-node-time-slide (uniform) operator.
     *
     * A species-node-time-slide proposal is a sliding window proposal on rooted subtrees without changing the topology.
     * That is, we pick a random node which is not the root.
     * Then, we uniformly propose a new age centered around the current age plus-minus epsilon.
     * All gene-trees that are present in the population will be scaled too!
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2015-06-23, version 1.0
     *
     */
    class SpeciesNodeTimeSlideUniform : public Proposal {
        
    public:
        SpeciesNodeTimeSlideUniform(StochasticNode<TimeTree> *sp, std::vector< StochasticNode<TimeTree> *> gt);                                               //!<  constructor
        
        // Basic utility functions
        void                                            cleanProposal(void);                                        //!< Clean up proposal
        SpeciesNodeTimeSlideUniform*                    clone(void) const;                                          //!< Clone object
        double                                          doProposal(void);                                           //!< Perform proposal
        const std::string&                              getProposalName(void) const;                                //!< Get the name of the proposal for summary printing
        void                                            prepareProposal(void);                                      //!< Prepare the proposal
        void                                            printParameterSummary(std::ostream &o) const;               //!< Print the parameter summary
        void                                            tune(double r);                                             //!< Tune the proposal to achieve a better acceptance/rejection ratio
        void                                            undoProposal(void);                                         //!< Reject the proposal
        
    protected:
        
        std::vector<TopologyNode*>                      getNodesInPopulation( TimeTree &tau, TopologyNode &n );
        void                                            swapNodeInternal(DagNode *oldN, DagNode *newN);             //!< Swap the DAG nodes on which the Proposal is working on
        
        
    private:
        
        // parameters
        StochasticNode<TimeTree>*                       speciesTree;                                                   //!< The variable the Proposal is working on
        std::vector< StochasticNode<TimeTree> *>        geneTrees;
        
        // stored objects to undo proposal
        TopologyNode*                                   storedNode;
        double                                          storedAge;
        
        std::map< std::string, TopologyNode*>           individualNames2geneTreeTips;
        
    };
    
}

#endif

