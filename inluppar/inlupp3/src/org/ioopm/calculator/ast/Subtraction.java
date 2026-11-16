package org.ioopm.calculator.ast;

public class Subtraction extends Binary {
    public Subtraction(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs); 
    }

    public String getName() {
        return "-"; 
    }
    
    @Override 
    public int getPriority() {
        return 70; 
    }

    public String toString() {
        return super.toString();
    }
}
