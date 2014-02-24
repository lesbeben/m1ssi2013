#define _XOPEN_SOURCE 700
#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "users.h"

otpuser user1;
otpuser user2;

int secretcmp(secret s1, secret s2) {
    if (s1->length != s2->length) {
        return -1;
    }
    for (int i = 0; i < s1->length; i++) {
        if (s1->buffer[i] != s2->buffer[i]) {
            return -1;
        }
    }
    return 0;
}

/** Test d'enregistrement de trois utilisateurs.
 */
START_TEST (test_register_user) {
    user1.username = strndup("usertest", 9);
    user1.method = HOTP_METHOD;
    user1.passwd = createSecret(20);
    user1.params.count = 0;
    ck_assert_msg( updateOTPUser(&user1) == 0,
        "Erreur lors de l'enregistrement du premier utilisateur:\n %s", 
        user1.username
    );

    user2.username = strndup("pedro", 6);
    user2.method = HOTP_METHOD;
    user2.passwd = createSecret(20);
    user2.params.count = 0;
    ck_assert_msg( updateOTPUser(&user2) == 0,
        "Erreur lors de l'enregistrement du second utilisateur:\n %s", 
        user2.username
    );

    otpuser user3;
    user3.username = strndup("johndoe", 8);
    user3.method = HOTP_METHOD;
    user3.passwd = createSecret(20);
    user3.params.count = 0;
    ck_assert_msg( updateOTPUser(&user3) == 0,
        "Erreur lors de l'enregistrement du troisième utilisateur:\n %s", 
        user3.username
    );
}
END_TEST

/** Test d'obtention de données d'utilisateurs.
 */
START_TEST (test_get_user) {
    otpuser * user = getOTPUser(user2.username); 
    ck_assert_msg( user != NULL,
        "Erreur lors de l'acces au deuxième utilisateur (inexistant):\n %s",
        user2.username
    );
    ck_assert_msg( user->username != NULL, 
        "Erreur nom d'utilisateur vide, attendu:\n %s",
        user2.username
    );
    
    if (user->username != NULL) {
        ck_assert_msg( strcmp(user->username, user2.username) == 0,
            "Erreur nom d'utilisateur incohérent:\n %s != %s",
            user2.username, user->username
        );
    }
    
    ck_assert_msg( user->method != user2.method,
        "Methode incohérente:\n %d != %d",
        user2.method, user->method
    );
    
    if (user2.method == user->method) {
        switch (user2.method) {
            case HOTP_METHOD:
                ck_assert_msg(user2.params.count == user->params.count,
                    "HOTP:Erreur les compteur ne sont pas cohérents:\n%d != %d",
                    user2.params.count, user->params.count
                );
                break;
            case TOTP_METHOD:
                
                break;
        }
    }

    ck_assert_msg(secretcmp(user->passwd, user2.passwd) == 0,
        "Erreur les secrets ne coïncident pas."
    );
}
END_TEST

/** Test de mise à jour des données d'un utilisateur.
 */
START_TEST (test_update_user_secret) {
    destroySecret(user2.passwd);
    user2.passwd = createSecret(20);
    ck_assert_msg(updateOTPUser(&user2) == 0,
        "Erreur lors de la mise à jour de l'utilisateur: %s",
        user2.username
    );
}
END_TEST


START_TEST (test_update_user_hotp_count) {
    user2.params.count += 1;
    ck_assert_msg(updateOTPUser(&user2) == 0,
        "Erreur lors de la mise à jour de l'utilisateur: %s",
        user2.username
    );
}
END_TEST

START_TEST (test_destroy_user) {
    ck_assert_msg( DestroyOTPUser(user2.username) == 0,
        "Erreur lors de la suppression d'un utilisateur: %s",
        user1.username
    );
    ck_assert_msg(getOTPUser(user2.username) != 0,
        "Erreur l'utilisateur ne devrait plus être présent dans le fichier."
    );
}
END_TEST

Suite * users_suite (void) {
    Suite * s = suite_create("Gestion users");
    TCase * tc_register = tcase_create("Enregistrement");
    tcase_add_test(tc_register, test_register_user);
    
    TCase * tc_get = tcase_create("Lecture");
    tcase_add_test(tc_get, test_get_user);
    
    TCase * tc_update = tcase_create("Mise à jour");
    tcase_add_test(tc_update, test_update_user_secret);
    tcase_add_test(tc_update, test_get_user);
    tcase_add_test(tc_update, test_update_user_hotp_count);
    tcase_add_test(tc_update, test_get_user);
    
    TCase * tc_destroy = tcase_create("Suppression");
    tcase_add_test(tc_destroy, test_destroy_user);
    
    suite_add_tcase(s, tc_register);
    suite_add_tcase(s, tc_get);
    suite_add_tcase(s, tc_update);
    suite_add_tcase(s, tc_destroy);
    return s;
}

int main(int argc, char * argv[]) {
    int failed_count = 0;
    Suite * s = users_suite();
    SRunner * sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    failed_count = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
