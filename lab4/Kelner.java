import java.util.concurrent.locks.*;
import java.util.Random;

class Kelner {
	private final int N;
	private Object[] mon;
	private boolean[] free;
	private Object table = new Object();
	private int table_people = 0;
	public Kelner(int n) {
		this.N = n;
		this.mon = new Object[n];
		this.free = new boolean[n];
		for(int i=0; i<n; i++){
			this.mon[i]=new Object();
			this.free[i]=false;
		}
	}
	public void reserve(int i) throws InterruptedException{
		synchronized(mon[i]) {
			if(!free[i]){
				free[i] = !free[i];
				mon[i].wait();
			} else {
				synchronized(table){
					while(table_people > 0){
						table.wait();
					}
					table_people=2;
					mon[i].notifyAll();
				}
			}
		}
	}
	public void free(int i){
		synchronized(table){
			table_people--;
			if(table_people==0){
				table.notifyAll();
				free[i]=false;
			}
		}
	}
}
