package org.ioopm.calculator.ast;

public class Quit extends Command {
    private static final Quit theInstance = new Quit();  
    private Quit() { assert theInstance == null : "Quit is a singleton, use Quit.instance()"; }
    public static Quit instance() {
        return theInstance; 
    }
}
