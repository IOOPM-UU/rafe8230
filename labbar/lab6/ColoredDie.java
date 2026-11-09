import java.util.Scanner;

public class ColoredDie extends Die {
    private String color;

    public ColoredDie(int numberOfSides, String color) {
        super(numberOfSides);
        this.color = color;
    }

    public String GetColor() {
        return this.color;
    }

    @Override
    public String toString() {
        return "ColoredDie(" + this.color + ", " + this.get() + ")";
    }

    @Override
    public boolean equals(Die otherDie) {

        if (!(otherDie instanceof ColoredDie)) {
            return false;

        }
        ColoredDie other = (ColoredDie) otherDie;
        return super.equals(other) && this.color.equals(other.color);
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.print("Number of sides: ");
        int sides = sc.nextInt();
        Die d = new Die(sides);

    }
}
