class CounterThread extends Thread {
	private Counter CounterObject;
	private boolean increment = true;
	CounterThread(Counter co, boolean inc) {   
		this.CounterObject = co; 
		this.increment = inc;
	}
	public void run(){
		for(int i=0; i<100000000; i++){
			if(increment){
				CounterObject.inc();
			} else {
				CounterObject.dec();
			}
		}
		System.out.println(CounterObject.value());
	}
}
