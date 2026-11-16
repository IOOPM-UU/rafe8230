package org.ioopm.calculator.ast;

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
}
