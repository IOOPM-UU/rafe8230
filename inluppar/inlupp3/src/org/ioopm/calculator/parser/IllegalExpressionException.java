package org.ioopm.calculator.parser;

import java.io.IOException;

public class IllegalExpressionException extends IOException{
    public IllegalExpressionException(String msg) {
        System.out.println(msg);
    }
    
}
