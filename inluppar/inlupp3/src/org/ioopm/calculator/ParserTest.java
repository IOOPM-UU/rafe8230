package org.ioopm.calculator;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.*;
public class ParserTest {
    private final CalculatorParser parser = new CalculatorParser();
    private int passed = 0;
    private int failed = 0;

    /// Parses input and checks that the resulting tree equals the expected tree.
    public void testParsing(SymbolicExpression expected, String input) {
        try {
            SymbolicExpression result = parser.parse(input, new Environment());
            if (result.equals(expected)) {
                passed++;
                System.out.println("Passed: '" + input + "' -> " + result);
            } else {
                failed++;
                System.out.println("Error: '" + input + "' expected '" + expected + "' but got '" + result + "'");
            }
        } catch (Exception e) {
            failed++;
            System.out.println("Error: '" + input + "' threw " + e.getClass().getSimpleName() + ": " + e.getMessage());
        }
    }

    /// Checks that parsing input throws an exception of the given type.
    public void testParseError(Class<? extends Exception> expectedType, String input) {
        try {
            SymbolicExpression result = parser.parse(input, new Environment());
            failed++;
            System.out.println("Error: '" + input + "' should throw " + expectedType.getSimpleName()
                               + " but parsed to '" + result + "'");
        } catch (Exception e) {
            if (expectedType.isInstance(e)) {
                passed++;
                System.out.println("Passed: '" + input + "' threw " + e.getClass().getSimpleName());
            } else {
                failed++;
                System.out.println("Error: '" + input + "' should throw " + expectedType.getSimpleName()
                                   + " but threw " + e.getClass().getSimpleName() + ": " + e.getMessage());
            }
        }
    }

    /// Checks that a command parses to its singleton instance (same object, ==).
    public void testCommand(SymbolicExpression expected, String input) {
        try {
            SymbolicExpression result = parser.parse(input, new Environment());
            if (result != null && result == expected) {
                passed++;
                System.out.println("Passed: '" + input + "' -> singleton");
            } else {
                failed++;
                System.out.println("Error: '" + input + "' did not return the singleton instance");
            }
        } catch (Exception e) {
            failed++;
            System.out.println("Error: '" + input + "' threw " + e.getClass().getSimpleName() + ": " + e.getMessage());
        }
    }

    public void parserTests() {
        Constant c1 = new Constant(1);
        Constant c2 = new Constant(2);
        Constant c3 = new Constant(3);
        Constant c5 = new Constant(5);
        Variable x = new Variable("x");
        Variable y = new Variable("y");

        // --- Atoms ---
        testParsing(c5, "5");
        testParsing(new Constant(2.5), "2.5");
        testParsing(x, "x");

        // --- Each binary operator ---
        testParsing(new Addition(c1, c2), "1 + 2");
        testParsing(new Subtraction(c1, c2), "1 - 2");
        testParsing(new Multiplication(c1, c2), "1 * 2");
        testParsing(new Division(c1, c2), "1 / 2");

        // --- Precedence: * and / bind tighter than + and - ---
        testParsing(new Addition(c1, new Multiplication(c2, c3)), "1 + 2 * 3");
        testParsing(new Subtraction(new Multiplication(c1, c2), c3), "1 * 2 - 3");

        // --- Left associativity: a - b - c means (a - b) - c ---
        testParsing(new Subtraction(new Subtraction(c5, c3), c1), "5 - 3 - 1");
        testParsing(new Division(new Division(c5, c3), c1), "5 / 3 / 1");

        // --- Parentheses override precedence ---
        testParsing(new Multiplication(new Addition(c1, c2), c3), "(1 + 2) * 3");
        testParsing(new Subtraction(c5, new Subtraction(c3, c1)), "5 - (3 - 1)");
        testParsing(c5, "((5))");

        // --- Unary operators ---
        testParsing(new Negation(c5), "-5");
        testParsing(new Negation(new Negation(c5)), "- -5");
        testParsing(new Negation(c5), "Neg 5");
        testParsing(new Sin(x), "Sin x");
        testParsing(new Cos(x), "Cos x");
        testParsing(new Exp(x), "Exp x");
        testParsing(new Log(x), "Log x");
        testParsing(new Sin(new Addition(x, c1)), "Sin (x + 1)");
        testParsing(new Addition(new Sin(x), c1), "Sin x + 1");          // unary binds tighter than +
        testParsing(new Multiplication(c2, new Negation(c3)), "2 * -3");

        // --- Assignment ---
        testParsing(new Assignment(c5, x), "5 = x");
        testParsing(new Assignment(new Addition(c1, c2), x), "1 + 2 = x");
        testParsing(new Assignment(new Assignment(c5, x), y), "5 = x = y"); // chained, left to right

        // --- Named constants become NamedConstant, not Variable ---
        testParsing(new Multiplication(new NamedConstant("pi", Math.PI), c2), "pi * 2");

        // --- Commands return their singleton ---
        testCommand(Quit.instance(), "Quit");
        testCommand(Vars.instance(), "Vars");
        testCommand(Clear.instance(), "Clear");

        // --- Syntax errors ---
        testParseError(SyntaxErrorException.class, "");          // nothing at all
        testParseError(SyntaxErrorException.class, "1 +");       // missing right operand
        testParseError(SyntaxErrorException.class, "(1 + 2");    // unbalanced parenthesis
        testParseError(SyntaxErrorException.class, "1 + 2)");    // extra parenthesis
        testParseError(SyntaxErrorException.class, "1 2");       // two numbers in a row
        testParseError(SyntaxErrorException.class, "5 = 3");     // number as variable name
        testParseError(SyntaxErrorException.class, "5 = ans");   // ans is reserved

        // --- Illegal redefinitions ---
        testParseError(IllegalExpressionException.class, "5 = Quit");

        // --- Round trip: printing a parsed tree and parsing it again gives the same tree ---
        String[] roundTrip = { "1 + 2 * 3", "(1 + 2) * 3", "5 - (3 - 1)", "x / (y / 2)", "-(x + 1)" };
        for (String input : roundTrip) {
            try {
                SymbolicExpression first = parser.parse(input, new Environment());
                SymbolicExpression second = parser.parse(first.toString(), new Environment());
                if (first.equals(second)) {
                    passed++;
                    System.out.println("Passed: round trip '" + input + "' -> '" + first + "'");
                } else {
                    failed++;
                    System.out.println("Error: round trip '" + input + "' printed as '" + first
                                       + "' which parses to '" + second + "'");
                }
            } catch (Exception e) {
                failed++;
                System.out.println("Error: round trip '" + input + "' threw " + e.getClass().getSimpleName()
                                   + ": " + e.getMessage());
            }
        }

        System.out.println("\n" + passed + " passed, " + failed + " failed");
    }

    public static void main(String[] args) {
        new ParserTest().parserTests();
    }
}