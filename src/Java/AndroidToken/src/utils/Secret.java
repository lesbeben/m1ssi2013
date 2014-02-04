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
		// TODO aléatoire
		this.length = length;

	}

	@Override
	public void setSecret(String hexRepresentation) {
		if (hexRepresentation == null) {
			throw new IllegalArgumentException();
		}
		
		//modifie la longueur en fonction du parametre
		this.length = hexRepresentation.length() * 2;
		
		//modifie le secret
		try {
			Integer val = Integer.valueOf(hexRepresentation, 16); //met en decimal
			String chiffreBin = Integer.toBinaryString(val.intValue());	//met en binaire
			
			this.sec = chiffreBin.getBytes(); //stocke dans sec (secret)
		} catch(NumberFormatException ex) {
			ex.printStackTrace();
		}
	}

	@Override
	public int getLength() {
		if(sec == null) {
			throw new IllegalAccessError();
		}
		
		return length;
	}

	@Override
	public byte[] getSecret() {
		return sec;
	}

	@Override
	public String getHexRepresentation() {
		if(sec == null) {
			throw new IllegalAccessError();
		}
		
		String str = new String(sec);
		return Integer.toString(Integer.parseInt(str, 16));
	}

}
