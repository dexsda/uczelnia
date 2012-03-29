import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Random;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;

public class Client {
	public static void main(String[] args) {
		if (args.length != 2) {
			System.out.println("poprawne argumenty: [IP] [port]");
			System.exit(-1);
		}
		Socket socket = null;
		Random generator = new Random();
		try {
			//socket i streamy
			socket = new Socket(args[0],Integer.parseInt(args[1]));
			DataInputStream  in  = new DataInputStream(socket.getInputStream());
			DataOutputStream out = new DataOutputStream(socket.getOutputStream());
			// bajt
			byte tempchar = (byte)generator.nextInt(25);
			System.out.println("Sent byte: " + tempchar);
			out.writeByte(tempchar);
			tempchar = in.readByte();
			System.out.println("Received: " + tempchar);
			// short
			short tempshort = (short)generator.nextInt(2500);
			System.out.println("Sent short: " + tempshort);
			out.writeShort(tempshort);
			tempshort = in.readShort();
			System.out.println("Received: " + tempshort);
			// int
			int tempint = generator.nextInt(250000);
			System.out.println("Sent int: " + tempint);
			out.writeInt(tempint);
			tempint = in.readInt();
			System.out.println("Received: " + tempint);
			// long
			long templong = generator.nextLong();
			System.out.println("Sent long: " + templong);
			out.writeLong(templong);
			templong = in.readLong();
			System.out.println("Received: " + templong);
		} catch (NumberFormatException e) {
			e.printStackTrace();
		} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if(socket != null) {
				try {
					socket.close(); 
				} catch(IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
