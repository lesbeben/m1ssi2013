package tests;

import static org.junit.Assert.*;

import java.util.Date;

import org.junit.Test;

import utils.IOTP;
import utils.ISecret;
import utils.Secret;
import utils.TOTP;

public class TOTPTest {
	
	private static final int S_TO_MS = 1000;
	
	@Test
	public void testGetCount() {
		ISecret key = new Secret();
		key.setSecret("aabbccddeeffgghh");
		IOTP otp = new TOTP(key, 6, 1);
		long t0 = (new Date()).getTime();
		long t = ((TOTP) otp).getCount();
		long t1 = (new Date()).getTime();
		t0 = t0 / S_TO_MS;
		t1 = t1 / S_TO_MS;
		assertTrue(
				"Bad calculation for counter. Expected " 
				+ t0 + " or " + t1 + " got " + t, 
				(t == t0) || (t == t1)
		);
	}

	@Test
	public void testTOTPISecretIntInt() {
		ISecret key = new Secret();
		key.setSecret("aabbccdd");
		IOTP otp = new TOTP(key, 7, 10);
		assertEquals(
				"Wrong quantum.", "aabbccdd", 
				((TOTP) otp).getKey().getHexRepresentation()
		);
	}

	@Test
	public void testTOTPISecret() {
		ISecret key = new Secret();
		key.setSecret("aabbccdd");
		IOTP otp = new TOTP(key);
		assertEquals(
				"Wrong quantum.", "aabbccdd", 
				((TOTP) otp).getKey().getHexRepresentation()
		);
	}

	@Test
	public void testGenerer() {
		ISecret key = new Secret();
		key.setSecret("aabbccddeeff");
		IOTP gene = new TOTP(key, 7, 1);
		int otp1a = gene.generer();
		int otp1b = gene.generer();
		try {
			Thread.sleep(1300);
		} catch (InterruptedException e) {
			// RIEN
		}
		int otp2 = gene.generer();
		
		assertEquals("Two successive values are equals", otp1a, otp2);
		assertNotEquals("Same parameters give different values", otp1a, otp1b);
		assertTrue("Wrong length for OTP", otp2 < (10 ^ 7));
	}

}
