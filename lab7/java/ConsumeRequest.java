
public class ConsumeRequest extends MethodRequest 
{

	public ConsumeRequest(Servant s, Future f)
	{
		super(s, f);
	}
	
	public boolean guard() 
	{
		return !servant.isEmpty();
	}

	public void call() 
	{
		servant.consume(future);
	}

}
