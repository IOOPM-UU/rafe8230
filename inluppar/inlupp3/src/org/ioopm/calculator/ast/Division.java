package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;

public class Division extends Binary {
    public Division(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs); 
    }

    public String getName() {
        return "/"; 
    }

    @Override 
    public int getPriority() {
        return 80;   
    }

    public String toString() {
        return super.toString();
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lArg = this.getLhs().eval(vars);  
        SymbolicExpression rArg = this.getRhs().eval(vars); 

        if (lArg.isConstant() && rArg.isConstant()) {
            return new Constant(lArg.getValue() / rArg.getValue());
        } 
        return new Division(lArg, rArg); 
    }

    public boolean equals(Object other) {
        if (other instanceof Division) {
            return equals((Division) other); 
        }
        return false;
    }

    public boolean equals(Division other) {
        return this.getLhs().equals(other.getLhs()) && this.getRhs().equals(other.getRhs());  
    }
}
