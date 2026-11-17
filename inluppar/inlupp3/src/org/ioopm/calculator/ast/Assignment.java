package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;

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
        SymbolicExpression value = this.getLhs().eval(vars); 
        vars.put((Variable) this.getRhs(), value); 
        return value; 
    }
}
