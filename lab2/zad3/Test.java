import java.util.concurrent.Semaphore;
import java.util.Random;

public class Test {
	final static int N = 10;
	int[] buffer = new int[2*N];
	int pos_prod = 0;
	int pos_cons = 0;
	int filled = 0;
	Semaphore first_consumer = new Semaphore(0);
	Semaphore first_producer = new Semaphore(1);
	Semaphore consumers = new Semaphore(1);
	Semaphore producers = new Semaphore(0);

	public static void main(String args[]) {
		Test pc = new Test();
		for(int j=0; j<3; j++) {
			Producer producer = pc.new Producer(j);
			producer.start();
		}
		for(int j=0; j<2; j++) {
			Consumer consumer = pc.new Consumer(j);
			consumer.start();
		}
	}

	class Producer extends Thread {
		Random gen = new Random();
		int index;
		Producer(int ind){
			this.index=ind;
		}
		public void run() {
			while(true){
				int temp = gen.nextInt(N);
				try{
					if(first_producer.availablePermits()!=1)
						producers.acquire();
					while((2*N-filled)<temp)
						first_producer.acquire();
				} catch (InterruptedException e) {}
					System.out.println(pos_prod+" -"+index+"> "+temp);
					filled+=temp;
					for(;temp!=0; temp--){
						buffer[pos_prod]=1;
						pos_prod=(pos_prod+1)%(2*N);
					}
				producers.release();
				first_consumer.release();
				try {
					Thread.sleep(gen.nextInt(500));
				} catch (InterruptedException e) {}
			}
		}
	}
	class Consumer extends Thread {
		Random gen = new Random();
		int index;
		Consumer(int ind){
			this.index=ind;
		}
		public void run() {
			while(true){
				int temp = gen.nextInt(N);
				try {
					if(first_consumer.availablePermits()!=1)
						consumers.acquire();
					while(filled<temp)
						first_consumer.acquire();
				} catch (InterruptedException e) {}
					System.out.println(pos_cons+" <"+index+"- "+temp);
					filled-=temp;
					for(;temp!=0; temp--){
						buffer[pos_cons]=0;
						pos_cons=(pos_cons+1)%(2*N);
					}
				consumers.release();
				first_producer.release();
				try {
					Thread.sleep(gen.nextInt(500));
				} catch (InterruptedException e) {}
			}
		}
	}
}
