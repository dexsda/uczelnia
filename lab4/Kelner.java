import java.util.concurrent.locks.*;
import java.util.Random;

class Kelner {
	private final int N;
	private ReentrantLock lock = new ReentrantLock();
	private Condition[] cond;
	private Condition con = lock.newCondition();
	private ReentrantLock[] locks;
	private int[] count;
	private boolean free = true;
	private Random gener = new Random();
	public Kelner(int n) {
		this.N = n;
		this.locks = new ReentrantLock[n];
		this.cond = new Condition[n];
		this.count = new int[n];
		for(int i=0; i<n; i++){
			this.locks[i]=new ReentrantLock();
			this.cond[i]=this.locks[i].newCondition();
			this.count[i]=0;
		}
	}
	public void reserve(int i){
		locks[i].lock();
		count[i]++;
		try{
			lock.lock();
			while(count[i]==2 && !free){
				con.await();
			}
			lock.unlock();
		} catch (InterruptedException e){}
		if(count[i]==2){
			cond[i].signal();
		}
		try {
			while(count[i] != 2)
				cond[i].await();
		} catch (InterruptedException e) {}
		locks[i].unlock();
	}
	public void free(){
		lock.lock();
		con.signal();
		lock.unlock();
	}
}
