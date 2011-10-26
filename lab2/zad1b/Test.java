import java.util.concurrent.Semaphore;
import java.util.Random;

public class Test {
	int[] buffer = new int[10];
	boolean filled = false;
	boolean empty = true;
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
				synchronized(buffer) {
					if(!filled){
						for(int i=0; i<10; i++){
							if(buffer[i]==0){
								System.out.println(i+" --> "+temp);
								buffer[i]=temp;
								empty = false;
								if(i==9)
									filled = true;
								break;
							}
						}
					}
				}
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
				synchronized(buffer) {
					if(!empty){
						for(int j=9; j>=0; j--){
							if(buffer[j]!=0){
								System.out.println(j+" <-- "+buffer[j]);
								buffer[j]=0;
								if(j==0)
									empty = true;
								filled = false;
								break;
							}
						}
					}
				}
				try {
					Thread.sleep(gen.nextInt(500));
				} catch (InterruptedException e) {}
			}
		}
	}
}
