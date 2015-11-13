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

import org.json.JSONObject;

import taller2.fiuba.cliente.R;

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
public class AdvancedSearchActivity extends PreferenceActivity {

    private final static Logger LOGGER = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //getFragmentManager().beginTransaction().replace(android.R.id.content, new MyPreferenceFragment()).commit();
        addPreferencesFromResource(R.xml.pref_general);
        setContentView(R.layout.activity_advanced_search);
        setTheme(R.style.GreyText);

    }

    public void search(View view){
        System.out.println("search");
        ListView lv = getListView();
        EditTextPreference etpFilename = (EditTextPreference) lv.getItemAtPosition(0);
        EditTextPreference etpOwner = (EditTextPreference) lv.getItemAtPosition(1);
        EditTextPreference etpTag = (EditTextPreference) lv.getItemAtPosition(2);
        EditTextPreference etpExtension = (EditTextPreference) lv.getItemAtPosition(3);
        String filename = etpFilename.getText().toString();
        String owner = etpOwner.getText().toString();
        String tag = etpTag.getText().toString();
        String extension = etpExtension.getText().toString();
        JSONObject searchParameters = new JSONObject();
        try {
            if (!filename.isEmpty()) {
                searchParameters.put("NAME", filename);
            }
            if (!owner.isEmpty()){
                searchParameters.put("OWNER", owner);
            }
            if(!tag.isEmpty()){
                searchParameters.put("TAG", tag);
            }
            if(!extension.isEmpty()){
                searchParameters.put("EXTENSION", extension);
            }
        } catch (Exception e){}
        System.out.println(searchParameters);
        System.out.println("todo ok");

    }
/*
    public static class MyPreferenceFragment extends PreferenceFragment
    {
        @Override
        public void onCreate(final Bundle savedInstanceState)
        {
            super.onCreate(savedInstanceState);
            addPreferencesFromResource(R.xml.pref_general);
        }
    }
*/

}
