class Counter {
	private int c = 0;
	public synchronized void inc() {
		c++;
	}
	public synchronized void dec() {
		c--;
	}
	public synchronized int value() {
		return c;
	}
}
