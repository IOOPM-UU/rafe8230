package org.ioopm.calculator.parser;

import java.util.HashMap;

import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.Variable;

public class Environment extends HashMap<Variable, SymbolicExpression>{}
