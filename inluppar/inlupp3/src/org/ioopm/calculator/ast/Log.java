package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.Environment;

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
    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression arg = this.getArg().eval(vars); 
        if (arg.isConstant()) {
            return new Constant(Math.log(arg.getValue())); 
        } else {
            return new Log(arg); 
        }
    }

    public boolean equals(Object other) {
        if (other instanceof Log) {
            return equals((Log) other);
        }
        return false;
    }

    public boolean equals(Log other) {
        return this.getArg().equals(other.getArg()); 
    }
}
