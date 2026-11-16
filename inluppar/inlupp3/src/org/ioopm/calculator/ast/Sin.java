package org.ioopm.calculator.ast;

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
}
