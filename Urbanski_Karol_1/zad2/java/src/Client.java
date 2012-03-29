import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Random;
import java.io.DataInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;

public class Client {
	public static void main(String[] args) {
		if (args.length != 3) {
			System.out.println("poprawne argumenty: [IP] [port] [name]");
			System.exit(-1);
		}
		Socket socket = null;
		Random generator = new Random();
		try {
			//socket i streamy
			socket = new Socket(args[0],Integer.parseInt(args[1]));
			DataInputStream  in  = new DataInputStream(socket.getInputStream());
			FileOutputStream fos = new FileOutputStream(args[2]);
			// bajt
			int size = in.readInt();
			System.out.println("predicted length: "+size);
			byte[] buffer = new byte[size];
			int read_already=0;
			while(read_already<size){
				int len = in.read(buffer,read_already,buffer.length-read_already);
				System.out.println("read: "+len);
				read_already+=len;
			}
			fos.write(buffer);
			fos.close();
			System.out.println("Finished!");
		} catch(FileNotFoundException e) {
			e.printStackTrace();
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
