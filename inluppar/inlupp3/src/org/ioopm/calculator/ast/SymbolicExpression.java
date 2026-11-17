package org.ioopm.calculator.ast;

public abstract class SymbolicExpression {
    private int defaultPriority;
    
    public SymbolicExpression() {
        this.defaultPriority = 0; 
    }
    public boolean isConstant() {
        return false;
    }

    public String getName() {
        throw new RuntimeException("getName() called on expression with no operator");  
    }


    public int getPriority() {
        return this.defaultPriority;          
    }

    public double getValue() {
        throw new RuntimeException("getValue called on non constant"); 
    }

    public abstract SymbolicExpression eval(); 
}