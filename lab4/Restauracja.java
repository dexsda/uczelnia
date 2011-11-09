import java.util.Random;

public class Restauracja {
	public Random gen = new Random();
	public static void main(String[] args){
		Restauracja tt = new Restauracja();
		Kelner mon = new Kelner(Integer.parseInt(args[0]));
		for(int i=0; i<Integer.parseInt(args[0])*2; i++){
			PrintThread thread = tt.new PrintThread(i,mon);
			thread.start();
		}
	}
	public void print(int index){
		int j = gen.nextInt(5)+1;
		System.out.println("omnom" + index);
		try {
			Thread.sleep(j*100);
		} catch(InterruptedException e) {}
	}
	class PrintThread extends Thread {
		Kelner kelner;
		private int index;
		PrintThread(int i,Kelner mon) {
			this.kelner = mon;
			this.index = i;
		}
		public void run(){
			for(;;){
				try {
					Thread.sleep(1000);//tworzenie zadania do druku
				} catch(InterruptedException e) {}
				kelner.reserve(index/2);
				print(index);
				kelner.free();
			}
		}
	}
}
