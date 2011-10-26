class IfBinarySemaphore {
	private boolean value = true;
	public synchronized void podnies() {
		this.value = true;
		notify();
	}
	public synchronized void opusc() {
		if(!this.value){
			try {
				wait();
			} catch (InterruptedException e) {}
		}
		this.value = false;
	}
}
