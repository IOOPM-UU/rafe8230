package org.ioopm.calculator.ast;

import java.util.HashMap;

import org.ioopm.calculator.parser.Environment;

public abstract class Unary extends SymbolicExpression {
    private SymbolicExpression arg; /// Some argument
    public Unary(SymbolicExpression arg) {
        super();
        this.arg = arg; 
    }

    public int getPriority() {
        return 90;
    }

    protected SymbolicExpression getArg() {
        return this.arg;
    }

    public String toString() {
        String s = this.getName() + " ";
        if (this.getPriority() > this.arg.getPriority()) {
            return s + "(" + this.arg + ")";
        }
        return s + this.arg;
    }

    public SymbolicExpression eval(Environment vars) {
        return this.arg.eval(vars); 
    }
    
}