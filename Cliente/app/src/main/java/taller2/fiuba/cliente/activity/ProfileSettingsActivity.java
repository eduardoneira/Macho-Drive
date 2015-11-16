package taller2.fiuba.cliente.activity;

import android.annotation.TargetApi;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.database.Cursor;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceCategory;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.preference.RingtonePreference;
import android.text.TextUtils;
import android.view.MenuItem;
import android.support.v4.app.NavUtils;
import android.view.View;
import android.widget.AdapterView;
import android.widget.EditText;
import android.widget.ListView;

import org.json.JSONException;
import org.json.JSONObject;

import taller2.fiuba.cliente.R;
import taller2.fiuba.cliente.model.Request;

import java.util.List;
import java.util.logging.Logger;


/**
 * A {@link PreferenceActivity} that presents a set of application settings. On
 * handset devices, settings are presented as a single list. On tablets,
 * settings are split by category, with category headers shown to the left of
 * the list of settings.
 * <p/>
 * See <a href="http://developer.android.com/design/patterns/settings.html">
 * Android Design: Settings</a> for design guidelines and the <a
 * href="http://developer.android.com/guide/topics/ui/settings.html">Settings
 * API Guide</a> for more information on developing a Settings UI.
 */
public class ProfileSettingsActivity extends PreferenceActivity {

    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);
    private String token, username;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getFragmentManager().beginTransaction().replace(android.R.id.content, new MyPreferenceFragment()).commit();
        addPreferencesFromResource(R.xml.pref_profile_settings);
        setContentView(R.layout.activity_profile_settings);
        setTheme(R.style.GreyText);
        username = getIntent().getStringExtra("username");
        token = getIntent().getStringExtra("token");
    }

    public void saveChanges(View view){
        System.out.println("save changes");
        try {
            ListView lv = getListView();
            EditTextPreference etpUsername = (EditTextPreference) lv.getItemAtPosition(0);
            EditTextPreference etpEmail = (EditTextPreference) lv.getItemAtPosition(1);
            String newUsername = etpUsername.getText().toString();
            String newEmail = etpEmail.getText().toString();
            JSONObject data = new JSONObject();
            data.put("email", newEmail);
            Request request = new Request("PUT", "/users/"+username, data);
            request.setHeader("conn_token", token);
            request.send();
        } catch (JSONException e){
            System.out.println("error al guardar cambios");
        }
    }
}