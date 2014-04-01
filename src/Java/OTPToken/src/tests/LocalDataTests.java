package tests;

import org.junit.Test;

import com.java.androidtoken.SetPINActivity;
import com.java.dataManager.LocalData;
import com.java.dataManager.OTPMethodType;
import com.java.dataManager.Token;
import com.java.utils.HOTP;
import com.java.utils.ISecret;
import com.java.utils.Secret;
import com.java.utils.TOTP;

import android.test.InstrumentationTestCase;

public class LocalDataTests extends InstrumentationTestCase {
	
	public void testCipher() {
		String data = "ceci est un test";
		LocalData ld = LocalData.getInstance();
		if (ld == null) {
			System.out.println("Null localdata");
			return;
		}
		byte[] key = ld.createAesKeyWithPin(
				getInstrumentation().getContext(), "0000"
		);
		byte[] cypher = ld.encryptData(data, key);
		String decipher 
			= LocalData.getInstance().decryptData(cypher, key);
		assertEquals("Inconsitent cipher / decipher", data, decipher);
	}
	
	public void testCipherBad() {
		LocalData ld = LocalData.getInstance();
		if (ld == null) {
			System.out.println("Null localdata");
			return;
		}
		int count = 0;
		String data = "Ceci est un test";
		byte[] key = ld.createAesKeyWithPin(
				getInstrumentation().getContext(), "0000"
		);
		byte[] cipher;
		try {
			cipher = ld.encryptData(null, key);
		} catch (IllegalArgumentException e) {
			count++;
		}
		try {
			cipher = ld.encryptData(data, null);
		} catch (IllegalArgumentException e) {
			count++;
		}
		assertEquals("Bad casses not correctly handled.", 2, count);
	}
	
	public void testPin() {
		LocalData ld = LocalData.getInstance();
		ld.setPIN("123456");
		ld.savePin(getInstrumentation().getContext());
		byte[] savedPIN = ld.loadPin(getInstrumentation().getContext());
		assertEquals(
				"Wrong PIN registered", 
				new String(ld.hashPin("123456")), new String(savedPIN)
		);
		assertTrue(
				"Incoherent pin validation for right pin", 
				ld.validatePin(getInstrumentation().getContext(), "123456")
		);
		assertFalse(
				"Incoherent pin validation for wrong pin", 
				ld.validatePin(getInstrumentation().getContext(), "untruc")
		);
	}
	
	public void testSerialize() {
		LocalData ld = LocalData.getInstance();
		ISecret s = new Secret();
		s.setSecret("AABBCCDDEE");
		Token token1 = new Token(
				"token1", 
				OTPMethodType.HOTP, 
				new HOTP(2, s, 8)
		);
		Token token2 = new Token(
				"token2", 
				OTPMethodType.TOTP, 
				new TOTP(s, 6, TOTP.DEFAULT_QUANTUM)
		);
		ld.addToken(token1);
		ld.addToken(token2);
		ld.commit(getInstrumentation().getContext());
		ld.load(getInstrumentation().getContext());
		ld = LocalData.getInstance();
		Token newToken1 = ld.getToken("token1");
		assertFalse("HOTP token missing", newToken1 == null);
		assertEquals(
				"Incoherent name for hotp token", 
				token1.getNom(), 
				newToken1.getNom()
		);
		assertEquals(
				"Incoherent method for hotp token", 
				token1.getMethodType(), 
				newToken1.getMethodType()
		);
		assertEquals(
				"Incoherent key for hotp token",
				newToken1.getSecretKey(),
				token1.getSecretKey()
		);
		assertEquals(
				"Incoherent count for hotp token",
				newToken1.getCount(),
				token1.getCount()
		);
		assertEquals(
				"Incoherent length for hotp token",
				newToken1.getSize(),
				token1.getSize()
		);
		
		Token newToken2 = ld.getToken("token2");
		assertFalse("HOTP token missing", newToken2 == null);
		assertEquals(
				"Incoherent name for totp token", 
				token2.getNom(), 
				newToken2.getNom()
		);
		assertEquals(
				"Incoherent method for totp token", 
				token2.getMethodType(), 
				newToken2.getMethodType()
		);
		assertEquals(
				"Incoherent key for totp token",
				newToken2.getSecretKey(),
				((TOTP) token2.getIotpGen()).getKey().getHexRepresentation()
		);
		assertEquals(
				"Incoherent count for totp token",
				newToken2.getQuantum(),
				token2.getQuantum()
		);
		assertEquals(
				"Incoherent length for totp token",
				newToken2.getSize(),
				token2.getSize()
		);
		assertEquals(
				"Incoherent value for generated OTP", 
				token2.generate(), 
				newToken2.generate()
		);
	}
	
	public void testGetTokenBad() {
		LocalData ld = LocalData.getInstance();
		int count = 0;
		try {
			ld.getToken(null);
		} catch (IllegalArgumentException e) {
			count++;
		}
		assertEquals("Bad cases not correctly handled", 1, count);
	}
	
	public void testAddTokenBad() {
		LocalData ld = LocalData.getInstance();
		ISecret s = new Secret();
		s.setSecret("aabbccdd");
		Token t = new Token("test", OTPMethodType.HOTP, new HOTP(s));
		int count = 0;
		ld.addToken(t);
		try {
			ld.addToken(null);
		} catch (IllegalArgumentException e) {
			count++;
		}
		try {
			ld.addToken(t);
		} catch (IllegalArgumentException e) {
			count++;
		}
		assertEquals("Bad cases not correctly handled", 2, count);
	}
	
	public void testRemoveToken() {
		LocalData ld = LocalData.getInstance();
		int count = 0;
		try {
			ld.removeToken(null);
		} catch (IllegalArgumentException e) {
			count++;
		}
		try {
			ld.removeToken(-1);
		} catch (IllegalArgumentException e) {
			count++;
		}
		assertEquals("Bad cases not correctly handled.", 2, count);
		
		ld.removeToken("test");
		assertTrue("Token not correctly removed", ld.getToken("test") == null);
	}
	
	public void testTest() {
		LocalData ld = LocalData.getInstance();
		assertFalse("tokenList == null", ld.getListeToken() == null);
		for (Token token : ld.getListeToken()) {
			assertTrue(
					"Bad method type", 
					token.getMethodType() == OTPMethodType.HOTP
					|| token.getMethodType() == OTPMethodType.TOTP);
					
		}
	}
	
	public void testPinBad() {
		LocalData ld = LocalData.getInstance();
		int count = 0;
		try {
			ld.setPIN(null);
		} catch (IllegalArgumentException e) {
			count++;
		}
		assertEquals("Bad cases not correctly handled", 1, count);
	}
}
