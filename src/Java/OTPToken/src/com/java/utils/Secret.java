package com.java.utils;

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
		this.secret = hexStringToByteArray(hexRepresentation);
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
	
	//Outils
	private byte[] hexStringToByteArray(String s) {
	    int len = s.length();
	    byte[] data = new byte[len / 2];
	    for (int i = 0; i < len; i += 2) {
	        data[i / 2] = (byte) ((Character.digit(s.charAt(i), HEX_BASE) << 4)
	                             + Character.digit(s.charAt(i + 1), HEX_BASE));
	    }
	    return data;
	}
}
