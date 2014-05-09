package com.java.dataManager;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;

import android.content.Context;

/**
 * Cette classe permet de créer, modifier, supprimer , enregistrer dans un 
 * fichier interne du device Android.
 * @author ADEGOLOYE Yves
 */
public final class IOFileUtils {
	
	private IOFileUtils() {
		
	}
	
	/**
	 * Cette fonction permet de savoir si le fichier passé en paramètre
	 * existe dans la mémoire interne du device.
	 * 
	 * @param context : Le contexte de l'application
	 * @param fileName : Le nom du fichier à chercher
	 * 
	 * @pre context != null
	 * 		filename != null
	 * 
	 * @return true si le fichier existe, false sinon
	 */
	public static boolean internalFileExists(Context context, String fileName) {
		if ((fileName == null) || (context == null)) {
			throw new IllegalArgumentException("Empty filename");
		}
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
	 * @param context : Le context de l'application.
	 * @param fileName : Le nom du fichier à créer.
	 * 
	 * @pre context != null
	 * 		filename != null
	 */

	public static void createInternalFile(Context context, String fileName) {
		if ((fileName == null) || (context == null)) {
			throw new IllegalArgumentException("Empty filename");
		}
		File file = new File(context.getFilesDir(), fileName);
		try {
			file.createNewFile();
		} catch (IOException e) {
			//TODO
		}
	}
	

	
	/**
	 * Cette fonction renvoie le fichier (en mémoire interne du device) ayant 
	 * pour nom celui passé en paramètres.
	 * 
	 * @param context Le contexte de l'application
	 * @param fileName Le nom du fichier à récupérer
	 * 
	 * @pre context != null
	 * 		filename != null
	 * 
	 * @return File : Le flux de fichier correspondant au nom passé en paramètre
	 */
	public static File getInternalFile(Context context, String fileName) {
		if ((fileName == null) || (context == null)) {
			throw new IllegalArgumentException("Empty filename");
		}
		File file = new File(context.getFilesDir() + "/" + fileName);
		return file;
	}
	
	
	/**
	 * Cette fonction supprime de la mémoire interne le fichier ayant pour nom
	 * celui passé en paramètres.
	 * 
	 * @param context : Le contexte de l'application.
	 * @param fileName : Le nom du fichier à supprimer
	 * 
	 * @pre : context != null
	 * 		  fileName != null
	 * 
	 * @return true si le fichier à été supprimé, false sinon
	 */

	public static boolean deleteFile(Context context, String fileName) {
		if ((fileName == null) || (context == null)) {
			throw new IllegalArgumentException("Empty filename");
		}
		File dir = context.getFilesDir();
		File file = new File(dir, fileName);
		boolean deleted = file.delete();
		return deleted;
	}
	
	
	/**
	 * Cette fonction enregistre le contenu passé en paramètre dans le fichier.
	 * 
	 * @param context : Le contexte de l'application
	 * @param fileName : Le nom du fichier à écrire
	 * @param content en byte[] : Les données à écrire dans le fichier.
	 * 
	 * @pre context != null
	 * 		filename != null
	 * 		contenu != null
	 * 		internalFileExists(filename)
	 */

	public static void saveToInternalFile(
			Context context, String fileName, byte[] content) {
		if ((fileName == null) || (context == null) || (content == null)) {
			throw new IllegalArgumentException("Empty filename");
		}
		FileOutputStream outputStream;
		try {
			outputStream = 
					context.openFileOutput(fileName, Context.MODE_PRIVATE);
			outputStream.write(content);
			outputStream.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	/**
	 * Cette fonction lit le fichier passé en paramétre.
	 * 
	 * @param context : Le contexte de l'application 
	 * @param fileName : Le nom du fichier à lire
	 * 
	 * @pre context != null
	 * 		filename != null
	 * 		internalFileExists(filename)
	 * 
	 * @return Le contenu du fichier sous la forme d'un tableau d'octet
	 */

	public static byte[] readFromInternalFile(
			Context context, String fileName) {
		if ((fileName == null) || (context == null)) {
			throw new IllegalArgumentException("Empty filename");
		}
		FileInputStream inputStream;
		byte[] content = null;
		try {
			inputStream = context.openFileInput(fileName);
			content = new byte[inputStream.available()];
            inputStream.read(content);
            inputStream.close();
		} catch (Exception e) {
			//TODO
		}
		return content;
	}
	
	
	/**
	 * Cette fonction supprime le contenu du fichier ayant pour nom celui passé 
	 * en paramètres.
	 * 
	 * @param context : Le contexte de l'application.
	 * @param fileName : Le nom du fichier à effacer.
	 * 
	 * @pre context != null
	 * 		filename != null
	 * 		internalFileExists(filename)
	 * 
	 * @return boolean
	 */

	public static void clearFile(Context context, String fileName) {
		if ((fileName == null) || (context == null)) {
			throw new IllegalArgumentException("Empty filename");
		}
		File dir = context.getFilesDir();
		File file = new File(dir, fileName);
		PrintWriter writer;
		try {
			writer = new PrintWriter(file);
			writer.print("");
			writer.close();
		} catch (FileNotFoundException e) {
			//TODO
		}
	}

}
