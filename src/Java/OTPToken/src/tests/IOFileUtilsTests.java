package tests;

import java.io.File;

import android.content.Context;
import android.test.AndroidTestCase;
import android.test.InstrumentationTestCase;
import android.test.IsolatedContext;

import com.java.dataManager.IOFileUtils;

public class IOFileUtilsTests extends InstrumentationTestCase {
	
	//repris sur stack overflow pour gagner du temps (à mettre dans utils?)
	//à nettoyer (cas des nom multiples de 4)
	private byte[] hexStringToByteArray(String s) {
	    int len = s.length();
	    byte[] data = new byte[len / 2];
	    for (int i = 0; i < len; i += 2) {
	        data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
	                             + Character.digit(s.charAt(i+1), 16));
	    }
	    return data;
	}
	
	
	public void testInternalFileExists() {
		Context context = getInstrumentation().getContext();
		String fileName = "test";
        IOFileUtils.createInternalFile(context, fileName);
        assertTrue("File does not exist.", IOFileUtils.internalFileExists(context, fileName));
        assertFalse("File exists.", IOFileUtils.internalFileExists(context, "other"));
	}
	
	public void testCreateInternalFile() {
		Context context = getInstrumentation().getContext();
		String fileName = "test2";
		IOFileUtils.createInternalFile(context, fileName);
		File out = IOFileUtils.getInternalFile(context, fileName);
		assertTrue("File does not exist.", out.exists());
	}
	
	public void testDeleteFile() {
		Context context = getInstrumentation().getContext();
		String fileName = "test3";
		IOFileUtils.createInternalFile(context, fileName);
		IOFileUtils.deleteFile(context, fileName);
		File out = IOFileUtils.getInternalFile(context, fileName);
		assertFalse("File exists.", out.exists());
	}
	
	public void testSaveFile() {
		Context context = getInstrumentation().getContext();
		String fileName = "test4";
		IOFileUtils.createInternalFile(context, fileName);
		byte[] content = hexStringToByteArray("aefd2b5c");
		IOFileUtils.saveToInternalFile(context, fileName, content);
		byte[] out = IOFileUtils.readFromInternalFile(context, fileName);
		assertEquals("Different content.", content, out);
	}
	
	public void testClearFile() {
		Context context = getInstrumentation().getContext();
		String fileName = "test4";
		IOFileUtils.createInternalFile(context, fileName);
		byte[] content = hexStringToByteArray("aefd2b5c");
		IOFileUtils.saveToInternalFile(context, fileName, content);
		IOFileUtils.clearFile(context, fileName);
		byte[] out = IOFileUtils.readFromInternalFile(context, fileName);
		assertEquals("File not Empty.", out.length, 0);
	}
	
	public void testIArs() {
		int count = 0;
		Context context = getInstrumentation().getContext();
		String fileName = "testiar";
        try {
            assertTrue(IOFileUtils.internalFileExists(context, null));
        } catch (IllegalArgumentException iae) {
            count++;
        }
        try {
            assertTrue(IOFileUtils.internalFileExists(null, fileName));
        } catch (IllegalArgumentException iae) {
            count++;
        }
        try {
            IOFileUtils.createInternalFile(null, fileName);
        } catch (IllegalArgumentException iae) {
            count++;
        }
        try {
            IOFileUtils.createInternalFile(context, null);
        } catch (IllegalArgumentException iae) {
            count++;
        }
        try {
            IOFileUtils.getInternalFile(context, null);
        } catch (IllegalArgumentException iae) {
            count++;
        }
        try {
            IOFileUtils.getInternalFile(null, fileName);
        } catch (IllegalArgumentException iae) {
            count++;
        }
        //On vérifie que les quatres cas se sont terminés avec des IAr
        assertEquals(
                "Bad cases not correctly handled" + (6 - count) + "out of 6",
                6, count
        );
	}
	

}
