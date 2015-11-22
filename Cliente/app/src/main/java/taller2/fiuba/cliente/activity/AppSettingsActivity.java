package taller2.fiuba.cliente.activity;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.EditTextPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceCategory;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.preference.PreferenceScreen;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import taller2.fiuba.cliente.R;

/**
 * Actividad de Settings, para permitir al usuario modificar opciones del programa
 */
public class AppSettingsActivity extends PreferenceActivity {

    /**
     * Constructor. Actualzia el manejador de fragmentos
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("AppSettingsActivity", "Se creo la actividad");
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_app_settings);
        setTheme(R.style.GreyText);
        getFragmentManager().beginTransaction().replace(android.R.id.content, new MyPreferenceFragment()).commit();
    }

    /**
     * Fragmento que contiene las preferencias a mostrar
     */
    public static class MyPreferenceFragment extends PreferenceFragment
                                            implements SharedPreferences.OnSharedPreferenceChangeListener {
        /**
         * Constructor.
         * @param savedInstanceState
         */
        @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);

            // Load the preferences_app_settings from an XML resource
            addPreferencesFromResource(R.xml.preferences_app_settings);
            Log.d("Preference Fragment", "Se cargaron las opciones");
            // show the current value in the settings screen
            for (int i = 0; i < getPreferenceScreen().getPreferenceCount(); i++) {
                pickPreferenceObject(getPreferenceScreen().getPreference(i));
            }
        }

        /**
         * Funcion para llamar a actualizar las descripciones de las preferencias
         * @param p
         */
        private void pickPreferenceObject(Preference p) {
            if (p instanceof PreferenceCategory) {
                PreferenceCategory cat = (PreferenceCategory) p;
                for (int i = 0; i < cat.getPreferenceCount(); i++) {
                    pickPreferenceObject(cat.getPreference(i));
                }
            } else {
                initSummary(p);
            }
        }

        /**
         * Actualiza descripcion de una preferencia para que coincida con su valor actual.
         * @param p
         */
        private void initSummary(Preference p) {
            if (p instanceof EditTextPreference) {
                EditTextPreference editTextPref = (EditTextPreference) p;
                p.setSummary(editTextPref.getText());
                Log.d("AppSettingsActivity", "Actualizando resumen de preferencia " + p.getTitle());
            }

            // More logic for ListPreference, etc...
        }

        /**
         * Actualiza SharedPreferences frente a un cambio de las mismas por el usuario.
         * @param sharedPreferences
         * @param key
         */
        @Override
        public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
            Preference pref = findPreference(key);
            pref.setSummary(sharedPreferences.getString(key, ""));
            Log.d("AppSettingsActivity", "Se registro un cambio por el usuario a los settings del programa");
        }

        /**
         * Define a este objeto como listener de cambios a shared preferences
         */
        @Override
        public void onResume() {
            super.onResume();
            getPreferenceScreen()
                    .getSharedPreferences()
                    .registerOnSharedPreferenceChangeListener(this);
        }

        /**
         * Define a este objeto como listener de cambios a shared preferences
         */
        @Override
        public void onPause() {
            super.onPause();
            getPreferenceScreen()
                    .getSharedPreferences()
                    .unregisterOnSharedPreferenceChangeListener(this);
        }
    }
}
