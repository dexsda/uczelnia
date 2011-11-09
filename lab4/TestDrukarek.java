import java.util.Random;

public class TestDrukarek {
	public Random gen = new Random();
	public static void main(String[] args){
		TestDrukarek tt = new TestDrukarek();
		MonitorDrukarek mon = new MonitorDrukarek(Integer.parseInt(args[0]));
		for(int i=0; i<Integer.parseInt(args[1]); i++){
			PrintThread thread = tt.new PrintThread(i,mon);
			thread.start();
		}
		for(;;){
			mon.print_state();
			try{
				Thread.sleep(1000);
			} catch (InterruptedException e) {}
		}
	}
	public void print(int n,int index){
		int j = gen.nextInt(5)+1;
		System.out.println("["+index+"] Drukuje na drukarce "+n+" przez 0."+j+" sek...");
		try {
			Thread.sleep(j*100);
		} catch(InterruptedException e) {}
	}
	class PrintThread extends Thread {
		MonitorDrukarek Monitor_Drukarek;
		private int index;
		PrintThread(int i,MonitorDrukarek mon) {
			this.Monitor_Drukarek = mon;
			this.index = i;
		}
		public void run(){
			for(;;){
				try {
					Thread.sleep(1000);//tworzenie zadania do druku
				} catch(InterruptedException e) {}
				int num_print = Monitor_Drukarek.reserve();
				print(num_print,index);
				Monitor_Drukarek.free();
			}
		}
	}
}
