import java.util.concurrent.locks.*;
import java.util.Random;

class MonitorDrukarek {
	private final int M;
	private ReentrantLock[] locks;
	private Random gener = new Random();
	public MonitorDrukarek(int m) {
		this.M = m;
		this.locks = new ReentrantLock[m];
		for(int i=0; i<m; i++){
			this.locks[i]=new ReentrantLock();
		}
	}
	public int reserve(){
		for(int i=0; i<this.M; i++){
			if(!locks[i].isLocked()){
				locks[i].lock();
				return i;
			}
		}
		int rand = gener.nextInt(this.M);
		locks[rand].lock();
		return rand;
	}
	public void free(){
		for(int i=0; i<this.M; i++){
			if(locks[i].isHeldByCurrentThread()){
				locks[i].unlock();
				break;
			}
		}
	}
	public void print_state(){
		System.out.print("[");
		for(int i=0; i<this.M; i++){
			if(locks[i].isLocked()){
				System.out.print("#");
			} else {
				System.out.print(" ");
			}
		}
		System.out.print("]\n");
	}
}
