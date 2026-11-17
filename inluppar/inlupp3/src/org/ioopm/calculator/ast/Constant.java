package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;

public class Constant extends Atom {
    private double value; 
    public Constant(double value) {
        this.value = value; 
    }

    @Override 
    public boolean isConstant() { return true; }

    public String toString() {
        return String.valueOf(this.value); 
    }

    public boolean equals(Object other) {
        if (other instanceof Constant) {
            return this.equals((Constant) other);  
        }
        return false; 
    }
    public boolean equals(Constant other) {
        return this.value == other.value; 
    }

    public double getValue() {
        return this.value; 
    }
    
    @Override 
    public SymbolicExpression eval(Environment vars) {
        return new Constant(value); 
    }
}

