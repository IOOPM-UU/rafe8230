import java.util.Scanner;

public class NameOrder {
   public static void main(String [] args) {
        Scanner sc = new Scanner(System.in); 
        System.out.print("Enter the first name: ");
        String name1 = sc.next(); 
        
        System.out.print("Enter the second name: ");
        String name2 = sc.next(); 
        if (name1.compareToIgnoreCase(name2) == 0) {
           System.out.println("The names are identical"); 
        }         
        else 
        {
            System.out.println("The names are not identical");
        }
   } 

}
