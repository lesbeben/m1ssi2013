package com.java.androidtoken;

import com.java.dataManager.IOFileUtils;
import com.java.dataManager.LocalData;

import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.app.Activity;
import android.content.Intent;

/**
 * Ecran Splash, premier écran affiché lors du lancement de l'application
 * loading des données Utilisateur sortie: -si première utilisation de
 * l'application, afficher SetPINActivity -sinon afficher PINLoginActivity.
 * 
 * @author ADEGOLOYE Yves
 */

public class SplashActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_splash);

		new PrefetchData().execute();
	}

	/**
	 * Async Task to make http call
	 */
	private class PrefetchData extends AsyncTask<Void, Void, Void> {
		private boolean isAreadyHavePIN = false;

		@Override
		protected void onPreExecute() {
			super.onPreExecute();
		}

		@Override
		protected Void doInBackground(Void... arg0) {
			
            synchronized(this)
            {
               try {
				wait(3000);
				
				
				// tester l'existence du fichier pin.xml, s'il n'existe pas, le créer
				if (IOFileUtils.internalFileExists(getApplicationContext(),
						LocalData.LOCAL_PIN_FILE)) {
					isAreadyHavePIN = true;
				} else {
					IOFileUtils.createInternalFile(getApplicationContext(),
							LocalData.LOCAL_PIN_FILE);
				}
				
				
				// tester l'existence du fichier localData.xml, s'il n'existe pas, le créer
				if (!IOFileUtils.internalFileExists(getApplicationContext(),
						LocalData.LOCAL_DATA_FILE)) {
					IOFileUtils.createInternalFile(getApplicationContext(),
							LocalData.LOCAL_PIN_FILE);
				}
			
				
				
				
			} catch (InterruptedException e) {
			}
            }			
			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			super.onPostExecute(result);

			Intent i = null;

			// afficher la prochaine Activity
			if (isAreadyHavePIN) {
				i = new Intent(SplashActivity.this, PINLoginActivity.class);
				startActivity(i);
			} else {
				i = new Intent(SplashActivity.this, SetPINActivity.class);
				startActivity(i);
			}

			// fermer le splash Activity
			finish();
		}

	}

}
