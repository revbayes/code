#ifndef NonHomologousDiscreteCharacterData_H
#define NonHomologousDiscreteCharacterData_H

#include "AbstractNonHomologousDiscreteCharacterData.h"
#include "DiscreteCharacterState.h"
#include "MatrixReal.h"
#include "DiscreteTaxonData.h"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

namespace RevBayesCore {
    
    /**
     * Template class for all discrete character data objects.
     *
     * The discrete character data class is template class for all discrete character data objects.
     * Here the data is stored via a set of DiscreteTaxonData objects.
     * Several convenience functions are provided to access, store and delete the data.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-04-15, version 1.0
     */
    template<class charType>
    class NonHomologousDiscreteCharacterData : public AbstractNonHomologousDiscreteCharacterData {
        
    public:
                                                            NonHomologousDiscreteCharacterData(void);                                   //!< Default constructor
        
        // Overloaded operators
        const DiscreteTaxonData<charType>&                  operator[](size_t i) const;                                                 //!< Subscript operator (const)
        bool                                                operator<(const NonHomologousDiscreteCharacterData& x) const;               //!< Less than operator
        
        // implemented methods of the Cloneable interface
        NonHomologousDiscreteCharacterData<charType>*       clone(void) const;
                
        // CharacterData functions
        const charType&                                     getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        std::string                                         getDatatype(void) const;
        std::vector<double>                                 getEmpiricalBaseFrequencies(void) const;                                    //!< Compute the empirical base frequencies
        const std::set<size_t>&                             getExcludedCharacters(void) const;                                          //!< Returns the name of the file the data came from
        std::vector<size_t>                                 getNumberOfCharacters(void) const;                                          //!< Number of characters
        size_t                                              getNumberOfCharacters(size_t i) const;                                      //!< Number of characters
        size_t                                              getNumberOfStates(void) const;                                              //!< Get the number of states for the characters in this matrix
        DiscreteTaxonData<charType>&                        getTaxonData(size_t tn);                                                    //!< Return a reference to a sequence in the character matrix
        const DiscreteTaxonData<charType>&                  getTaxonData(size_t tn) const;                                              //!< Return a reference to a sequence in the character matrix
        DiscreteTaxonData<charType>&                        getTaxonData(const std::string &tn);                                        //!< Return a reference to a sequence in the character matrix
        const DiscreteTaxonData<charType>&                  getTaxonData(const std::string &tn) const;                                  //!< Return a reference to a sequence in the character matrix
    };
    
}


#include "DiscreteCharacterState.h"
#include "DiscreteTaxonData.h"
#include "RbConstants.h"
#include "RbException.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/**
 * Default constructor,
 * Does nothing except instanciating the object.
 */
template<class charType>
RevBayesCore::NonHomologousDiscreteCharacterData<charType>::NonHomologousDiscreteCharacterData()
{
    
}



/**
 * Index (const) operator to access a TaxonData object at position i.
 *
 * \param[in]    i    The position of the TaxonData object.
 *
 * \return            The TaxonData object at position i.
 */
template<class charType>
const RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::operator[]( const size_t i ) const
{
    
    return getTaxonData( i );
}



/**
 * Less than operator.
 */
template<class charType>
bool RevBayesCore::NonHomologousDiscreteCharacterData<charType>::operator<(const NonHomologousDiscreteCharacterData<charType> &x) const
{
    
    return sequenceNames.size() < x.sequenceNames.size();
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the object.
 */
template<class charType>
RevBayesCore::NonHomologousDiscreteCharacterData<charType>* RevBayesCore::NonHomologousDiscreteCharacterData<charType>::clone( void ) const
{
    
    return new NonHomologousDiscreteCharacterData<charType>(*this);
}



/**
 * Get the cn-th character of the tn-th taxon.
 *
 * \param[in]    tn     The index/position of the taxon.
 * \param[in]    cn     The position of the character.
 *
 * \return              The cn-th character of the tn-th taxon.
 */
template<class charType>
const charType& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getCharacter( size_t tn, size_t cn ) const
{
    
    if ( cn >= getNumberOfCharacters(tn) )
    {
        throw RbException( "Character index out of range" );
    }
    
    return getTaxonData( tn )[cn];
}


/**
 * Get the data type of the character stored in this object.
 *
 * \return      The data type (e.g. DNA, RNA or Standard).
 */
template<class charType>
std::string RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getDatatype(void) const
{
    
    std::string dt = "";
    if ( sequenceNames.size() > 0 )
    {
        const DiscreteTaxonData<charType> &t = getTaxonData( sequenceNames[0] );
        if ( t.size() > 0 )
        {
            dt = t[0].getDatatype();
        }
        
    }
    
    return dt;
}


/**
 * Get the number of characters in taxon data object.
 * This i regardless of whether the character are included or excluded.
 * For simplicity we assume that all taxon data objects contain the same number
 * of character and thus we simply return the number from the first taxon data object.
 *
 * \return    The total number of characters
 */
template<class charType>
size_t RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getNumberOfCharacters(size_t i) const
{
    
    if (getNumberOfTaxa() > i)
    {
        return getTaxonData(i).getNumberOfCharacters();
    }
    else
    {
        throw RbException("No taxon found for this index.");
    }
    
    return 0;
}


/**
 * Get the number of characters in taxon data object.
 * This i regardless of whether the character are included or excluded.
 * For simplicity we assume that all taxon data objects contain the same number
 * of character and thus we simply return the number from the first taxon data object.
 *
 * \return    The total number of characters
 */
template<class charType>
std::vector<size_t> RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getNumberOfCharacters( void ) const
{
    
    
    std::vector<size_t> indices;
    for (size_t i = 0; i < getNumberOfTaxa(); ++i)
    {
        indices.push_back( getTaxonData( i ).getNumberOfCharacters() );
    }
    
    return indices;
}


/**
 * Get the number of states for the characters in this object.
 * We assume that all of the characters in the matrix are of the same
 * type and have the same number of potential states.
 *
 * \return      The number of states for the characters.
 */
template<class charType>
size_t RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getNumberOfStates(void) const
{
    
    // Get the first character in the matrix
    if ( getNumberOfTaxa() == 0 )
        return 0;
    
    const DiscreteTaxonData<charType>& sequence = getTaxonData( 0 );
    if ( sequence.getNumberOfCharacters() == 0 )
        return 0;
    
    return sequence[0].getNumberOfStates();
}


/**
 * Get the taxon data object with index tn.
 *
 * \return     A const reference to the taxon data object at position tn.
 */
template<class charType>
const RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonData( size_t tn ) const
{
    
    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );
    
    const std::string& name = sequenceNames[tn];
    const typename std::map<std::string, AbstractTaxonData* >::const_iterator& i = taxonMap.find( name );
    
    if (i != taxonMap.end() )
    {
        return static_cast<const DiscreteTaxonData<charType>& >(*i->second);
    }
    else
    {
        throw RbException("Cannot find taxon '" + name + "' in the CharacterData matrix.");
    }
    
}


/**
 * Get the taxon data object at position tn.
 *
 * \return     A non-const reference to the taxon data object at position tn.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonData( size_t tn )
{
    
    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );
    
    const std::string& name = sequenceNames[tn];
    const typename std::map<std::string, AbstractTaxonData* >::iterator& i = taxonMap.find( name );
    
    if (i != taxonMap.end() )
    {
        return static_cast< DiscreteTaxonData<charType>& >(*i->second);
    }
    else
    {
        throw RbException("Cannot find taxon '" + name + "' in the CharacterData matrix.");
    }
    
}


/**
 * Get the taxon data object with name tn.
 *
 * \return     A non-const reference to the taxon data object with name tn.
 */
template<class charType>
const RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonData( const std::string &tn ) const
{
    
    if ( tn == "" )
    {
        throw RbException("Ambiguous taxon name.");
    }
    
    const typename std::map<std::string, AbstractTaxonData* >::const_iterator& i = taxonMap.find(tn);
    
    if (i != taxonMap.end() )
    {
        return static_cast<const DiscreteTaxonData<charType>& >(*i->second);
    }
    else
    {
        throw RbException("Cannot find taxon '" + tn + "' in the CharacterData matrix.");
    }
    
}


/**
 * Get the taxon data object with name tn.
 *
 * \return     A const reference to the taxon data object with name tn.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonData( const std::string &tn )
{
    
    
    if ( tn == "" )
    {
        throw RbException("Ambiguous taxon name.");
    }
    
    const typename std::map<std::string, AbstractTaxonData* >::iterator& i = taxonMap.find(tn);
    
    if (i != taxonMap.end() )
    {
        return static_cast< DiscreteTaxonData<charType>& >(*i->second);
    }
    else
    {
        throw RbException("Cannot find taxon '" + tn + "' in the CharacterData matrix.");
    }
    
}



#endif

