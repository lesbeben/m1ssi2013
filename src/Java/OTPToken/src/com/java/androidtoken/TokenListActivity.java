package com.java.androidtoken;

import java.util.List;
import com.java.dataManager.LocalData;
import com.java.dataManager.OTPMethodType;
import com.java.dataManager.Token;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ListActivity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;

/**
 * Ecran d'accueil regroupant la liste des OTP créés.
 * Sortie -si "Bouton menu Android AddToken" cliké, afficher AddTokenActivity
 *       -si "Bouton menu Android RemoveToken" clické , 
 *       		afficher RemoveTokenActivity
 *       -sinon si "menu cancel Bouton" clické, éteindre l'application
 *       
 *@author ADEGOLOYE Yves
 */

public class TokenListActivity extends ListActivity {

	private static final int ACTIVITY_ADD_TOKEN = 0;
	private static final int ACTIVITY_CHANGE_PIN = 1;

	private static final int MENU_ADD_TOKEN = Menu.FIRST;
	private static final int MENU_CHANGE_PIN = Menu.FIRST + 1;
	private static final int MENU_DELETE_TOKEN = Menu.FIRST + 2;

	private static final int DIALOG_INVALID_PIN = 0;
	private static final int DIALOG_OTP = 1;
	private static final int DIALOG_DELETE_TOKEN = 2;

	private int mSelectedTokenId = -1;
	private int mTokenToDeleteId = -1;

	private ListView lv;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_token_list);

		lv = (ListView) findViewById(android.R.id.list);
		lv.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
			public boolean onItemLongClick(AdapterView<?> arg0, View v,
					int pos, long id) {
				return onLongListItemClick(v, pos, id);
			}
		});

		TokenAdapter mtokenAdaptor = new TokenAdapter(this);
		setListAdapter(mtokenAdaptor);
		fillData();

	}

	@Override
	protected void onResume() {
		super.onResume();
		TokenListActivity.this.fillData();
	}

	private void fillData() {
		((BaseAdapter) lv.getAdapter()).notifyDataSetChanged();
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	protected boolean onLongListItemClick(View v, final int pos, final long id) {

		AlertDialog.Builder builder = new AlertDialog.Builder(this);

		builder.setTitle(R.string.app_name)
				.setMessage(R.string.confirmDelete)
				.setIcon(android.R.drawable.ic_dialog_alert)
				.setPositiveButton(R.string.btnOK,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int which) {

								LocalData.getInstance().removeToken(pos);
								LocalData.getInstance().commit(
										getApplicationContext());
								fillData();

								Toast.makeText(getApplicationContext(),
										R.string.toastDeleted,
										Toast.LENGTH_SHORT).show();
							}
						}).setNegativeButton(R.string.cancel, null);

		builder.show();

		return true;
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

	@Override
	protected Dialog onCreateDialog(int id) {
		Dialog d;

		switch (id) {

		case DIALOG_INVALID_PIN:
			d = createAlertDialog(R.string.pinAlertInvalidPin);
			break;

		case DIALOG_OTP:
			d = new Dialog(this);

			d.setContentView(R.layout.otpdialog);
			d.setTitle(R.string.otpDialogTitle);

			ImageView image = (ImageView) d.findViewById(R.id.otpDialogImage);
			image.setImageResource(R.drawable.icon2);
			break;

		case DIALOG_DELETE_TOKEN:
			d = createDeleteTokenDialog();
			break;

		default:
			d = null;

		}
		return d;
	}

	@Override
	protected void onPrepareDialog(int id, Dialog dialog) {
		super.onPrepareDialog(id, dialog);

		switch (id) {
		case DIALOG_OTP:

			TextView text = (TextView) dialog.findViewById(R.id.otpDialogText);
			text.setText(generateOtp(mSelectedTokenId));
			break;

		case DIALOG_DELETE_TOKEN:
			mTokenToDeleteId = -1;
			break;
		}
	}

	private String generateOtp(int pos) {
		Token token = LocalData.getInstance().getListeToken().get(pos);
		int otp = token.generate();
		LocalData.getInstance().commit(getApplicationContext());
		return Integer.toString(otp);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		menu.add(0, MENU_ADD_TOKEN, 0, R.string.menu_add_token).setIcon(
				android.R.drawable.ic_menu_add);
		menu.add(0, MENU_CHANGE_PIN, 1, R.string.menu_pin_change).setIcon(
				android.R.drawable.ic_lock_lock);
		menu.add(0, MENU_DELETE_TOKEN, 2, R.string.menu_delete_token).setIcon(
				android.R.drawable.ic_menu_delete);
		return true;
	}

	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		menu.findItem(MENU_DELETE_TOKEN).setEnabled(
				this.getListView().getCount() > 0);
		return true;
	}

	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		switch (item.getItemId()) {
		case MENU_ADD_TOKEN:
			createToken();
			return true;

		case MENU_CHANGE_PIN:
			changePin();
			return true;

		case MENU_DELETE_TOKEN:
			showDialog(DIALOG_DELETE_TOKEN);
			return true;

		}

		return super.onMenuItemSelected(featureId, item);
	}

	private void changePin() {
		Intent i = new Intent(this, SetPINActivity.class);
		i.putExtra("change_pin", true);
		startActivityForResult(i, ACTIVITY_CHANGE_PIN);
	}

	private void createToken() {
		Intent intent = new Intent(this, AddTokenActivity.class);
		startActivityForResult(intent, ACTIVITY_ADD_TOKEN);
	}

	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {
		super.onListItemClick(l, v, position, id);
		mSelectedTokenId = (int) id;
		showDialog(DIALOG_OTP);
	}

	private Dialog createDeleteTokenDialog() {
		Dialog d;
		AlertDialog.Builder builder = new AlertDialog.Builder(this);

		builder.setTitle(R.string.app_name)
				.setSingleChoiceItems(
						new TokenAdapter(getApplicationContext()), -1,
						deleteTokenEvent)
				.setPositiveButton(R.string.btnOK, deleteTokenPositiveEvent)
				.setNegativeButton(R.string.cancel, deleteTokenNegativeEvent);

		d = builder.create();
		return d;
	}

	private DialogInterface.OnClickListener deleteTokenPositiveEvent = new DialogInterface.OnClickListener() {

		public void onClick(DialogInterface dialog, int which) {

			if (mTokenToDeleteId >= 0) {
				LocalData.getInstance().removeToken(mTokenToDeleteId);
				LocalData.getInstance().commit(getApplicationContext());
				mTokenToDeleteId = -1;
				fillData();
				removeDialog(DIALOG_DELETE_TOKEN);
			}
		}
	};

	private DialogInterface.OnClickListener deleteTokenNegativeEvent = new DialogInterface.OnClickListener() {
		public void onClick(DialogInterface dialog, int which) {
			removeDialog(DIALOG_DELETE_TOKEN);
		}
	};

	private DialogInterface.OnClickListener deleteTokenEvent = new DialogInterface.OnClickListener() {

		public void onClick(DialogInterface dialog, int which) {
			mTokenToDeleteId = which;

		}
	};

	private class TokenAdapter extends BaseAdapter {
		private Context mContext;
		private List<Token> listeTokens;

		public TokenAdapter(Context context) {
			mContext = context;
			listeTokens = LocalData.getInstance().getListeToken();
		}

		@Override
		public int getCount() {
			return listeTokens.size();
		}

		@Override
		public Object getItem(int position) {
			return listeTokens.get(position);
		}

		@Override
		public long getItemId(int position) {
			return (long) position;
		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {

			LayoutInflater inflater = (LayoutInflater) mContext
					.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			View row = inflater.inflate(R.layout.token_list_row, null);

			TextView nameText = (TextView) row
					.findViewById(R.id.tokenrowtextname);
			TextView methodText = (TextView) row
					.findViewById(R.id.tokenrowtextmethod);
			ImageView tokenImage = (ImageView) row
					.findViewById(R.id.ivTokenIcon);

			Token currentToken = (Token) getItem(position);

			nameText.setText(currentToken.getNom());
			methodText
					.setText(currentToken.getMethodType() == OTPMethodType.HOTP ? "HOTP"
							: "TOTP");
			tokenImage
					.setImageResource(currentToken.getMethodType() == OTPMethodType.HOTP ? R.drawable.round_add
							: R.drawable.clock32);

			return row;
		}
	}

}
