import java.util.Scanner;

public class Leaf implements Node {
	
	private int value;
	public Leaf(int value) {
		this.value = value;
	}
	@Override
	public int getValue() {
		return value;
	}
	@Override
	public void setAlpha(int alpha) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void setBeta(int beta) {
		// TODO Auto-generated method stub
		
	}
	@Override
	public void traverse(Scanner sc) {
		print();
	}
	private void print() {
		System.out.println("Type: Leaf Node");
		System.out.printf("Value: %d\n",value);
	}
}
