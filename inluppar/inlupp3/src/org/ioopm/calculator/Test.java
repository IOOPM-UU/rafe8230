package org.ioopm.calculator;

import org.ioopm.calculator.ast.*;

/// Printing tests for step B4/B5.
///
/// In step B4 every test should FAIL (objects print as "Constant@1b6d3586").
/// The goal of step B5 is to make them all pass.
///
/// The expected strings follow these printing rules. If you choose other
/// rules, change the expected strings to match. Just be consistent.
///
///   Constants  print as Java prints a double: 42 -> "42.0", 1e10 -> "1.0E10"
///   Variables  print their name: "x"
///   Binary     "lhs op rhs", with one space around the operator: "x + y"
///   Functions  "name arg", with one space: "sin x"
///   Negation   "-arg", no space: "-x"
///
///   Parentheses (priorities: atom 100, unary 90, * / 80, + - 70, = 60):
///   - A subtree gets parentheses if its priority is LOWER than its parent's.
///   - The RIGHT subtree of - and / also gets them if the priority is EQUAL,
///     because a - (b - c) is not the same as a - b - c.
public class Test {
    private static int passed = 0;
    private static int failed = 0;

    public static void main(String[] args) {
        /// Building blocks used in many tests
        SymbolicExpression x = new Variable("x");
        SymbolicExpression y = new Variable("y");
        SymbolicExpression z = new Variable("z");
        SymbolicExpression a = new Variable("a");
        SymbolicExpression b = new Variable("b");
        SymbolicExpression c = new Variable("c");
        SymbolicExpression d = new Variable("d");
        SymbolicExpression one = new Constant(1);
        SymbolicExpression two = new Constant(2);
        SymbolicExpression five = new Constant(5);

        section("The example from the instructions");
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition add = new Addition(c1, v);
        Multiplication m = new Multiplication(add, c2);
        testPrinting("(5.0 + x) * 2.0", m);
        testPrinting("(5.0 + x) * 2.0",
            new Multiplication(new Addition(new Constant(5), new Variable("x")), new Constant(2)));

        section("Constants");
        testPrinting("42.0", new Constant(42));
        testPrinting("0.0", new Constant(0));
        testPrinting("3.14", new Constant(3.14));
        testPrinting("0.1", new Constant(0.1));
        testPrinting("-5.0", new Constant(-5));                 /// can't be typed, but can be built
        testPrinting("0.3333333333333333", new Constant(1.0 / 3));
        testPrinting("1.0E10", new Constant(1e10));             /// Java switches to E-notation
        testPrinting("Infinity", new Constant(1.0 / 0));        /// what 1/0 evaluates to

        section("Variables");
        testPrinting("x", x);
        testPrinting("abc", new Variable("abc"));
        testPrinting("fooBar", new Variable("fooBar"));
        testPrinting("ans", new Variable("ans"));

        section("Each binary operator on its own");
        testPrinting("x + y", new Addition(x, y));
        testPrinting("x - y", new Subtraction(x, y));
        testPrinting("x * y", new Multiplication(x, y));
        testPrinting("x / y", new Division(x, y));
        testPrinting("5.0 = x", new Assignment(five, x));
        testPrinting("1.0 + 2.0", new Addition(one, two));

        section("Each unary operator on its own");
        testPrinting("-x", new Negation(x));
        testPrinting("sin x", new Sin(x));
        testPrinting("cos x", new Cos(x));
        testPrinting("exp x", new Exp(x));
        testPrinting("log x", new Log(x));
        testPrinting("-5.0", new Negation(five));              /// same text as new Constant(-5)!
        testPrinting("cos 0.0", new Cos(new Constant(0)));

        section("Precedence: no parentheses when they are not needed");
        testPrinting("a + b * c", new Addition(a, new Multiplication(b, c)));
        testPrinting("a * b + c", new Addition(new Multiplication(a, b), c));
        testPrinting("a * b + c * d",
            new Addition(new Multiplication(a, b), new Multiplication(c, d)));
        testPrinting("a / b - c / d",
            new Subtraction(new Division(a, b), new Division(c, d)));

        section("Precedence: parentheses when they are needed");
        testPrinting("(a + b) * c", new Multiplication(new Addition(a, b), c));
        testPrinting("a * (b + c)", new Multiplication(a, new Addition(b, c)));
        testPrinting("(a + b) * (c - d)",
            new Multiplication(new Addition(a, b), new Subtraction(c, d)));
        testPrinting("(a - b) / (c + d)",
            new Division(new Subtraction(a, b), new Addition(c, d)));

        section("Same priority, nested to the LEFT: never parentheses");
        testPrinting("a + b + c", new Addition(new Addition(a, b), c));
        testPrinting("a - b - c", new Subtraction(new Subtraction(a, b), c));
        testPrinting("a * b * c", new Multiplication(new Multiplication(a, b), c));
        testPrinting("a / b / c", new Division(new Division(a, b), c));
        testPrinting("a - b + c", new Addition(new Subtraction(a, b), c));
        testPrinting("1.0 + 2.0 + 1.0 + 2.0 + 5.0",
            new Addition(new Addition(new Addition(new Addition(one, two), one), two), five));

        section("Same priority, nested to the RIGHT");
        /// + and * don't care about grouping, so no parentheses are needed
        testPrinting("a + b + c", new Addition(a, new Addition(b, c)));
        testPrinting("a * b * c", new Multiplication(a, new Multiplication(b, c)));
        testPrinting("a + b - c", new Addition(a, new Subtraction(b, c)));
        /// The example from the instructions: (a * b) * (c / d)
        testPrinting("a * b * c / d",
            new Multiplication(new Multiplication(a, b), new Division(c, d)));
        /// - and / DO care: dropping these parentheses changes the meaning
        testPrinting("a - (b - c)", new Subtraction(a, new Subtraction(b, c)));
        testPrinting("a - (b + c)", new Subtraction(a, new Addition(b, c)));
        testPrinting("a / (b / c)", new Division(a, new Division(b, c)));
        testPrinting("a / (b * c)", new Division(a, new Multiplication(b, c)));

        section("Unary operators inside expressions");
        testPrinting("sin x + 1.0", new Addition(new Sin(x), one));
        testPrinting("sin x * 2.0", new Multiplication(new Sin(x), two));
        testPrinting("-x * y", new Multiplication(new Negation(x), y));
        testPrinting("x * -y", new Multiplication(x, new Negation(y)));
        testPrinting("x - -y", new Subtraction(x, new Negation(y)));
        testPrinting("-x + -y", new Addition(new Negation(x), new Negation(y)));

        section("Expressions inside unary operators");
        testPrinting("sin (x + 1.0)", new Sin(new Addition(x, one)));
        testPrinting("exp (x * 2.0)", new Exp(new Multiplication(x, two)));
        testPrinting("-(a + b)", new Negation(new Addition(a, b)));
        testPrinting("-(a * b)", new Negation(new Multiplication(a, b)));
        testPrinting("log (5.0 = x)", new Log(new Assignment(five, x)));

        section("Unary inside unary: no parentheses");
        testPrinting("sin cos x", new Sin(new Cos(x)));
        testPrinting("--x", new Negation(new Negation(x)));
        testPrinting("---x", new Negation(new Negation(new Negation(x))));
        testPrinting("-sin x", new Negation(new Sin(x)));
        testPrinting("log -x", new Log(new Negation(x)));
        testPrinting("-sin exp (a * a * 10.0)",
            new Negation(new Sin(new Exp(
                new Multiplication(new Multiplication(a, a), new Constant(10))))));

        section("Assignments");
        testPrinting("x + 1.0 = y", new Assignment(new Addition(x, one), y));
        testPrinting("2.0 * x = y", new Assignment(new Multiplication(two, x), y));
        testPrinting("sin x = y", new Assignment(new Sin(x), y));
        /// 2 = x = y  is  (2 = x) = y
        testPrinting("2.0 = x = y", new Assignment(new Assignment(two, x), y));
        testPrinting("1.0 = a = b = c",
            new Assignment(new Assignment(new Assignment(one, a), b), c));

        section("Assignments inside expressions need parentheses");
        testPrinting("(5.0 = x) + y", new Addition(new Assignment(five, x), y));
        testPrinting("y * (5.0 = x)", new Multiplication(y, new Assignment(five, x)));
        /// From the sample session: (x+y=z) - (z-1)*(1=x)
        testPrinting("(x + y = z) - (z - 1.0) * (1.0 = x)",
            new Subtraction(
                new Assignment(new Addition(x, y), z),
                new Multiplication(new Subtraction(z, one), new Assignment(one, x))));

        section("Bigger trees");
        testPrinting("(a + b) * (c + d) / (a - b)",
            new Division(
                new Multiplication(new Addition(a, b), new Addition(c, d)),
                new Subtraction(a, b)));
        testPrinting("sin (x * x + y * y) - cos (x - y)",
            new Subtraction(
                new Sin(new Addition(new Multiplication(x, x), new Multiplication(y, y))),
                new Cos(new Subtraction(x, y))));

        /// Add these once the classes can be created this way:
        /// testPrinting("pi", new NamedConstant("pi", Math.PI));     /// step C1
        /// testPrinting("pi * 2.0", new Multiplication(new NamedConstant("pi", Math.PI), two));
        /// testPrinting("vars", Vars.instance());                    /// step B8
        /// testPrinting("quit", Quit.instance());

        System.out.println();
        System.out.println(passed + " passed, " + failed + " failed");

        /// Only checked when assertions are enabled (make test)
        assert failed == 0 : failed + " printing test(s) failed";
    }

    public static void testPrinting(String expected, SymbolicExpression e) {
        String actual = "" + e;
        if (expected.equals(actual)) {
            passed++;
            System.out.println("Passed: " + actual);
        } else {
            failed++;
            System.out.println("Error: expected '" + expected + "' but got '" + actual + "'");
        }
    }

    private static void section(String title) {
        System.out.println();
        System.out.println("== " + title + " ==");
    }
}