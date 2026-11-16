package org.ioopm.calculator;

import org.ioopm.calculator.ast.*;

/// Tests for getPriority() (step B3).
///
/// The exact numbers are up to you, so these tests only check the ORDER:
///   atoms (numbers, variables)  >  unary (-, sin, cos, exp, log)
///   >  * and /  >  + and -  >  = (assignment)
/// and that operators that should bind equally hard have equal priority.
///
/// Run from the folder that contains src:
///   javac -d classes -sourcepath src src/org/ioopm/calculator/PriorityTest.java
///   java -cp classes org.ioopm.calculator.PriorityTest
public class PriorityTest {
    private static int passed = 0;
    private static int failed = 0;

    public static void main(String[] args) {
        /// Some small building blocks to put inside the operators
        SymbolicExpression x = new Variable("x");
        SymbolicExpression y = new Variable("y");
        SymbolicExpression five = new Constant(5);

        /// One node of each kind
        SymbolicExpression constant = five;
        SymbolicExpression variable = x;
        SymbolicExpression negation = new Negation(x);
        SymbolicExpression sin = new Sin(x);
        SymbolicExpression cos = new Cos(x);
        SymbolicExpression exp = new Exp(x);
        SymbolicExpression log = new Log(x);
        SymbolicExpression multiplication = new Multiplication(x, y);
        SymbolicExpression division = new Division(x, y);
        SymbolicExpression addition = new Addition(x, y);
        SymbolicExpression subtraction = new Subtraction(x, y);
        SymbolicExpression assignment = new Assignment(five, x);

        /// Print every priority, so you can see your numbers
        System.out.println("Priorities:");
        show("Constant", constant);
        show("Variable", variable);
        show("Negation", negation);
        show("Sin", sin);
        show("Cos", cos);
        show("Exp", exp);
        show("Log", log);
        show("Multiplication", multiplication);
        show("Division", division);
        show("Addition", addition);
        show("Subtraction", subtraction);
        show("Assignment", assignment);
        System.out.println();

        /// Things that should bind equally hard
        same("Constant and Variable", constant, variable);
        same("Negation and Sin", negation, sin);
        same("Sin and Cos", sin, cos);
        same("Cos and Exp", cos, exp);
        same("Exp and Log", exp, log);
        same("Multiplication and Division", multiplication, division);
        same("Addition and Subtraction", addition, subtraction);

        /// The order from tightest to loosest
        higher("Atoms bind tighter than unary operations", constant, negation);
        higher("Unary operations bind tighter than * and /", negation, multiplication);
        higher("* and / bind tighter than + and -", multiplication, addition);
        higher("+ and - bind tighter than =", addition, assignment);

        /// What this means for printing in step B5:
        /// a subtree gets parentheses when its priority is LOWER than its parent's.
        needsParens("(x + y) * 5: the + inside * needs parentheses",
                    new Multiplication(addition, five), addition);
        noParens("x * y + 5: the * inside + needs no parentheses",
                 new Addition(multiplication, five), multiplication);
        needsParens("-(x + y): the + inside - needs parentheses",
                    new Negation(addition), addition);
        noParens("-x * y: the - inside * needs no parentheses",
                 new Multiplication(negation, y), negation);
        needsParens("(5 = x) + y: an assignment inside + needs parentheses",
                    new Addition(assignment, y), assignment);
        noParens("sin x: an atom never needs parentheses",
                 sin, variable);

        System.out.println();
        System.out.println(passed + " passed, " + failed + " failed");
    }

    private static void show(String name, SymbolicExpression e) {
        System.out.println("  " + name + ": " + e.getPriority());
    }

    private static void same(String description, SymbolicExpression a, SymbolicExpression b) {
        check(description + " have the same priority",
              a.getPriority() == b.getPriority());
    }

    private static void higher(String description, SymbolicExpression tighter, SymbolicExpression looser) {
        check(description, tighter.getPriority() > looser.getPriority());
    }

    private static void needsParens(String description, SymbolicExpression parent, SymbolicExpression child) {
        check(description, child.getPriority() < parent.getPriority());
    }

    private static void noParens(String description, SymbolicExpression parent, SymbolicExpression child) {
        check(description, child.getPriority() >= parent.getPriority());
    }

    private static void check(String description, boolean ok) {
        if (ok) {
            passed++;
            System.out.println("Passed: " + description);
        } else {
            failed++;
            System.out.println("FAILED: " + description);
        }
    }
}

