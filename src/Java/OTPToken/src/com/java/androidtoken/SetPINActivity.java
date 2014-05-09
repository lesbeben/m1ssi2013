package com.java.androidtoken;

import com.java.dataManager.LocalData;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

/**
 * Ecran d'initialisation ou du mise à jour du PIN Ecran affiché soit à la
 * première utilisation de l'application soit lorsque l'utilisateur désire
 * modifier son PIN depuis le menu Android. 
 * Sortie: -afficher l'écran précédent si change PIN
 *         -afficher tokenlistActivity si liste des tokens non vide
 *         -afficher AddTokenActivity sinon
 * @author ADEGOLOYE Yves
 */

public class SetPINActivity extends Activity {
	private boolean isChange;

	private static final int DIALOG_INVALID_EXISTING_PIN = 0;
	private static final int DIALOG_DIFF_NEW_PIN = 1;
	private static final int DIALOG_NO_NEW_PIN = 2;

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

		Button setPinBtn = (Button) findViewById(R.id.pinChangeSubmit);
		setPinBtn.setOnClickListener(setPinListener);

	}

	private OnClickListener setPinListener = new OnClickListener() {

		public void onClick(View v) {

			if (isChange) {
				String existingPin = ((EditText) findViewById(R.id.pinChangeExistingPinEdit))
						.getText().toString();

				boolean validate = LocalData.getInstance().validatePin(
						getApplicationContext(), existingPin);

				if (!validate) {
					showDialog(DIALOG_INVALID_EXISTING_PIN);
					return;
				}
			}

			String newPin1 = ((EditText) findViewById(R.id.pinChangeNew1Edit))
					.getText().toString();
			String newPin2 = ((EditText) findViewById(R.id.pinChangeNew2Edit))
					.getText().toString();

			if (newPin1.length() == 0) {
				showDialog(DIALOG_NO_NEW_PIN);
				return;
			}

			if (!newPin1.contentEquals(newPin2)) {
				showDialog(DIALOG_DIFF_NEW_PIN);
				return;
			}

			LocalData.getInstance().setPIN(newPin1);
			LocalData.getInstance().savePin(getApplicationContext());

			if (isChange) {
				finish();
			} else {

				Intent i = new Intent(SetPINActivity.this,
						AddTokenActivity.class);
				i.putExtra("from-login", true);
				startActivity(i);
				finish();
			}
		}
	};

	@Override
	protected Dialog onCreateDialog(int id) {
		Dialog d;

		switch (id) {
		case DIALOG_DIFF_NEW_PIN:
			d = createAlertDialog(R.string.pinAlertNewPinsDifferent);
			break;

		case DIALOG_INVALID_EXISTING_PIN:
			d = createAlertDialog(R.string.pinAlertInvalidPin);
			break;

		case DIALOG_NO_NEW_PIN:
			d = createAlertDialog(R.string.pinAlertNewPinBlank);
			break;

		default:
			d = null;
		}

		return d;
	}

	private DialogInterface.OnClickListener dialogClose = new DialogInterface.OnClickListener() {

		public void onClick(DialogInterface dialog, int which) {
			dialog.dismiss();
		}
	};

	private Dialog createAlertDialog(int messageId) {

		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setMessage(messageId).setCancelable(false)
				.setPositiveButton(R.string.btnOK, dialogClose);

		return builder.create();

	}

}
