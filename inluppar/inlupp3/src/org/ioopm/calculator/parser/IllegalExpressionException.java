package org.ioopm.calculator.parser;

public class IllegalExpressionException extends RuntimeException{
    IllegalExpressionException() {
        super(); 
    }
    public IllegalExpressionException(String msg) {
        super(msg);
    }
    
}
