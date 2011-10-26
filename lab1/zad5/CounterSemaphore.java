class CounterSemaphore {
	private int value;
	private BinarySemaphore mutex = new BinarySemaphore(true);
	private BinarySemaphore delay = new BinarySemaphore(false);
	public CounterSemaphore(int val) {
		this.value=val;
	}
	public void opusc() {
		mutex.opusc();
		value--;
		if(value<0) {
			mutex.podnies();
			delay.opusc();
		}
		mutex.podnies();
	}
	public void podnies() {
		mutex.opusc();
		value++;
		if(value<=0) 
			delay.podnies();
		mutex.podnies();
	}
}
