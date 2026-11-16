package org.ioopm.calculator.ast; 

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

    @Override
    public String toString() {
        String s = this.getName() + " ";
        if (this.getPriority() > this.arg.getPriority()) {
            return s + "(" + this.arg + ")";
        }
        return s + this.arg;
    }
}