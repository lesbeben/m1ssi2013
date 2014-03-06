package tests;

/**
 * Created by beben on 3/5/14.
 */
import android.app.Activity;
import android.app.Application;
import android.content.Context;
import android.test.AndroidTestCase;
import com.java.dataManager.IOFileUtils;
import junit.framework.Assert;
import junit.framework.TestCase;
import org.junit.Test;

import java.io.File;
import java.io.IOException;

public class IOFileUtilsTests extends AndroidTestCase {

    @Test
    public void testInternalFileExists() throws IOException {
        int count = 0;
        Context context = new Application();
        String fileName = "test";
        String fileName2 = "try";
        File file = new File(context.getFilesDir(), fileName);
        file.createNewFile();
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
