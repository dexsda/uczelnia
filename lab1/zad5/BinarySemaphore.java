class BinarySemaphore {
	private boolean value = true;
	public BinarySemaphore(boolean bool) {
		this.value=bool;
	}
	public synchronized void podnies() {
		this.value = true;
		if(this.value) {
			notify();
		}
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
