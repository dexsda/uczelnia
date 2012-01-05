
public class Scheduler extends Thread 
{

	ActivationQueue activationQueue = new ActivationQueue();
	
	public void enqueue(MethodRequest methodRequest)
	{
		activationQueue.enqueue(methodRequest);
	}
	
	public void run()
	{
		dispatch();
	}
	
	public void dispatch()
	{
		while(true)
		{
			MethodRequest mr = activationQueue.dequeue();
			
			if (mr == null)
				continue;
			
			if (mr.guard())
				mr.call();
			else
				activationQueue.enqueue(mr);		
		}
	}
}
