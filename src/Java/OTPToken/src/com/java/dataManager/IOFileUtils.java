package com.java.dataManager;

import java.io.File;
import java.io.FileOutputStream;

import android.content.Context;

public class IOFileUtils {

	/**
	 * Cette fonction permet de savoir si le fichier passé en paramètre
	 * existe dans la mémoire interne du device
	 * @param context
	 * @param fileName
	 * @return boolean
	 */
	public static boolean isExistInternalFile(Context context, String fileName) {
		File file = new File(context.getFilesDir(), fileName);
		if (!file.exists()) {
			return false;
		}
		return true;
	}
	
	
	
	/**
	 * Cette fonction crée un nouveau fichier avec comme nom , celui passé en paramètre
	 * @param context
	 * @param fileName
	 */

	public static void createInternalFile(Context context, String fileName) {
		FileOutputStream outputStream;
		try {
			outputStream = context.openFileOutput(fileName,
					Context.MODE_PRIVATE);
			outputStream.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	

	
	/**
	 * Cette fonction renvoie le fichier (en mémoire interne du device)
	 * ayant pour nom celui passé en paramètres.
	 * @param context
	 * @param fileName
	 * @return File
	 */
	public static File getInternalFile(Context context, String fileName) {
		File file = new File(context.getFilesDir() + "/" + fileName);
		return file;
	}
	
	
	/**
	 * Cette fonction supprime de la mémoire interne le fichier ayant pour nom
	 * celui passé en paramètres
	 * @param context
	 * @param fileName
	 * @return boolean
	 */

	public static boolean deleteFile(Context context, String fileName) {
		File dir = context.getFilesDir();
		File file = new File(dir, fileName);
		boolean deleted = file.delete();
		return deleted;
	}

}
