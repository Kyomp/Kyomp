import java.util.Scanner;

public interface Node {
	public int getValue();
	public void setAlpha(int alpha);
	public void setBeta(int beta);
	public void traverse(Scanner sc);
}
