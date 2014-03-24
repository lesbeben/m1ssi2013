package tests;

import com.java.dataManager.LocalData;
import com.java.dataManager.OTPMethodType;
import com.java.dataManager.Token;

import android.test.AndroidTestCase;

public class LocalDataTests extends AndroidTestCase {
	
	public void testCipher() {
		String data = "ceci est un test";
		LocalData ld = LocalData.getInstance();
		if (ld == null) {
			System.out.println("Null localdata");
			return;
		}
		byte[] key = ld.createAesKeyWithPin(this.getContext(), "0000");
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
		byte[] key = ld.createAesKeyWithPin(this.getContext(), "0000");
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
	
	public void testTest() {
		LocalData ld = LocalData.getInstance();
		assertFalse("tokenList == null", ld.getListeToken() == null);
		for (Token token : ld.getListeToken()) {
			assertEquals("Bad name for token", token.getNom(), "");
			assertTrue(
					"Bad method type", 
					token.getMethodType() == OTPMethodType.HOTP
					|| token.getMethodType() == OTPMethodType.TOTP);
					
		}
	}
}
