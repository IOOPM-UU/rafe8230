import java.util.Scanner; 

public class MyDieTest {
    public static void main(String [] args) {
      Scanner sc = new Scanner(System.in);
      System.out.print("Number of sides: ");
      int sides = sc.nextInt();
      for(int i = 0; i < 10; i++) {
        Die d = new Die(sides);
        System.out.println("Alea iacta est: " + d.roll());
      }
  }
}