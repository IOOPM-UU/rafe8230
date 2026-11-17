package org.ioopm.calculator.ast;


public class NamedConstant extends Constant{

    private String identifier; 

    public NamedConstant(String identifier, double value) {
        super(value); 
        this.identifier = identifier; 
    }

    @Override 
    public String toString() {
        return this.identifier;
    }
}
