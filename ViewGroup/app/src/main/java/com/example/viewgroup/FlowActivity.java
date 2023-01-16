package com.example.viewgroup;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class FlowActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_flow);
        ViewGroup vg = findViewById(R.id.flow_root);
        for (int i = 0; i < vg.getChildCount(); i++) {
            View v = vg.getChildAt(i);
            v.setOnClickListener(this);
        }
    }

    @Override
    public void onClick(View view) {
        if (view instanceof TextView) {
            Log.d(getClass().getName(), ((TextView) view).getText().toString());
            view.setVisibility(View.GONE);
        }
    }
}