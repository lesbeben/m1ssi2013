package tests;

import static org.junit.Assert.*;

import org.junit.Test;

import utils.ISecret;
import utils.Secret;
import utils.Utils;

public class UtilsTest {
	@Test
	public void testTruncate() {
		byte[] test = new byte[] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
								  10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
		byte[] res = Utils.truncate(test);
		assertTrue("Mauvaise taille de troncature.", res.length == 4);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void testTruncateBad() {
		Utils.truncate(null);
	}

	@Test
	public void testConvert() {
		byte[] test = new byte[] {0, 0, 0, 10};
		int res = Utils.convert(test);
		assertEquals("Bas conversion.", res, 10);
	}

	@Test(expected = IllegalArgumentException.class)
	public void testConvertBad() {
		Utils.convert(null);
	}
	
	@Test
	public void testHmacSha1() {
		ISecret key = new Secret();
		key.setSecret("ba22ef522cb9a0");
		byte[] res1 = Utils.hmacSha1(key, 0);
		byte[] res2 = Utils.hmacSha1(key, 0);
		byte[] res3 = Utils.hmacSha1(key, 1);
		assertArrayEquals("Different values for same counter", res1, res2);
		assertFalse("Successive values are equals", res1.equals(res3));
	}

}
