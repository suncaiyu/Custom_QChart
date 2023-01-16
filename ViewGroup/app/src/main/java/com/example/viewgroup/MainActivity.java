package com.example.viewgroup;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        TextView hello = findViewById(R.id.hello);
        hello.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent it = new Intent(MainActivity.this, FlowActivity.class);
                startActivity(it);
            }
        });
    }

    public void jumpClick(View view) {
        Intent it = new Intent(MainActivity.this, TimelineActivity.class);
        startActivity(it);
    }
}