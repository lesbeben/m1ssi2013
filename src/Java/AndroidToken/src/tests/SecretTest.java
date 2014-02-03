package tests;

import static org.junit.Assert.*;

import org.junit.Test;

import utils.ISecret;
import utils.Secret;

public class SecretTest {

	@Test
	public void testSetSecretByteArray() {
		byte[] bytes = new byte[] {0, 1, 2, 3, 4};
		ISecret key = new Secret();
		key.setSecret(bytes);
		assertArrayEquals(
				"Wrong set secret or get secret", 
				bytes, 
				key.getSecret()
		);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void badTestSetSecretByteArray() {
		ISecret key = new Secret();
		key.setSecret((byte[]) null); 
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void badTestSetSecretString() {
		ISecret key = new Secret();
		key.setSecret((String) null); 
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void badTestSetSecretInt() {
		ISecret key = new Secret();
		key.setSecret(-1); 
	}
	
	@Test
	public void testSetSecretInt() {
		int length = 10;
		ISecret key = new Secret();
		key.setSecret(length);
		assertEquals(
				"Bad setSecret(int) or getLength", 
				length, 
				key.getLength()
		);
	}

	@Test
	public void testSetSecretString() {
		String hex = "03";
		ISecret secret = new Secret();
		secret.setSecret(hex);
		byte[] expected = new byte[] {3};
		assertArrayEquals(
				"Incoherent value for secret", 
				expected, 
				secret.getSecret()
		);
	}

	@Test
	public void testGetHexRepresentation() {
		ISecret key = new Secret();
		String hex = "2AEE217CD8";
		key.setSecret(hex);
		assertTrue(
				"Wrong setSecret(String) or getHexRepresentation."
				+ "\nIncoherent values", 
				hex.equals(key.getHexRepresentation())
		);
	}

}
