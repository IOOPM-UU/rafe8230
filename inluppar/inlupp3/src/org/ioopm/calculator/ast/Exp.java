package org.ioopm.calculator.ast;

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
    public SymbolicExpression eval() {
        SymbolicExpression arg = this.getArg().eval(); 
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
