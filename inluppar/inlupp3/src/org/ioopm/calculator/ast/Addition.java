package org.ioopm.calculator.ast;

import java.util.HashMap;

import org.ioopm.calculator.parser.Environment;

public class Addition extends Binary { 
    public Addition(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs); 
    }

    public String getName() {
        return "+"; 
    }

    @Override 
    public int getPriority() { 
        return 70; 
    }

    

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lArg = this.getLhs().eval(vars);  
        SymbolicExpression rArg = this.getRhs().eval(vars); 

        if (lArg.isConstant() && rArg.isConstant()) {
            return new Constant(lArg.getValue() + rArg.getValue());
        } 
       return new Addition(lArg, rArg); 
    }

    public boolean equals(Object other) {
        if (!(other instanceof Addition)) {
            return false; 
        }
        return this.equals((Addition) other); 
    }

    public boolean equals(Addition other) {
        return this.getLhs().equals(other.getLhs()) && this.getRhs().equals(other.getRhs());  
    }
}

