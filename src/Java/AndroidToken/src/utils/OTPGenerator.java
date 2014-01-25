package utils;
/**
 * Une classe abstraite pour les générateurs d'OTP.
 *
 * @author celtic
 */
public abstract class OTPGenerator implements IOTP {
    @Override
	public int generer() {
    	return 0;
    }

	/**
	 * Retourne la valeur du compteur actuel du générateur d'OTP. 
	 * @return La valeur du compteur.
	 */
	protected abstract long getCount();
	
	/**
	 * Fixe la valeur du compteur du générateur d'OTP.
	 */
	protected abstract void setCount();

	
}
