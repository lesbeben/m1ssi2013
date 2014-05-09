package com.java.androidtoken;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.java.dataManager.LocalData;
import com.java.dataManager.OTPMethodType;
import com.java.dataManager.Token;
import com.java.utils.HOTP;
import com.java.utils.IOTP;
import com.java.utils.ISecret;
import com.java.utils.Secret;
import com.java.utils.TOTP;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.text.Layout;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;


/**
 * Ecran d'Ajout d'un Token
 * Ecran affiché lorsque l'utilisateur désire ajouter un Token depuis le menu 
 * Android.
 * Sortie: - si liste des tokens est vide afficher AddTokenActivity
 *         - sinon si "Bouton enregistrement du Token" clické, afficher 
 * 			 TokenListActivity
 *         - sinon si "menu cancel Bouton" clické, afficher l'écran prècédent
 * @author ADEGOLOYE Yves
 */ 


public class AddTokenActivity extends Activity {

	private static final int DIALOG_NO_NAME = 0;
	private static final int DIALOG_NO_SERIAL = 1;
	private static final int DIALOG_TOKEN_NAME_ALREADY_EXIST = 2;
	private static final int DIALOG_INVALID_SERIAL_NOT_HEX = 3;
	private static final int DIALOG_INVALID_SERIAL_INVALID_LEN = 4;
	private boolean isFromLogin;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_token_add);

		Intent i = getIntent();
		isFromLogin = i.getBooleanExtra("from-login", false);

		loadSpinner(R.id.tokenTypeSpinner, R.array.tokenType);
		loadSpinner(R.id.tokenOtpSpinner, R.array.otpLength);

		Button btnComplete = (Button) findViewById(R.id.btnAddStep2);
		btnComplete.setOnClickListener(buttonComplete);

		Spinner otpType = (Spinner) findViewById(R.id.tokenTypeSpinner);
		
		otpType.setOnItemSelectedListener(new OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> parent, View view,
					int position, long id) {
				Spinner otpType = (Spinner) findViewById(R.id.tokenTypeSpinner);
				String methodName = (String) otpType.getSelectedItem();
				if (methodName.equalsIgnoreCase("hotp token")) {
					View v = findViewById(R.id.tokenOtpQuantumSpinner);
					v.setVisibility(View.INVISIBLE);
					v = findViewById(R.id.tokenOtpQuantum);
					v.setVisibility(View.INVISIBLE);
				} else {
					View v = findViewById(R.id.tokenOtpQuantumSpinner);
					v.setVisibility(View.VISIBLE);
					v = findViewById(R.id.tokenOtpQuantum);
					v.setVisibility(View.VISIBLE);
				}
			}

			@Override
			public void onNothingSelected(AdapterView<?> parent) {
				//Ne rien faire				
			}
		});
		
		otpType.setSelection(0);
		((Spinner) findViewById(R.id.tokenOtpSpinner)).setSelection(0);
		View v = findViewById(R.id.tokenOtpQuantumSpinner);
		v.setVisibility(View.INVISIBLE);
		v = findViewById(R.id.tokenOtpQuantum);
		v.setVisibility(View.INVISIBLE);
	}

	private void loadSpinner(int spinnerId, int arrayData) {
		Spinner spinner = (Spinner) findViewById(spinnerId);
		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
				this, arrayData, android.R.layout.simple_spinner_item);
		adapter.setDropDownViewResource(
			android.R.layout.simple_spinner_dropdown_item
		);
		spinner.setAdapter(adapter);
	}

	@Override
	protected Dialog onCreateDialog(int id) {
		Dialog d;

		switch (id) {
		case DIALOG_NO_NAME:
			d = createAlertDialog(R.string.tokenAddDialogNoName);
			break;

		case DIALOG_TOKEN_NAME_ALREADY_EXIST:
			d = createAlertDialog(R.string.tokenAddDialogNameAreadyExist);
			break;

		case DIALOG_INVALID_SERIAL_NOT_HEX:
			d = createAlertDialog(R.string.tokenAddDialogSerialNotHEX);
			break;

		case DIALOG_NO_SERIAL:
			d = createAlertDialog(R.string.tokenAddDialogNoSerial);
			break;
		
		case DIALOG_INVALID_SERIAL_INVALID_LEN:
			d = createAlertDialog(R.string.tokenAddDialogSerialInvalidLen);
			break;
			
		default:
			d = null;
		}

		return d;
	}

	private Dialog createAlertDialog(int messageId) {
		return this.createAlertDialog(messageId, null, dialogClose, null);
	}

	private Dialog createAlertDialog(int messageId, String additionalMessage,
			DialogInterface.OnClickListener positiveClick,
			DialogInterface.OnClickListener negativeClick) {

		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setMessage(messageId).setPositiveButton(R.string.btnOK,
				positiveClick);

		if (negativeClick != null) {
			builder.setNegativeButton(R.string.cancel, negativeClick);
		} else {
			builder.setCancelable(false);
		}

		if (additionalMessage != null) {
			builder.setMessage(String.format(getResources()
					.getString(messageId), additionalMessage));
		}

		return builder.create();
	}

	private DialogInterface.OnClickListener dialogClose = 
			new DialogInterface.OnClickListener() {
		public void onClick(DialogInterface dialog, int which) {
			dialog.dismiss();
		}
	};

	private OnClickListener buttonComplete = new OnClickListener() {

		public void onClick(View v) {

			boolean isValid = true;

			String name = ((EditText) findViewById(R.id.tokenNameEdit))
					.getText().toString();
			String serial = ((EditText) findViewById(R.id.tokenSerialEdit))
					.getText().toString();

			if (name.length() == 0) {
				isValid = false;
				showDialog(DIALOG_NO_NAME);
				return;
			}

			if (LocalData.getInstance().getToken(name) != null) {
				isValid = false;
				showDialog(DIALOG_TOKEN_NAME_ALREADY_EXIST);
				return;
			}

			if (serial.length() == 0) {
				isValid = false;
				showDialog(DIALOG_NO_SERIAL);
				return;
			}
			
			if (serial.length() % 2 == 1) {
				isValid = false;
				showDialog(DIALOG_INVALID_SERIAL_INVALID_LEN);
				return;
			}
			
			Pattern p = Pattern.compile("[A-Fa-f0-9]*");
			Matcher matcher = p.matcher(serial);

			if (!matcher.matches()) {
				isValid = false;
				showDialog(DIALOG_INVALID_SERIAL_NOT_HEX);
				return;
			}

			if (isValid) {

				int tokenType = ((Spinner) findViewById(R.id.tokenTypeSpinner))
					.getSelectedItemPosition();
				int otpLength = Integer
					.parseInt(((Spinner) findViewById(R.id.tokenOtpSpinner))
						.getSelectedItem().toString());
				int quantum = Integer.parseInt(
					((Spinner) findViewById(R.id.tokenOtpQuantumSpinner)
				).getSelectedItem().toString());
				
				ISecret key = new Secret();
				key.setSecret(serial);
				IOTP gene = null;
				if (tokenType == 0) {
					gene = new HOTP(0, key, otpLength);
				} else {
					gene = new TOTP(key, otpLength, quantum);
				}

				Token token = new Token(name,
						tokenType == 0 ? OTPMethodType.HOTP
								: OTPMethodType.TOTP, gene);

				LocalData.getInstance().addToken(token);
				LocalData.getInstance().commit(getApplicationContext());

				if (!isFromLogin) {
					setResult(RESULT_OK);
				} else {
					Intent i = new Intent(AddTokenActivity.this,
							TokenListActivity.class);
					startActivity(i);
				}

				finish();
			}

		}
	};

}
