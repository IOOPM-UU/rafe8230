package org.ioopm.calculator.ast;

public class Negation extends Unary {
    public Negation(SymbolicExpression arg) {
        super(arg);
    }

    public String getName() {
        return "-";
    }

    @Override 
    public String toString() {
        if (this.getPriority() > this.getArg().getPriority()) {
            return "-(" + this.getArg() + ")"; 
        }
        return "-" + this.getArg();
    }

    public SymbolicExpression eval() {
        SymbolicExpression arg = this.getArg().eval();
        if (arg.isConstant()) {
            return new Constant(arg.getValue() * -1); 
        }
        return new Negation(arg); 
    }

    public boolean equals(Object other) {
        if (other instanceof Negation) {
            return equals((Negation) other); 
        }
        return false; 
    }
    
    public boolean equals(Negation other) {
        return this.getPriority() == other.getPriority(); 
    }
}
