import java.util.Iterator;

public class Maximizer extends Stem {
	public Maximizer() {
		super();
		alpha = Integer.MIN_VALUE;
		beta = Integer.MAX_VALUE;
		type = "Maximizer";
		setValue(Integer.MIN_VALUE);
	}
	@Override
	public int getValue() {
		if(super.getValue() == Integer.MIN_VALUE) {
			setValue(getMax());
		}
		return super.getValue();
	}
	public void addMinimizer(Minimizer child) {
		children.add(child);
	}
	public void addMinimizer() {
		Minimizer child = new Minimizer();
		children.add(child);
	}
	private int getMax() {
		int max = Integer.MIN_VALUE;
		Iterator<Node> iter = children.iterator();
		for(int i = 0; i<children.size(); i++) {
			Node child = iter.next();
			child.setAlpha(alpha);
			child.setBeta(max);
			max = max(child.getValue(),max);
			alpha = max(max,alpha);
			if(max>beta) {
				break;
			}
//			pruning
		}
		return max;
	}
	private int max(int a, int b) {
		return a>b?a:b;
	}

	@Override
	protected void addStemNode() {
		addMinimizer();
	}
}
