package com.java.androidtoken;

import android.app.Activity;
import android.os.Bundle;

/**Ecran d'initialisation ou du mis à jour du PIN
*Ecran affiché soit à la première utilisation de l'application
*soit lorsque l'utilisateur désire modifier son PIN depuis le menu Android.
*Sortie: afficher l'écran précédent
*@author ADEGOLOYE Yves
*/

public class SetPINActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_set_pin);
	}

}
