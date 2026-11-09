package labbar.lab7;

import org.w3c.dom.Node;

public class Store {
    Register registers[];

    public Store(int amount) {
        this.registers = new Register[amount];
        for (int i = 0; i < this.registers.length; i++) {
            this.registers[i] = new Register(new Queue());
        }
        this.registers[0].open();
    }

    public int getlength() {
        return this.registers.length;
    }

    public float getAverageQueueLength() {
        int sum = 0;
        int openRegisters = 0;
        for (Register register : registers) {
            if (register.isOpen()) {
                sum += register.getQueueLength();  
                openRegisters++; 
            }
        }
        if (openRegisters == 0) {
            return 0;  
        }
        return (float) sum / openRegisters; 
    }

    public void newCustomer(Customer c) {
        int shortest = Integer.MAX_VALUE; 
        int save = 0; 
        for(int i = 0; i < this.registers.length; i++) {
            if (this.registers[i].isOpen() && this.registers[i].getQueueLength() < shortest) {
                shortest = this.registers[i].getQueueLength();  
                save = i; 
            }
        } 
        this.registers[save].addToQueue(c);   
    }

    public int getLongestQueueLength() {
        int longest = 0;
        for (Register register : registers) {
           int current = register.getQueueLength(); 
            if (longest < current) {
               longest = current; 
            }
        }
        return longest; 
    }

    public void step() {
        for (Register register : registers) {
            register.step();
        }
    }

    public void openNewRegister() {
        for (Register register : registers) {
            if (!register.isOpen()) {
                register.open();
                break;
            }
        }
    }


    public void closeNewRegister() {
        int open = 0; 
        for (Register register : registers) {
            if (register.isOpen()) {
                open++;
            } 
        }
        if (open == 0) {
           return;  
        }
    
        for(int i = this.registers.length; i > 0; i--) {
            if (this.registers[i].isOpen() && !this.registers[i].hasCustomers()) {
                this.registers[i].close(); 
                return;
            }
        }
    }

    public Queue getDoneCustomers() {
        Queue empty = new Queue();
        for (Register register : registers) {
           if (register.currentCustomerIsDone()) {
                empty.enqueue(register.removeCurrentCustomer());
           } 
       }
        return empty;
    }

    @Override 
    public String toString() {
        String s = ""; 
        for (Register register : registers) {
            s += register + "\n\n"; 
        }
        return s; 
    }
    public static void main(String[] args) {
        // Constructor
        Store s = new Store(3);
        System.out.println("Number of registers (expect 3): " + s.registers.length);
        System.out.println("First register open (expect true): " + s.registers[0].isOpen());
        System.out.println("Second register closed (expect false): " + s.registers[1].isOpen());
        System.out.println("Average with no customers (expect 0.0): " + s.getAverageQueueLength());

        // newCustomer with one open register
        Customer a = new Customer(0, 2);
        Customer b = new Customer(0, 1);
        s.newCustomer(a);
        s.newCustomer(b);
        System.out.println("Both in register 0 (expect 2): " + s.registers[0].getQueueLength());
        System.out.println("Closed register got none (expect 0): " + s.registers[1].getQueueLength());
        System.out.println("Average, 1 open register (expect 2.0): " + s.getAverageQueueLength());

        // openNewRegister
        s.openNewRegister();
        System.out.println("Register 1 now open (expect true): " + s.registers[1].isOpen());
        System.out.println("Average, 2 open registers (expect 1.0): " + s.getAverageQueueLength());

        // newCustomer picks the shortest queue
        Customer c = new Customer(0, 1);
        s.newCustomer(c);
        System.out.println("c went to the empty register 1 (expect 1): " + s.registers[1].getQueueLength());
        System.out.println("Register 0 unchanged (expect 2): " + s.registers[0].getQueueLength());

        // step + getDoneCustomers
        s.step(); // a: 2 -> 1, c: 1 -> 0 (done)
        Queue done = s.getDoneCustomers();
        System.out.println("One done after 1 step (expect 1): " + done.length());
        System.out.println("The done customer is c (expect true): " + (done.first() == c));
        System.out.println("c removed from register 1 (expect 0): " + s.registers[1].getQueueLength());

        s.step(); // a: 1 -> 0 (done)
        done = s.getDoneCustomers();
        System.out.println("a done after 2 steps (expect true): " + (done.length() == 1 && done.first() == a));
        System.out.println("b now first in register 0 (expect true): " + (s.registers[0].currentCustomer() == b));

        s.step(); // b: 1 -> 0 (done)
        done = s.getDoneCustomers();
        System.out.println("b done after 3 steps (expect true): " + (done.length() == 1 && done.first() == b));
        System.out.println("Store empty (expect 0.0): " + s.getAverageQueueLength());
        System.out.println("No done customers when empty (expect 0): " + s.getDoneCustomers().length());

    }
    // openNewRegister when all are open must not
}
