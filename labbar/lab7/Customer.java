package labbar.lab7;

public class Customer {
    private int bornTime;
    private int groceries;
    public Customer(int bornTime, int groceries) {

        this.bornTime = bornTime;
        this.groceries = groceries;
    }

    public int groceriesGet() { 
        return this.groceries; 
    }
    public void serve() {
        if (!isDone()) {
            this.groceries--;
        }
    }
    
    public int getGroceries() {
        return this.groceries; 
    }
    public int getBornTime() {
        return this.bornTime; 
    }
    
    public boolean isDone() {
        return this.groceries == 0;
    }

    public static void main(String[] args) {
        // Constructor stores the values
        Customer c = new Customer(5, 3);
        System.out.println("bornTime stored (expect 5): " + c.bornTime);
        System.out.println("groceries stored (expect 3): " + c.groceries);

        // Serving counts down
        System.out.println("Not done at start (expect false): " + c.isDone());
        c.serve();
        System.out.println("After 1 serve (expect 2): " + c.groceries);
        System.out.println("Not done after 1 serve (expect false): " + c.isDone());
        c.serve();
        c.serve();
        System.out.println("After 3 serves (expect 0): " + c.groceries);
        System.out.println("Done after 3 serves (expect true): " + c.isDone());

        // Serving a done customer must not go below 0
        c.serve();
        System.out.println("Extra serve stays at 0 (expect 0): " + c.groceries);
        System.out.println("Still done (expect true): " + c.isDone());

        // A customer with 1 item needs exactly 1 serve
        Customer one = new Customer(0, 1);
        one.serve();
        System.out.println("1 item, done after 1 serve (expect true): " + one.isDone());

        // Edge case: 0 groceries is done before being served
        Customer empty = new Customer(0, 0);
        System.out.println("0 groceries is done immediately (expect true): " + empty.isDone());

        // Two customers don't affect each other
        Customer a = new Customer(0, 2);
        Customer b = new Customer(0, 2);
        a.serve();
        System.out.println("Serving a doesn't change b (expect 2): " + b.groceries);
    }
}
