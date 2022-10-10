import java.util.Iterator;

public class Minimizer extends Stem {
	public Minimizer() {
		super();
		alpha = Integer.MIN_VALUE;
		beta = Integer.MAX_VALUE;
		type = "Minimizer";
		setValue(Integer.MAX_VALUE);
	}
	@Override
	public int getValue() {
		if(super.getValue() == Integer.MAX_VALUE) {
			setValue(getMin());
		}
		return super.getValue();
	}
	public void addMaximizer(Maximizer child) {
		children.add(child);
	}
	public void addMaximizer() {
		Maximizer child = new Maximizer();
		children.add(child);
	}
	private int getMin() {
		int min = Integer.MAX_VALUE;
		Iterator<Node> iter = children.iterator();
		for(int i = 0; i<children.size(); i++) {
			Node child = iter.next();
			child.setAlpha(alpha);
			child.setBeta(beta);
			min = min(child.getValue(),min);
			beta = min(min,alpha);
			if(min<alpha) {
				break;
			}
//			pruning
		}
		return min;
	}
	private int min(int a, int b) {
		return a<b?a:b;
	}

	@Override
	protected void addStemNode() {
		addMaximizer();
	}
}
