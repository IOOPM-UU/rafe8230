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
}
