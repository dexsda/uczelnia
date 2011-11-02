import java.util.Random;

public class Test {
	public static void main(String[] args){
		Test tt = new Test();
		GenericList list = null;
		if(args[2].equals(new String("fine"))){
			list = new LockedList();
		} else {
			list = new NormalList();
		}
		Random gen = new Random();
		for(int i=0; i<100; i++){
			list.add(i);
		}
		Thread[] array = new Thread[Integer.parseInt(args[0])];
		for(int i=0; i<Integer.parseInt(args[0]); i++){
			FineThread thread = tt.new FineThread(Integer.parseInt(args[1]),list);
			thread.start();
			array[i]=thread;
		}
		for(int i=0; i<Integer.parseInt(args[0]); i++){
			try {
				array[i].join();
			} catch (InterruptedException e) {
				;
			}
		}
		list.print();
	}
	class FineThread extends Thread {
		private GenericList list;
		private int iterations;
		private Random gen = new Random();
		FineThread(int it, GenericList ls) {   
			this.list = ls; 
			this.iterations = it;
		}
		public void run(){
			for(int j=0; j<iterations; j++){
				int temp = gen.nextInt(1);//zmienic dla innego testu
				int temp2 = gen.nextInt(101);
				if(temp==0) {
					System.out.println("Contains "+temp2+": " + list.contains(temp2)+" ("+j+")");
				} else if(temp==1){
					System.out.println("Added "+temp2+": " +list.add(temp2)+" ("+j+")");
				} else if(temp==2) {
					System.out.println("Removed "+temp2+": " + list.remove(temp2)+" ("+j+")");
				}
			}
		}
	}
}
