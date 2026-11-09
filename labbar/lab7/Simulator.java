package labbar.lab7;

public class Simulator {
    public static void main(String[] args) throws InterruptedException {
        int steps = 100;
        int intensity = 90;
        int maxGroceries = 6;
        int thresholdForNewRegister = 4;

        Simulation s = new Simulation(intensity, maxGroceries, thresholdForNewRegister);
        for (int i = 0; i < steps; i++) {
            s.step();
            System.out.println(s);
            Thread.sleep(500); 
        }
        System.out.println("\nSimulation finished!");
    }
}
