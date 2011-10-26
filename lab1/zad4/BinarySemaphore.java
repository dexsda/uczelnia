class BinarySemaphore {
	private boolean value = true;
	public synchronized void podnies() {
		this.value = true;
		notify();
	}
	public synchronized void opusc() {
		while(!this.value){
			try {
				wait();
			} catch (InterruptedException e) {}
		}
		this.value = false;
	}
}
