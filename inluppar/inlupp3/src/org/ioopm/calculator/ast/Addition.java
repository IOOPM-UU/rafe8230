package org.ioopm.calculator.ast;

public class Addition extends Binary { 
    public Addition(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs); 
    }

    public String getName() {
        return "+"; 
    }

    @Override 
    public int getPriority() { 
        return 70; 
    }

    

    @Override 
    public SymbolicExpression eval() {
        SymbolicExpression lArg = this.getLhs().eval();  
        SymbolicExpression rArg = this.getRhs().eval(); 

        if (lArg.isConstant() && rArg.isConstant()) {
            return new Constant(lArg.getValue() + rArg.getValue());
        } 
        return new Addition(lArg, rArg); 
    }

    public boolean equals(Object other) {
        if (!(other instanceof Addition)) {
            return false; 
        }
        return this.equals((Addition) other); 
    }

    public boolean equals(Addition other) {
        return this.getPriority() == other.getPriority(); 
    }
}

