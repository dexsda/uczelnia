import java.security.*;
import java.util.Date;

public class TSA {

	public PublicKey pubKey;
	private PrivateKey prvKey;

	public TSA() {
		try {
			KeyPairGenerator keyGen = KeyPairGenerator.getInstance("RSA");
			keyGen.initialize(1024);
			KeyPair pair = keyGen.generateKeyPair();
			this.pubKey = pair.getPublic();
			this.prvKey = pair.getPrivate();
			//System.out.println(this.pubKey);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private Date getTime() {
		return new Date();
	}

	public PublicKey getPubKey() {
		return pubKey;
	}

	public Timestamp getTimestamp(Hash hash){
		try {
			Signature rsa = Signature.getInstance("SHA1withRSA");
			Date time = getTime();
			byte[] timeBytes = time.toString().getBytes();
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
			rsa.initSign(prvKey);
			rsa.update(hashed);
			byte[] signed = rsa.sign();
			return new Timestamp(new Hash(Type.SHA1,signed),time);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}
}
