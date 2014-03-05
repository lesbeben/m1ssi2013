package com.java.androidtoken;


import android.app.Activity;
import android.os.Bundle;


/**Ecran d'Ajout d'un Token
 * Ecran affiché lorsque l'utilisateur désire ajouter un Token depuis le menu 
 * Android.
 * Sortie: - si "Bouton enregistrement du Token" clické, afficher 
 * 			 TokenListActivity
 *         - sinon si "menu cancel Bouton" clické, afficher l'écran prècédent
 *@author ADEGOLOYE Yves
 */       


public class AddTokenActivity extends Activity {

	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_token_add);			
	}

}
