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

    @Override 
    public SymbolicExpression eval() {
        SymbolicExpression lArg = this.getLhs().eval();  
        SymbolicExpression rArg = this.getRhs().eval(); 

        if (lArg.isConstant() && rArg.isConstant()) {
            double result = lArg.getValue() - rArg.getValue(); 
            return new Constant(result);
        } 
        return new Subtraction(lArg, rArg); 
    }

    public boolean equals(Object other) {
        if (other instanceof Subtraction) {
            return equals((Subtraction) other); 
        }
        return false; 
    }

    public boolean equals(Subtraction other) {
        return this.getPriority() == other.getPriority(); 
    }
}
