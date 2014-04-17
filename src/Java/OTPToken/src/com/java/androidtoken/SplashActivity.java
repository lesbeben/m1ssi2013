package com.java.androidtoken;

import com.java.dataManager.IOFileUtils;
import com.java.dataManager.LocalData;

import android.os.AsyncTask;
import android.os.Bundle;
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
	private static final int WAIT_TIME = 1000;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_splash);

		new PrefetchData().execute();
	}

	/**
	 * Async Task to fetch local data.
	 */
	private class PrefetchData extends AsyncTask<Void, Void, Void> {
		private boolean isAreadyHavePIN = false;

		@Override
		protected void onPreExecute() {
			super.onPreExecute();
		}

		@Override
		protected Void doInBackground(Void... arg0) {			
            synchronized (this) {
               try {
            	   //On profite du splash pour tenter de synchroniser le temps
					Thread t = new Thread(new Runnable() {						
						@Override
						public void run() {
							LocalData.getInstance().synchronize();			
						}
					});
					long t0 = System.currentTimeMillis();
					t.start();
					// tester l'existence du fichier pin.xml, s'il n'existe pas,
					// le créer
					if (IOFileUtils.internalFileExists(getApplicationContext(),
							LocalData.LOCAL_PIN_FILE)) {
						isAreadyHavePIN = true;
					} else {
						IOFileUtils.createInternalFile(getApplicationContext(),
								LocalData.LOCAL_PIN_FILE);
					}
					
					
					// tester l'existence du fichier localData.xml, 
					// s'il n'existe pas, le créer
					if (!IOFileUtils.internalFileExists(getApplicationContext(),
							LocalData.LOCAL_DATA_FILE)) {
						IOFileUtils.createInternalFile(getApplicationContext(),
								LocalData.LOCAL_DATA_FILE);
					}
					// Si l'action est executée trop vite, on attend un peu pour
					// que le splash soit visible.
					t.join();
					long t1 = System.currentTimeMillis();
					if (t1 - t0 < WAIT_TIME) {
						wait(WAIT_TIME - (t1 - t0));
					}
               } catch (InterruptedException e) {
            	   //TODO
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
				i.putExtra("change_pin", false);
				startActivity(i);
			}

			// fermer le splash Activity
			finish();
		}

	}

}
