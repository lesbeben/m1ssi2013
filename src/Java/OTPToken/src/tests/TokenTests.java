package tests;

import org.junit.Test;

import com.java.dataManager.OTPMethodType;
import com.java.dataManager.Token;
import com.java.utils.HOTP;
import com.java.utils.IOTP;
import com.java.utils.ISecret;
import com.java.utils.Secret;
import com.java.utils.TOTP;

import junit.framework.TestCase;

public class TokenTests extends TestCase {
	
	/*
	 * Test du constructeur en condition normales.
	 */
	@Test
	public void testToken() {
		String nom = "test";
		OTPMethodType methodType = OTPMethodType.HOTP;
		ISecret secret = new Secret();
		secret.setSecret("AABBCCDDEEFF");
		IOTP otpGen = new HOTP(secret);
		Token token = new Token(nom, methodType, otpGen);
		assertEquals("Name is incorect.", token.getNom(), "test");
		assertEquals(
				"OTP method is incorect.", 
				token.getMethodType(), 
				OTPMethodType.HOTP
		);
		assertEquals("Generator is incorect.", token.getIotpGen(), otpGen);
	}
	
	/*
	 * Test du constructeur dans des cas dégénérés.
	 */
	@SuppressWarnings("unused")
	@Test
	public void testTokenBad() {
		int count = 0;
		OTPMethodType methodType = OTPMethodType.HOTP;
		ISecret secret = new Secret();
		secret.setSecret("AABBCCDDEEFF");
		IOTP otpGen = new HOTP(secret);
		Token token;
		//Nom à null
		try {
			token = new Token(null, methodType, otpGen);
		} catch (IllegalArgumentException e) {
			count++;
		}
		//methodType à null
		try {
			token = new Token("test", null, otpGen);
		} catch (IllegalArgumentException e) {
			count++;
		}
		//otpGen à null
		try {
			token = new Token("test", methodType, null);
		} catch (IllegalArgumentException e) {
			count++;
		}
		//incoherent method type
		try {
			token = new Token("test", OTPMethodType.TOTP, otpGen);
		} catch (IllegalArgumentException e) {
			count++;
		}
		assertEquals(
				"Bad cases not correctly handled" + (4 - count) + "out of 4", 
				4, count
		);
	}
	
	@Test
	public void testGenerateHOTP() {
		int count = 4;
		OTPMethodType methodType = OTPMethodType.HOTP;
		ISecret secret = new Secret();
		secret.setSecret("AABBCCDDEEFF");
		IOTP otpGen = new HOTP(count, secret, 6);
		IOTP otpGen2 = new HOTP(count, secret, 6);
		Token token = new Token("test", OTPMethodType.HOTP, otpGen);
		for (int i = 0; i < 100; i++) {	
			assertEquals(
					"Wrong value for" + i + "th OTP.", 
					otpGen2.generer(), 
					token.generate()
			);
		}
	}
	
	@Test
	public void testGenerateTOTP() {
		int count = 4;
		OTPMethodType methodType = OTPMethodType.TOTP;
		ISecret secret = new Secret();
		secret.setSecret("AABBCCDDEEFF");
		IOTP otpGen = new TOTP(secret, 6, 1);
		IOTP otpGen2 = new TOTP(secret, 6, 1);
		Token token = new Token("test", OTPMethodType.TOTP, otpGen);
		assertEquals(
			"Wrong value for OTP.", 
			otpGen2.generer(), 
			token.generate()
		);
		try {
			Thread.sleep(1000);
		} catch (InterruptedException e) {
			System.out.println("Fatal ; interruption while in sleep.");
			return;
		}
		assertEquals(
			"Wrong value for OTP.", 
			otpGen2.generer(), 
			token.generate()
		);
	}
}
