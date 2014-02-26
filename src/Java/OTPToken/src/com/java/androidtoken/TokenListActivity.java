package com.java.androidtoken;

import android.os.Bundle;
import android.app.ListActivity;

/**Ecran d'accueil regroupant la liste des OTP créé
Sortie -si "Bouton menu Android AddToken" cliké, afficher AddTokenActivity
       -si "Bouton menu Android RemoveToken" clické , afficher RemoveTokenActivity
       -sinon si "menu cancel Bouton" clické, éteindre l'application
*/

public class TokenListActivity extends ListActivity {
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_token_list);
	}

}
