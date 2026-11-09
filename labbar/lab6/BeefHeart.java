public class BeefHeart {
    public static void main(String[] args) {
        String a = "Beefheart";
        String b = "Beefheart";
        String c = "heart";
        String d = "beef" + c;
        int i = 1;
        int j = 1; 
        Integer k = 2; 
        Integer l = i + j; 

        System.out.println(a == b);
        System.out.println(a.equals(b));
        System.out.println(a == d);
        System.out.println(a.equals(d));
        System.out.println(i == j);
        System.out.println(k == i + j);
        System.out.println(k.equals(i + j));
        System.out.println(k == l);
        System.out.println(k.equals(l));

    } 
}
