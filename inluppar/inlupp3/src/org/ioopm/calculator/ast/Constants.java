package org.ioopm.calculator.ast;

import java.util.HashMap;

public class Constants {
    public static final HashMap<String, Double> namedConstants = new HashMap<>();

    static {
        Constants.namedConstants.put("pi", Math.PI);
        Constants.namedConstants.put("e", Math.E);
        Constants.namedConstants.put("phi", 1.6180339887);
        Constants.namedConstants.put("answer", 42.0); // TODO:  
        Constants.namedConstants.put("L", 6.022140857 * Math.pow(10, 23));
    }
}
