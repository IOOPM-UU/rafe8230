package labbar.lab7;

public class Queue {
    
    private static class Node {
        Customer element;
        Node next;

        Node(Customer element) {
            this.element = element;
            this.next = null;
        }
   }

    private Node first;
    protected Node last;
    private int length;

    Queue() {
        this.first = null;
        this.last = null;
        this.length = 0;
    }

    public int length(){
        return this.length;
    }

    public void enqueue(Customer c) {
        Node n = new Node(c);
        if (this.length == 0) {
            this.first = n;
        } else {
            this.last.next = n;
        }
        this.last = n;
        this.length++;
    }

    public class EmptyQueueExeption extends RuntimeException {}

    public Customer dequeue() {
        if (this.first == null) {
            throw new EmptyQueueExeption(); 
        }

        Customer temp = this.first(); 
        this.first = this.first.next;
        if (this.first == null) {
            this.last = null;
        }
        this.length--;
        return temp; 
    }

    public Customer first() {
        return this.first.element;
    }

    public Customer last() {
        return this.last.element; 
    }
    public static void main(String[] args) {
        Queue q = new Queue();
        System.out.println("Empty length (expect 0): " + q.length());

        Customer a = new Customer(0, 3);
        Customer b = new Customer(1, 5);
        Customer c = new Customer(2, 1);
        q.enqueue(a);
        q.enqueue(b);
        q.enqueue(c);
        System.out.println("After 3 enqueues (expect 3): " + q.length());
        System.out.println("First is a (expect true): " + (q.first() == a));

        System.out.println("Dequeue gives a (expect true): " + (q.dequeue() == a));
        System.out.println("Dequeue gives b (expect true): " + (q.dequeue() == b));
        System.out.println("Length (expect 1): " + q.length());

        System.out.println("Dequeue gives c (expect true): " + (q.dequeue() == c));
        System.out.println("Empty again (expect 0): " + q.length());

        // Refill after emptying: checks that last was reset correctly
        q.enqueue(a);
        System.out.println("Refilled, first is a (expect true): " + (q.first() == a));
        System.out.println("Length (expect 1): " + q.length());
    }
}
