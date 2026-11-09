import java.util.Scanner;

public class PairOfDice {
   private Die first; 
   private Die second; 
   
   public PairOfDice(int numberOfSides)
   {
     String[] colors = {"red", "blue", "yellow", "green", "black", "white"};
     String randomColor = colors[(int) (Math.random() * colors.length)]; 
        this.first = new ColoredDie(numberOfSides, "red");
        this.second = new ColoredDie(numberOfSides, randomColor);
   }
   public int roll()
   {
        return this.first.roll() + this.second.roll(); 
   }
   public int getFirst()
   {
        return this.first.get(); 
   }
    
   public int getSeconds()
   {
        return this.second.get(); 
   }
   
   public String toString()
   {
        return "Die(" + this.first + ", " + this.second + ")"; 
   }

    public static void main(String [] args)
   {
      Scanner sc = new Scanner(System.in);
      System.out.print("Number of sides: ");
      int sides = sc.nextInt();
        PairOfDice pair = new PairOfDice(sides);  
        System.out.println("Rolled: " + pair.roll());
        System.out.println("First: " + pair.getFirst());
        System.out.println("Second: " + pair.getSeconds());
        System.out.println(pair);
    
   }
}
