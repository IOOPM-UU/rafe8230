package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;

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
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lArg = this.getLhs().eval(vars);  
        SymbolicExpression rArg = this.getRhs().eval(vars); 

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
        return this.getLhs().equals(other.getLhs()) && this.getRhs().equals(other.getRhs());  
    }
}
