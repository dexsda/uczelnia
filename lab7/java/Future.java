import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.ReentrantLock;


public class Future 
{

	private boolean futura;
	private Object whatHasCome;
	private ReentrantLock lock = new ReentrantLock();
	private Condition condition = lock.newCondition();
	
	public boolean hasBeenSeen()
	{
		return futura;
	}
	
	public void setObject(Object object)
	{
		this.whatHasCome = object;
		lock.lock();
		futura = true;
		condition.signalAll();
		lock.unlock();
	}
	
	public Object get()
	{
		lock.lock();
		
		if (!futura)
			try 
			{
				condition.await();
			} 
			catch (InterruptedException e) 
			{
				e.printStackTrace();
			}
		
		lock.unlock();
		
		return whatHasCome;
	}
}
