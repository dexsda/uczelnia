import java.util.concurrent.locks.*;

class LockedList extends GenericList {
	private Link head;
	public LockedList() {
		this.head = null;
	}
	public boolean add(Object o) {
		if(head==null){
			head = new Link(o);
			return true;
		} else {
			Link temp = head;
			Link temp2;
			temp.lock.lock();
			while(temp!=null){
				if(temp.next!=null){
					temp.next.lock.lock();
					temp2=temp;
					temp=temp.next;
					temp2.lock.unlock();
				} else {
					temp.next = new Link(o);
					temp.lock.unlock();
					return true;
				}
			}
		}
		return false;
	}
	public boolean contains(Object o) {
		Link temp = head;
		Link temp2;
		if(temp!=null)
			temp.lock.lock();
		while(temp!=null){
			if(temp.next!=null)
				temp.next.lock.lock();
			if(temp.data.equals(o)){
				if(temp.next!=null)
					temp.next.lock.unlock();
				temp.lock.unlock();
				return true;
			} else {
				temp2=temp;
				temp=temp.next;
				temp2.lock.unlock();
			}
		}
		return false;
	}
	public boolean remove(Object o) {
		Link temp = head;
		Link prev = null;
		if(temp!=null){
			temp.lock.lock();
			if(temp.data.equals(o)){
				head=temp.next;
				temp.next = null;
				temp.lock.unlock();
				return true;
			} else {
				prev = temp;
				temp = temp.next;
				while(temp!=null){
					temp.lock.lock();
					if(temp.data.equals(o)){
						prev.next=temp.next;
						temp.next=null;
						prev.lock.unlock();
						temp.lock.unlock();
						return true;
					} else {
						prev.lock.unlock();
						prev = temp;
						temp = temp.next;
					}
				}
				prev.lock.unlock();
			}
		}
		return false;
	}
	public void print() {
		Link temp = head;
		Link temp2;
		temp.lock.lock();
		while(temp!=null){
			System.out.print(temp.toString() + ", ");
			if(temp.next!=null)
				temp.next.lock.lock();
			temp2=temp;
			temp=temp.next;
			temp2.lock.unlock();
		}
		System.out.print("\n");
	}
	class Link {
		private Object data;
		private Link next;
		private ReentrantLock lock;
		public Link(Object o, Link nextLink){
			this.lock = new ReentrantLock();
			this.data = o;
			this.next = nextLink;
		}
		public Link(Object o){
			this.lock = new ReentrantLock();
			this.data = o;
		}
		@Override
		public String toString() {
			return this.data.toString();
		}
	}
}
