package org.ioopm.calculator;

import java.io.IOException;
import org.ioopm.calculator.ast.Quit;
import org.ioopm.calculator.ast.Clear;
import org.ioopm.calculator.ast.Vars;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.Variable;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.parser.Environment;

public class Calculator {
    private int expressions;
    private int successfull;
    private int fullyEvaluated;

    Calculator() {
        this.expressions = 0;
        this.successfull = 0;
        this.fullyEvaluated = 0;
    }

    public static void main(String[] args) throws IOException {

        boolean quit = true;
        final CalculatorParser p = new CalculatorParser();
        Environment env = new Environment();
        Calculator calc = new Calculator();
        Variable ans = new Variable("ans");
        while (quit) {
            final Environment backup = env;
            String input = System.console().readLine();
            try {
                SymbolicExpression s = p.parse(input, env);
                if (s.isCommand()) {
                    if (s instanceof Quit) {
                        quit = false;
                        System.out.println(calc.expressions);
                        System.out.println(calc.successfull);
                        System.out.println(calc.fullyEvaluated);
                        break;
                    }
                    
                    
                    if (s instanceof Clear) {
                        env.clear();
                    }
                    if (s instanceof Vars) {
                        System.out.println(env);
                    }
                } else {
                    calc.expressions++;
                    SymbolicExpression res = s.eval(env);
                    System.out.println(res);
                    calc.successfull++;

                    if (res.isConstant()) {
                        calc.fullyEvaluated++;
                        env.put(ans, res);
                    }
                }
            } catch (RuntimeException Exception) {
                env = backup;
                System.out.println(Exception.getMessage());
            }

        }
    }
}
