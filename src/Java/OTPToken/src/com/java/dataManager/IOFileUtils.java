package com.java.dataManager;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;

import android.content.Context;

/**
 * Classe permet de créer, modifier, supprimer , enregistrer dans un fichier 
 * interne du device Android.
 * @author ADEGOLOYE Yves
 */
public final class IOFileUtils {
	
	private IOFileUtils() {
		
	}
	
	/**
	 * Cette fonction permet de savoir si le fichier passé en paramètre
	 * existe dans la mémoire interne du device.
	 * 
	 * @param context
	 * @param fileName
	 * @return boolean
	 */
	public static boolean internalFileExists(Context context, String fileName) {
		File file = new File(context.getFilesDir(), fileName);
		if (!file.exists()) {
			return false;
		}
		return true;
	}	
	
	/**
	 * Cette fonction crée un nouveau fichier avec comme nom , celui passé en 
	 * paramètre.
	 * 
	 * @param context
	 * @param fileName
	 */

	public static void createInternalFile(Context context, String fileName) {
		File file = new File(context.getFilesDir(), fileName);
		try {
			file.createNewFile();
		} catch (IOException e) {
			//RIEN
		}
	}
	

	
	/**
	 * Cette fonction renvoie le fichier (en mémoire interne du device) ayant 
	 * pour nom celui passé en paramètres.
	 * 
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
	 * celui passé en paramètres.
	 * 
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
	
	
	/**
	 * Cette fonction enregistre le contenu passé en paramètre dans le fichier.
	 * 
	 * @param context
	 * @param fileName
	 * @param contenu en byte[]
	 */

	public static void saveToInternalFile(
			Context context, String fileName, byte[] contenu) {
		FileOutputStream outputStream;
		try {
			outputStream = 
					context.openFileOutput(fileName, Context.MODE_PRIVATE);
			outputStream.write(contenu);
			outputStream.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	/**
	 * Cette fonction lit le fichier passé en paramétre.
	 * @param context
	 * @param fileName
	 * @param contenu en byte[]
	 */

	public static byte[] readFromInternalFile(
			Context context, String fileName) {
		FileInputStream inputStream;
		byte[] content = null;
		try {
			inputStream = context.openFileInput(fileName);
			content = new byte[inputStream.available()];
            inputStream.read(content);
            inputStream.close();
		} catch (Exception e) {
			//RIEN
		}
		return content;
	}
	
	
	/**
	 * Cette fonction supprime le contenu le fichier ayant pour nom celui passé 
	 * en paramètres.
	 * 
	 * @param context
	 * @param fileName
	 * @return boolean
	 */

	public static void clearFile(Context context, String fileName) {
		File dir = context.getFilesDir();
		File file = new File(dir, fileName);
		PrintWriter writer;
		try {
			writer = new PrintWriter(file);
			writer.print("");
			writer.close();
		} catch (FileNotFoundException e) {
			//RIEN
		}
	}

}
