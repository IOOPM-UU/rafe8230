package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;
import org.ioopm.calculator.parser.SyntaxErrorException;

public abstract class Command extends SymbolicExpression {

    @Override 
    public SymbolicExpression eval(Environment vars) {
        throw new SyntaxErrorException("Commans may not be evaluated"); 
    }

    public boolean isCommand() { return true; } 

}
