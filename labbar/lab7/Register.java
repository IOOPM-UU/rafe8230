package labbar.lab7;

import java.util.EmptyStackException;

public class Register {
    private boolean open;
    private Queue queue;
    private Queue done; 
    public Register(Queue queue) {
        this.open = false;
        this.queue = queue;
    }

    public void open() {
        this.open = true;
    }

    public void close() {
        this.open = false;
    }

    public boolean isOpen() {
        return open;
    }

    public void step() {
        if (this.hasCustomers()) {
            this.queue.first().serve();
        }
    }

    public boolean hasCustomers() {
        return this.queue.length() != 0;
    }

    public boolean currentCustomerIsDone() {    
        return this.hasCustomers() && this.queue.first().isDone();
    }

    public void addToQueue(Customer c) {
        this.queue.enqueue(c);
    }

    public Customer removeCurrentCustomer() {
       return this.queue.dequeue();
    }

    public int getQueueLength() {
        return this.queue.length();
    }

    public Customer currentCustomer() {
        return this.queue.first();
    }

    @Override 
    public String toString() {
        String prefix = this.open ? " " : " X "; 
    
        if (!this.hasCustomers()) {
            return prefix + "[ ]"; 
        }
        String s = prefix + "[" + this.currentCustomer().getGroceries() + "]"; 
       for(int i = 1; i < this.getQueueLength(); i++) {
            s += "@"; 
        }
        return s; 
    }
    public static void main(String[] args) {
        // Open / close
        Register r = new Register(new Queue());
        System.out.println("New register closed (expect false): " + r.isOpen());
        r.open();
        System.out.println("After open() (expect true): " + r.isOpen());
        r.close();
        System.out.println("After close() (expect false): " + r.isOpen());

        // Empty register
        System.out.println("No customers at start (expect false): " + r.hasCustomers());
        System.out.println("Queue length at start (expect 0): " + r.getQueueLength());
        r.step(); // must not crash on an empty queue
        System.out.println("step() on empty register did not crash (expect true): true");

        // Adding customers
        Customer a = new Customer(0, 2);
        Customer b = new Customer(1, 1);
        r.addToQueue(a);
        r.addToQueue(b);
        System.out.println("Has customers (expect true): " + r.hasCustomers());
        System.out.println("Queue length (expect 2): " + r.getQueueLength());
        System.out.println("Current customer is a (expect true): " + (r.currentCustomer() == a));

        // Serving: a has 2 groceries
        System.out.println("a done before serving (expect false): " + r.currentCustomerIsDone());
        r.step();
        System.out.println("a done after 1 step (expect false): " + r.currentCustomerIsDone());
        r.step();
        System.out.println("a done after 2 steps (expect true): " + r.currentCustomerIsDone());
        System.out.println("step() only serves the first (b not done, expect false): " + b.isDone());

        // Removing
        System.out.println("Removed customer is a (expect true): " + (r.removeCurrentCustomer() == a));
        System.out.println("Queue length (expect 1): " + r.getQueueLength());
        System.out.println("Current customer is now b (expect true): " + (r.currentCustomer() == b));

        r.step();
        System.out.println("b done after 1 step (expect true): " + r.currentCustomerIsDone());
        System.out.println("Removed customer is b (expect true): " + (r.removeCurrentCustomer() == b));
        System.out.println("Empty again (expect false): " + r.hasCustomers());
        System.out.println("Queue length (expect 0): " + r.getQueueLength());
    }
}
