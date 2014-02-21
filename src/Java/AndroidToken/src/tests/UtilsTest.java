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
								  10, 9, 8, 7, 6, 5, 4, 3, 2, 4};
		byte[] res = Utils.truncate(test);
		assertTrue(
				"Mauvaise taille de troncature. Expected 32 got " 
			  +	(res.length * 8), res.length == 4
		);
		assertArrayEquals(
				"Wrong output for truncation", 
				new byte[] {5, 6, 7, 8}, res
		);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void testTruncateBad() {
		Utils.truncate(null);
	}
	
	@Test(expected = IllegalArgumentException.class)
	public void testTruncateBad2() {
		Utils.truncate(new byte[] {});
	}
	
	@Test
	public void testConvert() {
		//6253 : 00000001 00001000 00000110 00001101
		byte[] test = new byte[] {0, 0, 24, 109};
		//assertEquals("Bad conversion.", 6253, Utils.convert(test));
		test = new byte[] {0, 0, 0, 0};
		assertEquals("Bad conversion.", 0, Utils.convert(test));
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
	
	@Test
	public void testHmacSha1Bad() {
		int count = 0;
		ISecret key = new Secret();
		try {
			Utils.hmacSha1(key, 0);
		} catch (IllegalArgumentException e) {
			count++; 
		}
		key.setSecret("ba22ef522cb9a0");
		try {
			Utils.hmacSha1(null, 0);
		} catch (IllegalArgumentException e) {
			count++; 
		}
		try {
			Utils.hmacSha1(key, -2);
		} catch (IllegalArgumentException e) {
			count++; 
		}
		
		assertEquals("Bad param not correctly handled", 3, count);		
	}
	
	@Test
	public void coherenceTest() {
		byte[] buff = new byte[] {
			23, 51, 2, 43, 114, 57, 63, 71, 48, 9,
	        19, 108, 27, 56, 105, 74, 33, 22, 51,10
		};
		byte[] res = Utils.truncate(buff);
		int num = Utils.convert(res);
		assertEquals(
				"incoherence with C version. Expected 721564439 received " + num
				, 721564439, num
		);
	}

}
