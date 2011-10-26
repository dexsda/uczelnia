import java.util.concurrent.locks.*;

class Counter {
	//private IfBinarySemaphore sem = new IfBinarySemaphore();
	private CounterSemaphore sem = new CounterSemaphore(1);
	private int c = 0;
	public void inc() {
		sem.opusc();
		c++;
		sem.podnies();
	}
	public void dec() {
		sem.opusc();
		c--;
		sem.podnies();
	}
	public int value() {
		return c;
	}
}
