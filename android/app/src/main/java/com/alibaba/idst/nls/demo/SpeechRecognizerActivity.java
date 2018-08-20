package com.alibaba.idst.nls.demo;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

import com.alibaba.fastjson.JSONObject;
import com.alibaba.idst.util.NlsClient;
import com.alibaba.idst.util.SpeechRecognizer;
import com.alibaba.idst.util.SpeechRecognizerCallback;

import java.lang.ref.WeakReference;
import java.nio.ByteBuffer;

public class SpeechRecognizerActivity extends AppCompatActivity {
    private static final String TAG="AliSpeechDemo";

    private EditText mFullEdit;
    private EditText mResultEdit;
    private static Handler handler;
    // Demo录音线程
    private MyRecorder myRecorder = new MyRecorder();
    private NlsClient client;
    private SpeechRecognizer speechRecognizer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_speech_recognizer);
        mFullEdit = (EditText) findViewById(R.id.editText2);
        mResultEdit = (EditText) findViewById(R.id.editText);

        //第一步，创建client实例，client只需要创建一次，可以用它多次创建recognizer
        client = new NlsClient();
        //UI在主线程更新
        handler= new MyHandler(this);
    }

    @Override
    public void onDestroy(){
        if (speechRecognizer != null){
            speechRecognizer.stop();
        }

        // 最终，退出时释放client
        client.release();
        super.onDestroy();
    }

    // 此方法内启动录音和识别逻辑，为了代码简单便于理解，没有加防止用户重复点击的逻辑，用户应该在真实使用场景中注意
    public void startRecognizer(View view){
        mFullEdit.setText("");
        mResultEdit.setText("");

        // 第二步，新建识别回调类
        SpeechRecognizerCallback callback = new MyCallback();

        // 第三步，创建识别request
        speechRecognizer = client.createRecognizerRequest(callback);

        // 第四步，设置相关参数
        // 请使用https://help.aliyun.com/document_detail/72153.html 动态生成token
        speechRecognizer.setToken("");
        // 请使用阿里云语音服务管控台(https://nls-portal.console.aliyun.com/)生成您的appkey
        speechRecognizer.setAppkey("");
        // 设置返回中间结果，更多参数请参考官方文档
        speechRecognizer.enableIntermediateResult(true);

        //启动录音线程
        myRecorder.recordTo(speechRecognizer);
    }

    public void stopRecognizer(View view){
        // 停止录音
        myRecorder.stop();
        // 第八步，停止本次识别
        speechRecognizer.stop();
    }

    // 语音识别回调类，得到语音识别结果后在这里处理
    // 注意不要在回调方法里调用recognizer.stop()方法
    // 注意不要在回调方法里执行耗时操作
    private static class MyCallback implements SpeechRecognizerCallback {

        // 识别开始
        @Override
        public void onRecognizedStarted(String msg, int code)
        {
            Log.d(TAG,"OnRecognizedStarted " + msg + ": " + String.valueOf(code));
        }

        // 请求失败
        @Override
        public void onTaskFailed(String msg, int code)
        {
            Log.d(TAG,"OnTaskFailed " + msg + ": " + String.valueOf(code));
            handler.sendEmptyMessage(0);
        }

        // 识别返回中间结果，只有开启相关选项时才会回调
        @Override
        public void onRecognizedResultChanged(final String msg, int code)
        {
            Log.d(TAG,"OnRecognizedResultChanged " + msg + ": " + String.valueOf(code));
            Message message = new Message();
            message.obj = msg;
            handler.sendMessage(message);
        }

        // 第七步，识别结束，得到最终完整结果
        @Override
        public void onRecognizedCompleted(final String msg, int code)
        {
            Log.d(TAG,"OnRecognizedCompleted " + msg + ": " + String.valueOf(code));
            Message message = new Message();
            message.obj = msg;
            handler.sendMessage(message);
        }

        // 请求结束，关闭连接
        @Override
        public void onChannelClosed(String msg, int code) {
            Log.d(TAG, "OnChannelClosed " + msg + ": " + String.valueOf(code));
        }

    };

    // 根据识别结果更新界面的代码
    private static class MyHandler extends Handler {
        private final WeakReference<SpeechRecognizerActivity> mActivity;

        public MyHandler(SpeechRecognizerActivity activity) {
            mActivity = new WeakReference<SpeechRecognizerActivity>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            if (msg.obj == null) {
                Log.i(TAG, "Empty message received.");
                return;
            }
            String fullResult = (String)msg.obj;
            String result = null;
            if (!fullResult.equals("")){
                JSONObject jsonObject = JSONObject.parseObject(fullResult);
                if (jsonObject.containsKey("payload")){
                    result = jsonObject.getJSONObject("payload").getString("result");
                }
            }
            mActivity.get().mFullEdit.setText(fullResult);
            mActivity.get().mResultEdit.setText(result);
        }
    }

    // 录音并发送给识别的代码，客户可以直接使用
    private static class MyRecorder implements Runnable {
        final static int SAMPLE_RATE = 16000;
        int SAMPLES_PER_FRAME = 640;
        private AudioRecord mAudioRecorder;
        private SpeechRecognizer recognizer;
        private volatile boolean sending = false;
        private Thread thread;

        public void recordTo(SpeechRecognizer recognizer){
            this.recognizer = recognizer;
            Log.d(TAG,"init audio recorder");
            int bufferSizeInBytes = AudioRecord.getMinBufferSize(SAMPLES_PER_FRAME,
                    AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT);
            mAudioRecorder = new AudioRecord(MediaRecorder.AudioSource.MIC, SAMPLE_RATE,
                    AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, bufferSizeInBytes * 2);

            sending = true;
            thread = new Thread(this);
            thread.start();
        }

        public void stop(){
            sending = false;
        }

        @Override
        public void run() {
            try {
                if (mAudioRecorder != null && mAudioRecorder.getState() == 1) {
                    Log.d(TAG, "mAudioRecorder state is : " + String.valueOf(mAudioRecorder.getState()));
                    try {
                        mAudioRecorder.stop();
                        mAudioRecorder.startRecording();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

                // 第五步，启动语音识别
                int startCode = recognizer.start();
                if(startCode < 0)
                {
                    Log.i(TAG, "启动语音识别失败！");
                    recognizer.stop();
                    return;
                }
                Log.d(TAG,"recognizer start done");

                ByteBuffer buf = ByteBuffer.allocateDirect(SAMPLES_PER_FRAME);
                while(sending){
                    buf.clear();
                    // 采集语音
                    int readBytes = mAudioRecorder.read(buf, SAMPLES_PER_FRAME);
                    byte[] bytes = new byte[SAMPLES_PER_FRAME];
                    buf.get(bytes, 0, SAMPLES_PER_FRAME);
                    if (readBytes>0 && sending){
                        // 第六步，发送语音数据到识别服务
                        int code = recognizer.sendAudio(bytes, bytes.length);
                        if (code < 0) {
                            Log.i(TAG, "发送语音失败！");
                            break;
                        }
                    }
                    buf.position(readBytes);
                    buf.flip();
                }
                mAudioRecorder.stop();
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }
}
