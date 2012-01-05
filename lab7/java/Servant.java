import java.util.LinkedList;

public class Servant 
{

	private LinkedList<Integer> buffer = new LinkedList<Integer>();
	private static int bufferSize;
	
	public Servant()
	{
		bufferSize=5;
	}
	
	public Servant(int n)
	{
		bufferSize=n;
	}
	
	public void consume(Future f)
	{
		f.setObject(buffer.removeFirst());
	}
	
	public void produce(int e, Future f)
	{		
			buffer.addLast(e);
			f.setObject(true);
	}
	
	public boolean isEmpty()
	{
		return buffer.isEmpty();
	}
	
	public boolean isFull()
	{
		return buffer.size() >= bufferSize;
	}
}