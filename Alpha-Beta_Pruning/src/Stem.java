import java.util.LinkedList;
import java.util.Scanner;

public abstract class Stem implements Node{
	protected String type;
	protected int alpha;
	protected int beta;
	protected LinkedList<Node> children;
	private int value;
	public Stem() {
		children = new LinkedList<Node>();
	}
	@Override
	public int getValue() {
		return value;
	}
	protected void setValue(int value) {
		this.value = value;
	}
	@Override
	public void setAlpha(int alpha) {
		this.alpha = alpha;
		
	}
	@Override
	public void setBeta(int beta) {
		this.beta = beta;
	}
	public void addLeafNode(int value){
		Leaf child = new Leaf(value);
		children.add(child);
	}
	public void addLeafNode(Leaf child) {
		children.add(child);
	}
	protected abstract void addStemNode();
	
	public void traverse(Scanner sc) {
		int input = 0;
		while(input != 4) {
			print();
			System.out.println("1. Add leaf node");
			System.out.println("2. Add stem node");
			System.out.println("3. Traverse to child");
			System.out.println("4. Back");
			input = sc.nextInt();
			switch(input) {
			case 1: 
				System.out.printf("Value: ");
				addLeafNode(sc.nextInt());
				break;
			case 2:
				addStemNode();
				break;
			case 3:
				System.out.printf("Select child [0-%d]: ", children.size()-1);
				int pick = sc.nextInt();
				children.get(pick).traverse(sc);
				break;
			}
		}
	}
	private void print() {
		System.out.println("Type: "+type);
		System.out.println("Value: " + String.valueOf(value));
	}
}
