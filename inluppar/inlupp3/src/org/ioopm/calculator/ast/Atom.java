package org.ioopm.calculator.ast;

public abstract class Atom extends SymbolicExpression {
    @Override 
    public int getPriority() { 
        return 100; 
    }
}
