import java.util.Date;
public class Timestamp {
	public Hash hash;
	public Date time;
	public Timestamp(Hash hash, Date time) {
		this.hash = hash;
		this.time = time;
	}
	@Override
	public String toString() {
		return hash.toString() + "@" + time.toString();
	}
}
