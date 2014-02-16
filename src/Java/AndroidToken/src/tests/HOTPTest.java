package tests;

import static org.junit.Assert.*;

import org.junit.Test;

import utils.HOTP;
import utils.IOTP;
import utils.ISecret;
import utils.Secret;

public class HOTPTest {

	@Test
	public void testHOTPLongISecretInt() {
		ISecret key = new Secret();
		key.setSecret("AABBCCDDEEFF");
		IOTP gene = new HOTP(10, key, 7);
		assertEquals("Wrong count for generator", 10, ((HOTP) gene).getCount());
		assertEquals(
				"Wrong key for generator", 
				"AABBCCDDEEFF", 
				((HOTP) gene).getKey().getHexRepresentation()
		);
	}

	@Test
	public void testHOTPISecret() {
		ISecret key = new Secret();
		key.setSecret("AABBCCDDEEFF");
		IOTP gene = new HOTP(key);
		assertEquals("Wrong count for generator", 0, ((HOTP) gene).getCount());
		assertEquals(
				"Wrong key for generator", 
				"AABBCCDDEEFF", 
				((HOTP) gene).getKey().getHexRepresentation()
		);
	}
	
	@Test (expected = IllegalArgumentException.class)
	public void testBadHOTP() {
		ISecret key = new Secret();
		key.setSecret("AABBCCDDEEFF");
		
		@SuppressWarnings("unused")
		IOTP otp = new HOTP(-1, key, 7);
		otp = new HOTP(1, null, 7);
		otp = new HOTP(1, key, 3);
		otp = new HOTP(null);
	}
	
	@Test
	public void testGenerer() {
		ISecret key = new Secret();
		key.setSecret("AABBCCDDEEFF");
		IOTP gene = new HOTP(0, key, 6);
		IOTP gene2 = new HOTP(0, key, 6);
		IOTP gene3 = new HOTP(1, key, 6);
		int otp1a = gene.generer();
		int otp1b = gene2.generer();
		int otp2 = gene.generer();
		int otp3 = gene3.generer();
		System.out.println(otp1a + " " + otp1b + " " + otp2 + " " + otp3);
		assertNotEquals(
				"Two successive values are equals" + otp1a + "vs" + otp2, 
				otp1a, otp2
		);
		assertEquals(
				"Same parameters give different values" 
			  + otp1a + " vs " + otp1b, 
			  otp1a, otp1b
		);
		assertTrue("Wrong length for OTP", otp2 < (Math.pow(10, 7)));		
	}
}
