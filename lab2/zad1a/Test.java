import java.util.concurrent.Semaphore;
import java.util.Random;

public class Test {
	int[] buffer = new int[10];
	Semaphore filled = new Semaphore(0);
	Semaphore empty = new Semaphore(10);
	public static void main(String args[]) {
		Test pc = new Test();
		Producer producer = pc.new Producer();
		Consumer consumer = pc.new Consumer();
		producer.start();
		consumer.start();
	}

	class Producer extends Thread {
		Random gen = new Random();
		public void run() {
			while(true){
				int temp = gen.nextInt(1000);
				try{
					empty.acquire();
				} catch (InterruptedException e) {}
					for(int i=0; i<10; i++){
						if(buffer[i]==0){
							System.out.println(i+" --> "+temp);
							buffer[i]=temp;
							break;
						}
					}
				filled.release();
				try {
					Thread.sleep(gen.nextInt(500));
				} catch (InterruptedException e) {}
			}
		}
	}
	class Consumer extends Thread {
		Random gen = new Random();
		public void run() {
			while(true){
				try {
					filled.acquire();
				} catch (InterruptedException e) {}
					for(int j=9; j>=0; j--){
						if(buffer[j]!=0){
							System.out.println(j+" <-- "+buffer[j]);
							buffer[j]=0;
							break;
						}
					}
				empty.release();
				try {
					Thread.sleep(gen.nextInt(500));
				} catch (InterruptedException e) {}
			}
		}
	}
}
