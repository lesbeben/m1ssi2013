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

public class PINLoginActivity extends Activity {

	/**
	 * Ecran de connexion à l'application Ecran affiché après l'écran splash (si
	 * ce n'est pas la première utilisation de l'application) Sortie: -si le PIN
	 * est OK, afficher TokenListActivity -sinon, Dialog de PIN incorrect.
	 * 
	 * @author ADEGOLOYE Yves
	 */

	private static final int DIALOG_INVALID_PIN = 1;
	private static final int DIALOG_PIN_EDITVIEW_NULL = 0;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_login);

		Button loginBtn = (Button) findViewById(R.id.mainLogin);
		loginBtn.setOnClickListener(loginListener);
	}

	private OnClickListener loginListener = new OnClickListener() {

		@Override
		public void onClick(View arg0) {
			String pin = ((EditText) findViewById(R.id.mainPinEdit)).getText()
					.toString();

			if (!pin.equalsIgnoreCase("")) {
				boolean validate = LocalData.getInstance().validatePin(
						getApplicationContext(), pin);

				if (!validate) {
					showDialog(DIALOG_INVALID_PIN);
					return;
				} else {

					LocalData.getInstance().setPIN(pin);

					LocalData.getInstance().load(getApplicationContext());

					Intent i = null;
					if (LocalData.getInstance().getListeToken().size() != 0) {
						i = new Intent(PINLoginActivity.this,
								TokenListActivity.class);
					} else {
						i = new Intent(PINLoginActivity.this,
								AddTokenActivity.class);
						i.putExtra("from-login", true);
					}

					startActivity(i);
					finish();
				}

			} else {
				showDialog(DIALOG_PIN_EDITVIEW_NULL);
				return;
			}

		}

	};

	@Override
	protected Dialog onCreateDialog(int id) {
		Dialog d;

		switch (id) {

		case DIALOG_INVALID_PIN:
			d = createAlertDialog(R.string.pinAlertInvalidPin);
			break;

		case DIALOG_PIN_EDITVIEW_NULL:
			d = createAlertDialog(R.string.loginEditPinEmpty);
			break;

		default:
			d = null;
			break;

		}

		return d;
	}

	private Dialog createAlertDialog(int messageId) {

		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setMessage(messageId).setCancelable(false)
				.setPositiveButton(R.string.btnOK, dialogClose);

		return builder.create();

	}

	private DialogInterface.OnClickListener dialogClose = new DialogInterface.OnClickListener() {
		public void onClick(DialogInterface dialog, int which) {
			dialog.dismiss();
		}
	};

}
