package com.java.dataManager;

/**
 * Enumération représentant les types de générateurs d'OTP disponibles.
 * 
 * @author ADEGOLOYE Yves
 */
public enum OTPMethodType {
	HOTP("hotp"), TOTP("totp");
	
	private String type;
	
	private OTPMethodType(String type) {
		this.type = type;
	}
	
	public String toString() {
		return type;
	}
}
