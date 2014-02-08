package utils;

import java.math.BigInteger;
import java.security.SecureRandom;


public class Secret implements ISecret {

    private byte[] secret;
    
    private static final int HEX_BASE = 16;


	@Override
	public void setSecret(byte[] secret) {
        if (secret == null) {
            throw new IllegalArgumentException("secret");
        }
        this.secret = secret;
	}

	@Override
	public void setSecret(int length) {
		if (length < 0) {
			throw new IllegalArgumentException();
		}
		
		SecureRandom random = new SecureRandom();
		byte[] bytes = new byte[length];
		random.nextBytes(bytes);
		this.secret = bytes;
	}

	@Override
	public void setSecret(String hexRepresentation) {
        //TODO Ca marche ce truc?
		if (hexRepresentation == null) {
			throw new IllegalArgumentException();
		}
		//conversion en byte[] et stocke dans secret
		 this.secret = 
				 new BigInteger(hexRepresentation, HEX_BASE).toByteArray();
	}

	@Override
	public int getLength() {
        if (secret == null) {
            throw new IllegalStateException("secret");
        }
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
