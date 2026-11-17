package org.ioopm.calculator.ast;

public abstract class Binary extends SymbolicExpression {
    private SymbolicExpression lhs = null;
    private SymbolicExpression rhs = null;  

    public Binary(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(); 
        this.lhs = lhs; 
        this.rhs = rhs; 
    }

    protected SymbolicExpression getLhs() {
        return this.lhs; 
    }
    protected SymbolicExpression getRhs() {
        return this.rhs; 
    }

    public String toString() {
        String s = "";  
        if (this.getPriority() > this.lhs.getPriority()) {
            s = s + "(" + this.lhs + ")";  
        } else {
            s = s + this.lhs; 
        }

        s = s + " " + this.getName() + " ";
        if (this.getPriority() > this.rhs.getPriority()) {
            return s = s + "(" + this.rhs + ")";
        }

        if (this.getPriority() == this.rhs.getPriority() && 
            (this.getName().equals("-") || this.getName().equals("/"))
            ) {

            return s = s + "(" + this.rhs + ")";  
        }
        return s = s + this.rhs; 
    }
}
