public class CounterTest {
	public static void main(String args[]) {
		Counter CounterTestObject = new Counter();
		CounterThread t1 = new CounterThread(CounterTestObject,true);
		t1.start();
		CounterThread t2 = new CounterThread(CounterTestObject,false);
		t2.start();
	}
}
