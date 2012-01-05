
public abstract class MethodRequest 
{

	protected Future future;
	protected Servant servant;

	public MethodRequest(Servant s, Future f)
	{
		servant = s;
		future = f;
	}
	
	
	public abstract boolean guard();
	public abstract void call();
}


