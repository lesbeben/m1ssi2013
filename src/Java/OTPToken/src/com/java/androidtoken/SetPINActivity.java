package com.java.androidtoken;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

/**
 * Ecran d'initialisation ou du mise à jour du PIN Ecran affiché soit à la
 * première utilisation de l'application soit lorsque l'utilisateur désire
 * modifier son PIN depuis le menu Android. Sortie: afficher l'écran précédent.
 * 
 * @author ADEGOLOYE Yves
 */

public class SetPINActivity extends Activity {
	private boolean isChange;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_set_pin);

		Intent i = getIntent();
		isChange = i.getBooleanExtra("change_pin", false);

		TextView existingPIN = (TextView) findViewById(R.id.pinChangeExistingPin);
		EditText existingPinEdit = (EditText) findViewById(R.id.pinChangeExistingPinEdit);
		if (isChange) {
			existingPIN.setVisibility(View.VISIBLE);
			existingPinEdit.setVisibility(View.VISIBLE);
		} else {
			existingPIN.setVisibility(View.GONE);
			existingPinEdit.setVisibility(View.GONE);
		}

	}

}
