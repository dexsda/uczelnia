public class Hash {
	public Type type;
	public byte[] hashBytes;
	public Hash(Type type, byte[] hash){
		this.type = type;
		this.hashBytes = hash;
	}
	@Override
	public String toString() {
		StringBuffer sb = new StringBuffer(hashBytes.length * 2);
		for (int i = 0; i < hashBytes.length; i++) {
			int v = hashBytes[i] & 0xff;
			if (v < 16) {
				sb.append('0');
			}
			sb.append(Integer.toHexString(v));
		}
		return sb.toString().toLowerCase();
	}
}
