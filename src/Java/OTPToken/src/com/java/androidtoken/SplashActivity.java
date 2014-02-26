package com.java.androidtoken;

import android.os.Bundle;
import android.app.Activity;

/**Ecran Splash, premier écran affiché lors du lancement de l'application
loading des données Utilisateur
sortie: -si première utilisation de l'application, afficher SetPINActivity
        -sinon afficher PINLoginActivity.
*/

public class SplashActivity extends Activity {
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_splash);
	}

}
