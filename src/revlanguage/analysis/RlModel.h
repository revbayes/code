/**
 * @file
 * This file contains the declaration of the RevLanguage wrapper of the model class.
 *
 * @brief Declaration of RlModel
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-06 20:14:22 +0200 (Mon, 06 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 * @extends RbObject
 *
 * $Id: Real.h 1746 2012-08-06 18:14:22Z hoehna $
 */

#ifndef RlModel_H
#define RlModel_H

#include "Model.h"
#include "WorkspaceToCoreWrapperObject.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class Model : public WorkspaceToCoreWrapperObject<RevBayesCore::Model> {
        
    public:
        
        Model(void);                                                                                                                        //!< Default constructor
        
        // Basic utility functions
        virtual Model*                              clone(void) const;                                                                      //!< Clone object
        void                                        constructInternalObject(void);                                                          //!< We construct the a new internal model object.
        static const std::string&                   getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                      getClassTypeSpec(void);                                                                 //!< Get class type spec
        const MemberRules&                          getParameterRules(void) const;                                                          //!< Get member rules (const)
        virtual const TypeSpec&                     getTypeSpec(void) const;                                                                //!< Get language type of the object

        // Member method inits
        virtual RevPtr<RevVariable>                 executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
        
    protected:
        
        virtual void                                printValue(std::ostream& o) const;                                                      //!< Print value (for user)
        void                                        setConstParameter(const std::string& name, const RevPtr<const RevVariable> &var);     //!< Set member variable
        void                                        printModelDotGraph(const std::string &fn, bool vb, const std::string &bgc);
        
        std::set<RevPtr<const RevVariable> >        sources;
        
    };
    
}

#endif
