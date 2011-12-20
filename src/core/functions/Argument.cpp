/**
 * @file
 * This file contains the implementation of Argument, which is
 * used to hold a potentially labeled argument passed to a
 * function.
 *
 * @brief Implementation of Argument
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-20, version 1.0
 *
 * $Id$
 */

#include <sstream>

#include "Argument.h"
#include "DAGNode.h"
#include "RbUtil.h"
#include "VectorString.h"


// Definition of the static type spec member
const TypeSpec Argument::typeSpec(Argument_name);

/** Construct from argument label and DAG node */
Argument::Argument(RbPtr<Variable> v) : RbInternal() {
    
    label   = "";
    var     = v;
}


/** Construct from argument label and DAG node */
Argument::Argument(const std::string& argLabel, RbPtr<Variable> v) : RbInternal() {

    label   = argLabel;
    var     = v;
}

/** Copy Constructor. We keep the same pointer to the variable stored inside this argument. */
Argument::Argument(const Argument &x) : RbInternal(x) {
    
    label   = x.label;
    var     = x.var;
}


/** Destructor */
Argument::~Argument() {
    
}


/** Get class vector describing type of object */
const VectorString& Argument::getClass(void) const { 

    static VectorString rbClass = VectorString(Argument_name) + RbInternal::getClass();
	return rbClass; 
}


RbPtr<const DAGNode> Argument::getDagNode(void) const {
    return RbPtr<const DAGNode>( var->getDagNode() );
}


RbPtr<DAGNode> Argument::getDagNode(void) {
    return var->getDagNode();
}


const std::string& Argument::getLabel(void) const {
    return label;
}


/** Get the type spec of this class. We return a static class variable because all instances will be exactly from this type. */
const TypeSpec& Argument::getTypeSpec(void) const {
    return typeSpec;
}


RbPtr<const Variable> Argument::getVariable(void) const {
    return RbPtr<const Variable>( var );
}


RbPtr<Variable> Argument::getVariable(void) {
    return var;
}


/** Complete info about object */
std::string Argument::richInfo(void) const {

    std::ostringstream o;
    o << "Argument: label = \"" << label << "\", value = ";
    var->printValue(o);

    return o.str();
}

/** Set the DAG node of the argument */
void Argument::setDagNode(RbPtr<DAGNode> newNode) {
    var->setDagNode(newNode);
}


/** Set the variable of the argument */
void Argument::setVariable(RbPtr<Variable> newVar) {
    
    var = newVar;
}

