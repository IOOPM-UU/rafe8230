package org.ioopm.calculator.ast;

public class Vars extends Command {
    private static final Vars theInstance = new Vars(); 
    private Vars() {assert theInstance == null : "Vars is a singleton, use Vars.instance()"; }
    public static Vars instance() {
            return theInstance; 
    }
}
