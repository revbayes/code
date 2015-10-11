#include "RbConstants.h"
#include "RbException.h"
#include "StringUtilities.h"
#include "TaxonReader.h"
#include <sstream>

using namespace RevBayesCore;


/**
 * Constructor. Here we read in immediately the file and then we parse through each line 
 * and extract the taxon information.
 *
 * \param[in]     fn       The name of the file where the data is stored.
 * \param[in]     delim    The delimiter between the columns.
 */
TaxonReader::TaxonReader(const std::string &fn, char delim) : DelimitedDataReader( fn, delim )
{
    
    //Reading the header
    std::vector<std::string>& line = chars[0];
    int columnTaxon = -1;
    int columnAge = -1;
    
    for (size_t i = 0 ; i < line.size() ; ++i)
    {
        std::string tmp = line[i];
        StringUtilities::toLower( tmp );
        if ( tmp == "taxon" )
        {
            columnTaxon = int(i);
        }
        else if ( tmp == "age" )
        {
            columnAge = int(i);
        }
        else
        {
            throw RbException("Wrong header in the taxa definition file. It should contain 'taxon' and 'age' fields.");
        }
    }
    if (columnAge == -1 || columnTaxon == -1)
    {
        throw RbException("Wrong header in the taxa definition file. It should contain 'taxon' and 'age' fields.");
    }
    
    for (size_t i = 1; i < chars.size(); ++i) //going through all the lines
    {
        const std::vector<std::string>& line = chars[i];
        Taxon t = Taxon(line[ columnTaxon ]);
        std::stringstream ss;
        ss.str( line[ columnAge ] );
        double age;
        ss >> age;
        t.setAge( age );        
        taxa.push_back( t );
    }
    
}


/**
 * Get the taxon information read from the file.
 *
 * \return The vector of taxa.
 */
const std::vector<Taxon>& TaxonReader::getTaxa( void ) const
{
    
    return taxa;
}
