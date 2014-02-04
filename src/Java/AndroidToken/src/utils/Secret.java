package utils;

public class Secret implements ISecret {

    byte[] secret;



	@Override
	public void setSecret(byte[] secret) {
        if (secret.length == 0) {
            throw new IllegalArgumentException("secret");
        }
        this.secret = secret;

	}

	@Override
	public void setSecret(int length) {
		//On prend une lib particulière?

	}

	@Override
	public void setSecret(String hexRepresentation) {
		// TODO Auto-generated method stub

	}

	@Override
	public int getLength() {
		return secret.length;
	}

	@Override
	public byte[] getSecret() {
		return secret;
	}

	@Override
	public String getHexRepresentation() {
        StringBuffer sb = new StringBuffer();
        for (byte b : secret) {
            sb.append(String.format("%02X", b));
        }
		return sb.toString();
	}

}
