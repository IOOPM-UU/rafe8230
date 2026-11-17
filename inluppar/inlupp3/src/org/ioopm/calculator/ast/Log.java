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
    @Override 
    public SymbolicExpression eval() {
        SymbolicExpression arg = this.getArg().eval(); 
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
        return this.getPriority() == other.getPriority();
    }
}
