package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;

public class Exp extends Unary {
    public Exp(SymbolicExpression arg) {
        super(arg); 
    }
    public String getName() {
        return "exp";
    }

    public String toString() {
        return super.toString();
    }
    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.getArg().eval(vars); 
        if (arg.isConstant()) {
            return new Constant(Math.exp(arg.getValue()));  
        } else {
            return new Exp(arg); 
        }
    }

    public boolean equals(Object other) {
        if (other instanceof Exp) {
            return equals((Exp) other); 
        }
        return false; 
    }
    
    public boolean equals(Exp other) {
        return this.getPriority() == other.getPriority(); 
    }
}
