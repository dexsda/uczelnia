import java.util.concurrent.locks.*;

class NormalList extends GenericList {
	private Link head;
	private ReentrantLock lock;
	public NormalList() {
		this.head = null;
		lock = new ReentrantLock();
	}
	public boolean add(Object o) {
		this.lock.lock();
		if(head==null){
			head = new Link(o);
			this.lock.unlock();
			return true;
		} else {
			Link temp = head;
			while(temp!=null){
				if(temp.next!=null){
					temp=temp.next;
				} else {
					temp.next = new Link(o);
					this.lock.unlock();
					return true;
				}
			}
		}
		this.lock.unlock();
		return false;
	}
	public boolean contains(Object o) {
		Link temp = head;
		this.lock.lock();
		while(temp!=null){
			if(temp.data.equals(o)){
				this.lock.unlock();
				return true;
			} else {
				temp=temp.next;
			}
		}
		this.lock.unlock();
		return false;
	}
	public boolean remove(Object o) {
		Link temp = head;
		Link prev = null;
		this.lock.lock();
		if(temp!=null){
			if(temp.data.equals(o)){
				head=temp.next;
				temp.next = null;
				this.lock.unlock();
				return true;
			} else {
				prev = temp;
				temp = temp.next;
				while(temp!=null){
					if(temp.data.equals(o)){
						prev.next=temp.next;
						temp.next=null;
						this.lock.unlock();
						return true;
					} else {
						prev = temp;
						temp = temp.next;
					}
				}
			}
		}
		this.lock.unlock();
		return false;
	}
	public void print() {
		Link temp = head;
		this.lock.lock();
		while(temp!=null){
			System.out.print(temp.toString() + ", ");
			temp=temp.next;
		}
		System.out.print("\n");
		this.lock.unlock();
	}
	class Link {
		private Object data;
		private Link next;
		public Link(Object o, Link nextLink){
			this.data = o;
			this.next = nextLink;
		}
		public Link(Object o){
			this.data = o;
		}
		@Override
		public String toString() {
			return this.data.toString();
		}
	}
}
