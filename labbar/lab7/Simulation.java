package labbar.lab7;
import java.lang.reflect.Array;
import java.time.*;

public class Simulation {
    private int time;
    private Store store;
    private int intensity;
    private int maxGroceries;
    private int thresholdForNewRegisters;

    private int customersServed;
    private int totalWaitTime;
    private int maxWaitTime;

    public Simulation(int intensity, int maxGroceries, int thresholdForNewRegisters) {
        this.time = 0;
        this.intensity = intensity;
        this.maxGroceries = maxGroceries;
        this.thresholdForNewRegisters = thresholdForNewRegisters;
        this.store = new Store(5);
    }

    public int getTime() {
        return this.time;
    }

    public void step() {
        this.store.step();
        if ((int) (Math.random() * 100) < this.intensity) {
            int groceries = (int) (Math.random() * this.maxGroceries) + 1;
            Customer c = new Customer(this.time, groceries);
            this.store.newCustomer(c);
        }
        if (this.store.getAverageQueueLength() > thresholdForNewRegisters) {
            this.store.openNewRegister();
        }

        Queue done = this.store.getDoneCustomers();
        while (done.length() > 0) {
            Customer c = done.dequeue();
            int wait = this.time - c.getBornTime();
            this.customersServed++;
            this.totalWaitTime += wait;
            if (wait > this.maxWaitTime) {
                this.maxWaitTime = wait;
            }
        }
            
        this.time++;
    }

    public int getCustomersServed() {
        return this.customersServed;
    }

    public float getAverageWaitTime() {
        if (this.customersServed == 0) {
            return 0;
        }
        return (float) this.totalWaitTime / this.customersServed;
    }

    public int getMaxWaitTime() {
        return this.maxWaitTime;
    }

    @Override 
    public String toString() {
        return this.store 
            + "Number of customers served" + this.customersServed + "\n"
            + "Max wait time" + this.maxWaitTime + "\n"
            + "Average wait time" + this.getAverageWaitTime(); 
    }


    public static void test() {
        // 1. Time moves forward
        Simulation s = new Simulation(50, 5, 2);
        for (int i = 0; i < 10; i++)
            s.step();
        System.out.println("Time after 10 steps (expect 10): " + s.getTime());

        // 2. Intensity 0: nobody ever arrives
        Simulation none = new Simulation(0, 5, 2);
        for (int i = 0; i < 100; i++)
            none.step();
        System.out.println("Intensity 0, served (expect 0): " + none.getCustomersServed());
        System.out.println("Intensity 0, average queue (expect 0.0): " + none.store.getAverageQueueLength());
        System.out.println("Intensity 0, average wait (expect 0.0): " + none.getAverageWaitTime());

        // 3. Intensity 100 + 1 item each: one customer arrives and one leaves every
        // step
        Simulation steady = new Simulation(100, 1, 1000);
        for (int i = 0; i < 100; i++)
            steady.step();
        System.out.println("Intensity 100, 1 item, served (expect 99): " + steady.getCustomersServed());
        System.out.println("Queue never builds up (expect 1.0): " + steady.store.getAverageQueueLength());

        // 4. Intensity 50: about half the steps get a customer (random, so a range)
        Simulation half = new Simulation(50, 1, 1);
        for (int i = 0; i < 2000; i++)
            half.step();
        int served = half.getCustomersServed();
        System.out.println("Intensity 50, served between 850 and 1150 (expect true): "
                + (served > 850 && served < 1150) + "  (was " + served + ")");

        // 5. A low threshold opens registers, which keeps queues shorter
        Simulation fewRegisters = new Simulation(100, 10, 1000);
        Simulation manyRegisters = new Simulation(100, 10, 1);
        for (int i = 0; i < 200; i++) {
            fewRegisters.step();
            manyRegisters.step();
        }
        System.out.println("Low threshold gives shorter waits (expect true): "
                + (manyRegisters.getAverageWaitTime() < fewRegisters.getAverageWaitTime()));

        // 6. Statistics are sensible
        System.out.println("Max wait >= average wait (expect true): "
                + (fewRegisters.getMaxWaitTime() >= fewRegisters.getAverageWaitTime()));
        System.out.println("Average wait not negative (expect true): "
                + (fewRegisters.getAverageWaitTime() >= 0));
        System.out.println("Served never more than steps (expect true): "
                + (fewRegisters.getCustomersServed() <= fewRegisters.getTime()));
    }
}

