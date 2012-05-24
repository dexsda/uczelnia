import java.security.*;
import java.io.*;
import java.util.Date;

public class Client {
	private TSA tsa;
	private Client() {
		byte[] hash = computeFileDigest("Client.java");
		System.out.println("hash: " + byteArrayToHexString(hash));
		System.out.println("-> creating test tsa");
		tsa = new TSA();
		System.out.println("-> sending hash");
		Timestamp stamp = tsa.getTimestamp(new Hash(Type.SHA1,hash));
		System.out.println("timestamp: " + stamp.toString());
		System.out.println("-> verifying with wrong file");
		byte[] hashWrong = computeFileDigest("TSA.java");
		System.out.println("hash: " + byteArrayToHexString(hashWrong));
		System.out.println("result: " + verifyTimestamp(new Hash(Type.SHA1,hashWrong),stamp));
		System.out.println("-> verifying with wrong time");
		Timestamp stampWrong = new Timestamp(stamp.hash, new Date(543543));
		System.out.println("hash: " + byteArrayToHexString(hash));
		System.out.println("result: " + verifyTimestamp(new Hash(Type.SHA1,hash),stampWrong));
		System.out.println("-> verifying properly");
		System.out.println("hash: " + byteArrayToHexString(hash));
		System.out.println("result: " + verifyTimestamp(new Hash(Type.SHA1,hash),stamp));
	}

	private boolean verifyTimestamp(Hash hash, Timestamp stamp){
		try {
			Signature rsa = Signature.getInstance("SHA1withRSA");
			PublicKey pub = tsa.getPubKey();
			byte[] timeBytes = stamp.time.toString().getBytes();
			byte[] hashTwo = new byte[hash.hashBytes.length + timeBytes.length];
			int i=0;
			for(i=0; i<hash.hashBytes.length; i++){
				hashTwo[i] = hash.hashBytes[i];
			}
			int j=0;
			for(;i<hashTwo.length; i++){
				hashTwo[i] = timeBytes[j];
			}
			MessageDigest sha = MessageDigest.getInstance("SHA-1");
			sha.update(hashTwo);
			byte[] hashed = sha.digest();
			rsa.initVerify(pub);
			rsa.update(hashed);
			return rsa.verify(stamp.hash.hashBytes);
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}

	public static String byteArrayToHexString(byte[] b) {
		StringBuffer sb = new StringBuffer(b.length * 2);
		for (int i = 0; i < b.length; i++) {
			int v = b[i] & 0xff;
			if (v < 16) {
				sb.append('0');
			}
			sb.append(Integer.toHexString(v));
		}
		return sb.toString().toLowerCase();
	}

	private byte[] computeFileDigest(String filename) {
		try {
			File file = new File(filename);
			InputStream is = new FileInputStream(file);
			long length = file.length();
			byte[] bytes = new byte[(int)length];
			int offset = 0;
			int numRead = 0;
			while (offset < bytes.length && (numRead=is.read(bytes, offset, bytes.length-offset)) >= 0) {
				offset += numRead;
			}
			if (offset < bytes.length) {
				throw new IOException("Could not completely read file "+filename);
			}
			is.close();

			MessageDigest sha = MessageDigest.getInstance("SHA-1");
			sha.update(bytes);
			byte[] hash = sha.digest();
			return hash;
		} catch (Exception e){
			e.printStackTrace();
			return null;
		}
	}

	public static void main(String[] args) {
		new Client();
	}
}
