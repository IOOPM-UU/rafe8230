package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;

public class Sin extends Unary {
    public Sin(SymbolicExpression arg) {
        super(arg); 
    }
    public String getName() {
        return "sin";
    }
    public String toString() {
        return super.toString();
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.getArg().eval(vars); 
        if (arg.isConstant()) {
            return new Constant(Math.sin(arg.getValue())); 
        } else {
            return new Sin(arg); 
        }
    }

    public boolean equals(Object other) {
        if (other instanceof Sin) {
            return equals((Sin) other); 
        }
        return false; 
    }
    
    public boolean equals(Sin other) {
        return this.getPriority() == other.getPriority(); 
    }
}
