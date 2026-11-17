package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;

public class Cos extends Unary {
    public Cos(SymbolicExpression arg) {
        super(arg); 
    }

    public String getName() {
        return "cos";
    }

    public String toString() {
        return super.toString();
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.getArg().eval(vars); 
        if (arg.isConstant()) {
            return new Constant(Math.cos(arg.getValue())); 
        } else {
            return new Cos(arg); 
        }
    }

    public boolean equals(Object other) {
        if (other instanceof Cos) {
            return equals((Cos) other); 
        }
        return false; 
    }
    
    public boolean equals(Cos other) {
        return this.getArg().equals(other.getArg()); 
    }
}
