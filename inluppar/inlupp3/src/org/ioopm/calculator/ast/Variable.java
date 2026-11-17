package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;

public class Variable extends Atom {
    private String identifier; 
    public Variable(String identifier) {
        this.identifier = identifier;
    }

    
    public String toString() {
        return this.identifier;
    }

    public boolean equals(Object other) {
        if (other instanceof Variable) {
            return this.equals((Variable) other);
        } else {
            return false;
        }
    }

    public boolean equals(Variable other) {
        return this.identifier.equals(other.identifier); 
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        if (vars.containsKey(this)) {
            return vars.get(this); 
        }
        return this; 
    }

    @Override 
    public int hashCode() {
        return this.identifier.hashCode(); 
    }
}
