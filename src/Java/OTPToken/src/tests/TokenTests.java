package tests;

import org.junit.Test;

import com.java.dataManager.OTPMethodType;
import com.java.dataManager.Token;
import com.java.utils.HOTP;
import com.java.utils.IOTP;
import com.java.utils.ISecret;
import com.java.utils.Secret;

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
		assertEquals("Bad cases not correctly handled", 4, count);
	}
}
