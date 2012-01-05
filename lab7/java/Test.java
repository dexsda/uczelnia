import java.util.Random;
import java.util.Timer;

class Producer extends Thread
{
	private Proxy proxy;
	private int id;
	private int times;
	private Random random = new Random();
	
	Producer(Proxy proxy, int id, int times)
	{
		this.proxy = proxy;
		this.id = id;
		this.times = times;
	}
	
	public void run()
	{
		while(times>0)
		{
			int bla = random.nextInt();
			Object a = proxy.produce(bla).get();
			System.out.println(id + " produced number: " + bla +" , and future returned: " + a);
			times--;
		}
	}
}

class Consumer extends Thread
{
	private Proxy proxy;
	private int id;
	private Random random = new Random();
	private int times;
	
	Consumer(Proxy proxy, int id, int times)
	{
		this.times = times;
		this.proxy = proxy;
		this.id = id;
	}
	
	public void run()
	{	
		while(times>0)
		{
			Future f = proxy.consume();
			System.out.println(id + " consumed number: " + f.get());
			times--;
		}
	}
}



public class Test 
{

	static int bufforSize=4;
	static int actions = 500;
	static int threads = 10;
	public static void main(String[] args)
	{
		Proxy proxy = new Proxy(bufforSize);
		Thread[] producers = new Thread[threads];
		Thread[] consumers = new Thread[threads];
		for (int i=0;i<threads;i++)
		{
			producers[i] =new Producer(proxy, i, actions);
			consumers[i] =new Consumer(proxy, i, actions);
		}
		
		long start = System.nanoTime();
		for(int i=0;i<threads;i++)
		{
			producers[i].start();
			consumers[i].start();
		}
		for(int i=0;i<threads;i++)
		{
			try
			{
				producers[i].join();
				consumers[i].join();
			}
			catch (Exception e)
			{
				e.printStackTrace();
			}
		}
		System.out.println((System.nanoTime()-start)/1000);
	}
	
}
