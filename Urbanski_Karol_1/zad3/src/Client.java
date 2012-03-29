import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;

public class Client {
	static MulticastSocket socket;
	static String nick="default";
	static InetAddress address;
	static int port;
	public static void main(String[] args) {
		if (args.length != 3) {
			System.out.println("zle argumenty! [ip] [port] [nick]");
			System.exit(-1);
		}
		try {
			address = InetAddress.getByName(args[0]);
			port = Integer.parseInt(args[1]);
			socket = new MulticastSocket(port); //Socket
			socket.joinGroup(address); //dolacz do grupy
			nick=args[2];
			Runnable sender = new Runnable() { //watek1
				public void run() {
					try {
						while (true) {
							String line = System.console().readLine();
							String line2 = nick + "\n" + line;//separujemy nicka i linie znakiem nowej linii, bo na pewno nie będzie jej nigdzie w samej linii
							byte[] message = line2.getBytes(); //przesylamy jako bajty
							DatagramPacket packet = new DatagramPacket(message, message.length, address, port);
							socket.send(packet);
						}
					} catch (Exception e) {}
				}
			};
			Runnable reader = new Runnable() { //watek2
				public void run() {
					try {
						while (true) {
							byte[] buf = new byte[1024];
							DatagramPacket recv = new DatagramPacket(buf, buf.length);
							socket.receive(recv); //odbieramy pakiet
							String lines[] = (new String(buf)).split("\\r?\\n"); //dzielimy pakiet
							if(!nick.equals(lines[0])){
								System.out.println("<"+lines[0]+"> "+lines[1]); //wypisujemy, jezeli trzeba
							}
						}
					} catch (Exception e) {}
				}
			};
			Thread thr1 = new Thread(sender);
			Thread thr2 = new Thread(reader);
			thr1.start();
			thr2.start();
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(-1);
		}
	}
}
