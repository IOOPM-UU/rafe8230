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
}
