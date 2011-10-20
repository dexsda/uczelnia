import java.util.concurrent.locks.*;

class Counter {
	private final Lock lock = new ReentrantLock();
	private int c = 0;
	public void inc() {
		lock.lock();
		try {
			c++;
		} finally {
			lock.unlock();
		}
	}
	public void dec() {
		lock.lock();
		try {
			c--;
		} finally {
			lock.unlock();
		}
	}
	public int value() {
		return c;
	}
}
