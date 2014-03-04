package com.java.dataManager;

/**
 * Interface catégorisant les 2 types de génération d'OTP utilisées.
 * 
 * @author ADEGOLOYE Yves
 */
public interface OTPMethodType {
	
	/**
	 * OTP Method Type HOTP.
	 */
	int HOTP = 0;		
	
	/**
	 * OTP Method Type HOTP.
	 */
	int TOTP = 1;
}
