import java.util.concurrent.LinkedBlockingQueue;


public class ActivationQueue 
{

	private LinkedBlockingQueue<MethodRequest> queue = new LinkedBlockingQueue<MethodRequest>();
	
	public void enqueue(MethodRequest methodRequest)
	{
		queue.add(methodRequest);
	}
	
	public MethodRequest dequeue()
	{
		try 
		{
			return queue.take();
		} 
		catch (InterruptedException e) 
		{
			e.printStackTrace();
		}
		return null;
	}
}
