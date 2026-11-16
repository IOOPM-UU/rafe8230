package org.ioopm.calculator.ast;

public class Log extends Unary {
    public Log(SymbolicExpression arg) {
        super(arg);
    } 

    public String getName() {
        return "log";
    }

    public String toString() {
        return super.toString();
    }
}
