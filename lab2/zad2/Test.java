import java.util.concurrent.Semaphore;
import java.util.Random;

public class Test {
	final static int N = 10;
	final static int M = 5; //procesow
	int[] buffer = new int[N];
	Semaphore[] semaphores = new Semaphore[M];
	public static void main(String args[]) {
		Test pc = new Test();
		pc.semaphores[0]=new Semaphore(N);
		for(int j=1; j<M; j++){
			pc.semaphores[j]=new Semaphore(0);
		}
		Producer producer = pc.new Producer();
		producer.start();
		for(int j=1; j<M-1; j++){
			Worker worker = pc.new Worker(j);
			worker.start();
		}
		Consumer consumer = pc.new Consumer();
		consumer.start();
	}

	class Producer extends Thread {
		Random gen = new Random();
		int i = 0;
		public void run() {
			while(true){
				int temp = gen.nextInt(1000);
				try{
					semaphores[0].acquire();
				} catch (InterruptedException e) {}
					System.out.println(i+" --> "+temp);
					buffer[i]=temp;
					i=(i+1)%N;
				semaphores[1].release();
				try {
					Thread.sleep(gen.nextInt(500));
				} catch (InterruptedException e) {}
			}
		}
	}
	class Worker extends Thread {
		Random gen = new Random();
		int i=0;
		int index;
		Worker(int _index){
			this.index=_index;
		}
		public void run() {
			while(true){
				try {
					semaphores[index].acquire();
				} catch (InterruptedException e) {}
					System.out.println(i+" <"+index+"> "+buffer[i]+"++");
					buffer[i]++;
					i=(i+1)%N;
				semaphores[(index+1)%M].release();
				try {
					Thread.sleep(gen.nextInt(500));
				} catch (InterruptedException e) {}
			}
		}
	}
	class Consumer extends Thread {
		Random gen = new Random();
		int i=0;
		public void run() {
			while(true){
				try {
					semaphores[M-1].acquire();
				} catch (InterruptedException e) {}
					System.out.println(i+" <-- "+buffer[i]);
					buffer[i]=0;
					i=(i+1)%N;
				semaphores[0].release();
				try {
					Thread.sleep(gen.nextInt(500));
				} catch (InterruptedException e) {}
			}
		}
	}
}
