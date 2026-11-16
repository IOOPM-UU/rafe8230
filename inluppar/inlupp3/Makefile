all:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java

all_new: 
	javac -d classes src/org/ioopm/calculator/ast/*.java

run:
	java -cp classes org.ioopm.calculator.Calculator

clean:
	rm -rf classes