
public class Proxy {

	private Scheduler scheduler = new Scheduler();
	private Servant servant;
	
	public Proxy(int n)
	{
		servant = new Servant(n);
		scheduler.start();
	}
	
	public Future consume()
	{
		Future future = new Future();
		ConsumeRequest request = new ConsumeRequest(servant, future);
		scheduler.enqueue(request);
		return future;
	}
	
	public Future produce(int e)
	{
		Future future = new Future();
		ProduceRequest request = new ProduceRequest(servant, future, e);
		scheduler.enqueue(request);
		return future;
	}
}
