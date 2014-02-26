package com.java.androidtoken;


import android.app.Activity;
import android.os.Bundle;

public class PINLoginActivity extends Activity {

	
/**Ecran de connexion à l'application
Ecran affiché après l'écran splash (si ce n'est pas la première utilisation de l'application)
Sortie: -si le PIN est OK, afficher TokenListActivity
	      -sinon,  Dialog de PIN incorrect
*/	
	
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_login);			
	}

}
