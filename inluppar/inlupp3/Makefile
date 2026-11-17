all:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java

all_new: 
	javac -d classes src/org/ioopm/calculator/ast/*.java

run:
	java -cp classes org.ioopm.calculator.Calculator

TestEval:
	javac -d classes -sourcepath src src/org/ioopm/calculator/ast/*.java src/org/ioopm/calculator/parser/Environment.java src/org/ioopm/calculator/EvalTest.java
	java -cp classes org.ioopm.calculator.EvalTest

TestCreateExpression: 
	javac -d classes -sourcepath src src/org/ioopm/calculator/ast/*.java src/org/ioopm/calculator/TestCreateExpression.java
	java -cp classes org.ioopm.calculator.TestCreateExpression

TestAll: TestEval TestCreateExpression

clean:
	rm -rf classes