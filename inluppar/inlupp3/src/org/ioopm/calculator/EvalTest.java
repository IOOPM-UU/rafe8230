package org.ioopm.calculator;

import org.ioopm.calculator.ast.*;

public class EvalTest {

    public void testEvaluating(SymbolicExpression expected, SymbolicExpression e) {
        SymbolicExpression r = e.eval();
        if (r.equals(expected)) {
            System.out.println("Passed: " + r);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + r + "'");
        }
    }

    public void evaluationTests() {
        Constant c0 = new Constant(0);
        Constant c1 = new Constant(1);
        Constant c2 = new Constant(2);
        Constant c5 = new Constant(5);
        Variable x = new Variable("x");

        // Leaves
        testEvaluating(c5, c5);  // a constant evaluates to itself
        testEvaluating(x, x);    // an unbound variable evaluates to itself

        // Binary operations that reduce fully
        testEvaluating(new Constant(42), new Addition(new Constant(5), new Constant(37)));
        testEvaluating(new Constant(3),  new Subtraction(c5, c2));
        testEvaluating(new Constant(10), new Multiplication(c5, c2));
        testEvaluating(new Constant(2.5), new Division(c5, c2));

        // Unary operations that reduce fully
        testEvaluating(c0, new Sin(c0));
        testEvaluating(c1, new Cos(c0));
        testEvaluating(c1, new Exp(c0));
        testEvaluating(c0, new Log(c1));
        testEvaluating(new Constant(-5), new Negation(c5));

        // Nested, reduces fully
        testEvaluating(new Constant(14),
                       new Multiplication(new Addition(c5, c2), c2));        // (5 + 2) * 2  ->  14
        testEvaluating(new Constant(-3),
                       new Negation(new Subtraction(c5, c2)));               // -(5 - 2)  ->  -3

        // Cannot reduce fully: the unbound variable stays
        testEvaluating(new Addition(c5, x), new Addition(c5, x));            // 5 + x
        testEvaluating(new Multiplication(new Addition(c5, x), c2),
                       new Multiplication(new Addition(c5, x), c2));         // (5 + x) * 2
        testEvaluating(new Sin(x), new Sin(x));                              // sin(x)

        // Partial reduction: the constant part is reduced, the rest kept
        testEvaluating(new Addition(new Constant(7), x),
                       new Addition(new Addition(c5, c2), x));               // (5 + 2) + x  ->  7 + x
        testEvaluating(new Multiplication(x, new Constant(10)),
                       new Multiplication(x, new Multiplication(c5, c2)));   // x * (5 * 2)  ->  x * 10
        testEvaluating(new Negation(new Addition(c1, x)),
                       new Negation(new Addition(new Cos(c0), x)));          // -(cos(0) + x)  ->  -(1 + x)

        // Assignment evaluates to the assigned value
        testEvaluating(c5, new Assignment(c5, x));                           // 5 = x  ->  5
        testEvaluating(new Constant(7), new Assignment(new Addition(c5, c2), x)); // 5 + 2 = x  ->  7
    }

    public static void main(String[] args) {
        EvalTest t = new EvalTest();
        t.evaluationTests();
    }
}