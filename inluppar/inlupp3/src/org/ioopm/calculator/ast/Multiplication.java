package org.ioopm.calculator.ast;

public class Multiplication extends Binary{
    public Multiplication(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs); 
    }

    public String getName() { 
        return "*"; 
    }

    @Override
    public int getPriority() {
        return 80; 
    }

    public String toString() {
        return super.toString();
    }

    @Override 
    public SymbolicExpression eval() {
        SymbolicExpression lArg = this.getLhs().eval();  
        SymbolicExpression rArg = this.getRhs().eval(); 

        if (lArg.isConstant() && rArg.isConstant()) {
            return new Constant(lArg.getValue() * rArg.getValue());
        } 
        return new Multiplication(lArg, rArg); 
    }

    public boolean equals(Object other) {
        if (other instanceof Multiplication) {
            return equals((Multiplication) other);  
        }
        return false; 
    }

    public boolean equals(Multiplication other) {
        return this.getLhs().equals(other.getLhs()) && this.getRhs().equals(other.getRhs());  
    }

}

