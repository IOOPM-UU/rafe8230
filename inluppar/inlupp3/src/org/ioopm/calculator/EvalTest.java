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

    public void testNotEqual(SymbolicExpression a, SymbolicExpression b) {
        if (!a.equals(b)) {
            System.out.println("Passed: " + a + " != " + b);
        } else {
            System.out.println("Error: '" + a + "' should not equal '" + b + "'");
        }
    }

    public void evaluationTests() {
        Constant c0 = new Constant(0);
        Constant c1 = new Constant(1);
        Constant c2 = new Constant(2);
        Constant c5 = new Constant(5);
        Variable x = new Variable("x");

        // Leaves
        testEvaluating(c5, c5); // a constant evaluates to itself
        testEvaluating(x, x); // an unbound variable evaluates to itself

        // Binary operations that reduce fully
        testEvaluating(new Constant(42), new Addition(new Constant(5), new Constant(37)));
        testEvaluating(new Constant(3), new Subtraction(c5, c2));
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
                new Multiplication(new Addition(c5, c2), c2)); // (5 + 2) * 2 -> 14
        testEvaluating(new Constant(-3),
                new Negation(new Subtraction(c5, c2))); // -(5 - 2) -> -3

        // Cannot reduce fully: the unbound variable stays
        testEvaluating(new Addition(c5, x), new Addition(c5, x)); // 5 + x
        testEvaluating(new Multiplication(new Addition(c5, x), c2),
                new Multiplication(new Addition(c5, x), c2)); // (5 + x) * 2
        testEvaluating(new Sin(x), new Sin(x)); // sin(x)

        // Partial reduction: the constant part is reduced, the rest kept
        testEvaluating(new Addition(new Constant(7), x),
                new Addition(new Addition(c5, c2), x)); // (5 + 2) + x -> 7 + x
        testEvaluating(new Multiplication(x, new Constant(10)),
                new Multiplication(x, new Multiplication(c5, c2))); // x * (5 * 2) -> x * 10
        testEvaluating(new Negation(new Addition(c1, x)),
                new Negation(new Addition(new Cos(c0), x))); // -(cos(0) + x) -> -(1 + x)

        // Assignment evaluates to the assigned value
        testEvaluating(c5, new Assignment(c5, x)); // 5 = x -> 5
        testEvaluating(new Constant(7), new Assignment(new Addition(c5, c2), x)); // 5 + 2 = x -> 7

        // --- equals must be able to say no ---
        testNotEqual(new Addition(c5, x), new Multiplication(c5, x)); // different operator
        testNotEqual(new Sin(x), new Cos(x)); // different unary operator
        testNotEqual(new Subtraction(c5, x), new Subtraction(x, c5)); // order matters
        testNotEqual(new Variable("x"), new Variable("y")); // different names
        testNotEqual(c5, x); // constant vs variable
        testNotEqual(new Addition(c5, x), new Addition(c2, x)); // different child

        // --- equals with separately created but identical objects ---
        testEvaluating(new Variable("x"), new Variable("x")); // not the same object

        // --- partial reduction, the other binary operations ---
        testEvaluating(new Subtraction(x, new Constant(3)),
                new Subtraction(x, new Subtraction(c5, c2))); // x - (5 - 2) -> x - 3
        testEvaluating(new Division(new Constant(10), x),
                new Division(new Multiplication(c5, c2), x)); // (5 * 2) / x -> 10 / x

        // --- unary operations with a nested argument ---
        testEvaluating(c0, new Sin(new Subtraction(c5, c5))); // sin(5 - 5) -> 0
        testEvaluating(new Constant(5), new Negation(new Negation(c5))); // -(-5) -> 5
        testEvaluating(new Exp(new Addition(c1, x)),
                new Exp(new Addition(new Cos(c0), x))); // exp(cos 0 + x) -> exp(1 + x)

        // --- deeper nesting, both sides reduce ---
        testEvaluating(new Constant(35),
                new Multiplication(new Addition(c5, c2),
                        new Subtraction(new Constant(10), c5))); // (5+2)*(10-5) -> 35

        // --- eval must not change the original tree ---
        SymbolicExpression original = new Addition(new Addition(c5, c2), x);
        original.eval();
        testEvaluating(new Addition(new Addition(c5, c2), x), original); // still (5 + 2) + x before eval
    }

    public static void main(String[] args) {
        EvalTest t = new EvalTest();
        t.evaluationTests();
    }
}