package utils;

public class Secret implements ISecret {

	private byte[] sec;	//secret
	private int length; //taille en octet


	@Override
	public void setSecret(byte[] secret) {
		this.sec = secret;
		this.length = secret.length;
	}

	@Override
	public void setSecret(int length) {
		if (length < 0) {
			throw new IllegalArgumentException();
		}
		//aléa
		this.length = length;

	}

	@Override
	public void setSecret(String hexRepresentation) {
		if (hexRepresentation == null) {
			throw new IllegalArgumentException();
		}
		
		this.length = hexRepresentation.length * 2;
		this.sec = hexToBinary(hexRepresentation);
	}

	@Override
	public int getLength() {
		//TODO test getSecret != null
		
		return length;
	}

	@Override
	public byte[] getSecret() {
		return sec;
	}

	@Override
	public String getHexRepresentation() {
		//TODO test getSecret != null
		return hexRepresentation;
	}

}
