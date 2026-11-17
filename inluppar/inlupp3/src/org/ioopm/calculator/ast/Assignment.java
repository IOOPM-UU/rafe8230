package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;
import org.ioopm.calculator.parser.IllegalExpressionException;

public class Assignment extends Binary {

    public Assignment(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs); 
    }

    public String getName() {
        return "=";  
    }

    public int getPriority() {
        return 60; 
    }

    public String toString() {
        return super.toString(); 
    }

    public SymbolicExpression eval(Environment vars) {
        if (!(this.getRhs() instanceof Variable)) {
            throw new IllegalExpressionException("Error: Cannot redefine named constat " + this.getRhs()); 
        }
        SymbolicExpression value = this.getLhs().eval(vars); 
        vars.put((Variable) this.getRhs(), value); 
        return value; 
    }

    public boolean equals(Object other) {
        if (other instanceof Assignment) {
            return this.equals((Assignment) other );  
        }
        return false; 
    }

    public boolean equals(Assignment other) {
        return this.getLhs().equals(other.getLhs()) && this.getRhs().equals(other.getRhs()); 
    }
}
