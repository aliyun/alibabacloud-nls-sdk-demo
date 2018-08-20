package com.alibaba.idst.nls.demo;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

@RequiresApi(api = Build.VERSION_CODES.CUPCAKE)
public class MainActivity extends AppCompatActivity
{
    // 要申请的权限
    private String[] permissions = {Manifest.permission.RECORD_AUDIO};

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        requestPermissions();
    }

    private void requestPermissions() {
        // 版本判断。当手机系统大于 23 时，才有必要去判断权限是否获取
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            // 检查该权限是否已经获取
            int i = ContextCompat.checkSelfPermission(this, permissions[0]);
            // 权限是否已经 授权 GRANTED---授权  DINIED---拒绝
            if (i != PackageManager.PERMISSION_GRANTED) {
                // 如果没有授予该权限，就去提示用户请求
                ActivityCompat.requestPermissions(this, permissions, 321);
            }
        }
    }

    public void goRecognizerWithRecorder(View view) {
        Intent intent = new Intent(this, SpeechRecognizerWithRecorderActivity.class);
        startActivity(intent);
    }

    public void goRecognizer(View view) {
        Intent intent = new Intent(this, SpeechRecognizerActivity.class);
        startActivity(intent);
    }

    public void goTranscriberWithRecorder(View view) {
        Intent intent = new Intent(this, SpeechTranscriberWithRecorderActivity.class);
        startActivity(intent);
    }

    public void goTranscriber(View view) {
        Intent intent = new Intent(this, SpeechTranscriberActivity.class);
        startActivity(intent);

    }

    public void goSynthesizer(View view) {
        Intent intent = new Intent(this, SpeechSynthesizerActivity.class);
        startActivity(intent);

    }

}
