import java.util.Scanner;

public class Main {

	public static void main(String[] args) {
		Maximizer root = new Maximizer();
		Scanner sc = new Scanner(System.in);
		root.traverse(sc);
		System.out.println(root.getValue());
		root.traverse(sc);
		sc.close();
	}

}
