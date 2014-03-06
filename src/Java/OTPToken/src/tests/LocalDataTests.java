package tests;

import java.io.File;
import java.io.IOException;

import com.java.dataManager.IOFileUtils;

import android.content.Context;
import android.test.AndroidTestCase;
import android.test.IsolatedContext;

public class LocalDataTests extends AndroidTestCase {
	
	public void testTokenList() {
		int count = 0;
		Context context = new IsolatedContext(null, getContext());
		String fileName = "test";
        String fileName2 = "try";
        File file = new File(context.getFilesDir(), fileName);
        try {
			file.createNewFile();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        try {
            assertTrue(IOFileUtils.internalFileExists(context, null));
        } catch (IllegalArgumentException iae) {
            count++;
        }
        //On vérifie que les quatres cas se sont terminés avec des IAR
        assertEquals(
                "Bad cases not correctly handled" + (1 - count) + "out of 1",
                1, count
        );
        assertTrue("File exists.", IOFileUtils.internalFileExists(context, fileName));
        assertFalse("File does not exist.", IOFileUtils.internalFileExists(context, "other"));
	}

}
