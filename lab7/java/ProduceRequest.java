
public class ProduceRequest extends MethodRequest 
{

	private int e;
	
	public ProduceRequest(Servant servant, Future future, int e)
	{
		super(servant, future);
		this.e = e;
	}
	
	public boolean guard() 
	{
		return !servant.isFull();
	}

	public void call() 
	{
		servant.produce(e, future);
	}

}

