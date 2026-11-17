package org.ioopm.calculator.ast;

public class Clear extends Command {
    private static final Clear theInstance = new Clear();  
    private Clear() { assert theInstance == null : "Clear is a singleton, use Clear.instance()"; }
    public static Clear instance() {
        return theInstance; 
    }
}