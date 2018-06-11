package nls.idst.alibaba.com.newgatewaysdkdemo;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.support.annotation.RequiresApi;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import com.alibaba.fastjson.JSONObject;
import com.alibaba.idst.util.NlsClient;
import com.alibaba.idst.util.NlsRequest;
import com.alibaba.idst.util.NlsSpeechCallback;

import java.nio.ByteBuffer;

import static android.media.AudioFormat.ENCODING_PCM_16BIT;

@RequiresApi(api = Build.VERSION_CODES.CUPCAKE)
public class MainActivity extends AppCompatActivity
{

    private static final String TAG="AliSpeech";
    NlsClient client = new NlsClient(true);
    public final static int SAMPLE_RATE = 16000;
    private AudioRecord mAudioRecorder = new AudioRecord(MediaRecorder.AudioSource.MIC, SAMPLE_RATE,
            AudioFormat.CHANNEL_IN_MONO, ENCODING_PCM_16BIT, 3200*10);
    private int WAVE_FRAM_SIZE = 3200;
    private volatile boolean isover = true;
    private volatile boolean isstart = false;
    private NlsRequest nlsRequest;
    private EditText mFullEdit;
    private EditText mResultEdit;
    private Handler handler;
    private volatile String fullResult;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d(TAG,"init audio recorder");
        mFullEdit = (EditText) findViewById(R.id.editText2);
        mResultEdit = (EditText) findViewById(R.id.editText);
        mAudioRecorder.setPositionNotificationPeriod(WAVE_FRAM_SIZE);


        //主线程更新UI
        handler=new Handler(){
            @Override
            public void handleMessage(Message msg) {
                String payload = null;
                if (fullResult != null && fullResult != ""){
                    JSONObject jsonObject = JSONObject.parseObject(fullResult);
                    if (jsonObject.containsKey("payload")){
                        payload = jsonObject.getString("payload");
                    }
                }
                mFullEdit.setText(fullResult);
                mResultEdit.setText(payload);
            }
        };
    }

    //点击开始语音识别
    public void asrClick(View view)
    {
        mFullEdit.setText("");
        mResultEdit.setText("");
        Log.d(TAG,"asr click to start");
        start(client,callback);
    }

    //点击结束语音识别
    public void asrStop(View view){
        Log.d(TAG,"asr click to stop");
        stop();
    }

    //Demo录音程序
    private Runnable recordRun =new Runnable() {
        int SAMPLES_PER_FRAME = 640;

        final ByteBuffer buf = ByteBuffer.allocateDirect(SAMPLES_PER_FRAME);
        int readBytes;
        @Override
        public void run() {
            try {
                Log.d(TAG,"audio state is:"+ mAudioRecorder.getState());
                if (mAudioRecorder != null && mAudioRecorder.getState() == 1) {
                    Log.d("VoiceRecorder", "mAudioRecorder state is : " + String.valueOf(mAudioRecorder.getState()));

                    try {
                        mAudioRecorder.stop();
                        mAudioRecorder.startRecording();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

                while(!isover){
                    buf.clear();
                    readBytes = mAudioRecorder.read(buf, SAMPLES_PER_FRAME);
                    byte[] bytes = new byte[SAMPLES_PER_FRAME];
                    buf.get(bytes, 0, SAMPLES_PER_FRAME);
                    if (readBytes>0 && !isover){
                        nlsRequest.sendAudio(bytes, bytes.length);
                    }
                    buf.position(readBytes);
                    buf.flip();

                }
                mAudioRecorder.stop();
            }catch (Exception e){
                e.printStackTrace();
            }


        }
    };


    //语音识别设置
    public void start(NlsClient client, NlsSpeechCallback callback)
    {
        nlsRequest = client.createRecognizerRequest(callback, null);
        //请使用https://help.aliyun.com/document_detail/72153.html 动态生成token
        //<AccessKeyId> <AccessKeySecret> 请使用您的阿里云账户生成 https://ak-console.aliyun.com/
        nlsRequest.setToken("");
        //请使用阿里云语音服务管控台(https://nls-portal.console.aliyun.com/)生成您的appkey
        nlsRequest.setAppkey("");
        //语音格式，设置为opu压缩格式，节省流量
        nlsRequest.setFormat("opu");
        nlsRequest.setSampleRate("16000");
        nlsRequest.setEnableIntermediateResult(true);
        Log.d(TAG,"set params done");
        if(nlsRequest.start() < 0)
        {
            nlsRequest.release();
            return;
        }
        Log.d(TAG,"recognizer start done");
        isover = false;
        //启动录音线程
        Thread recordThread =new Thread(recordRun);
        recordThread.start();
        isstart = true;
    }

    public void stop(){
        if (isstart){
            isstart = false;
            isover = true;
            nlsRequest.stop();
        }
    }


    //语音识别回调
    NlsSpeechCallback callback = new NlsSpeechCallback()
    {
        //识别开始
        @Override
        public void OnRecognizedStarted(String msg, int code)
        {
            Log.d(TAG,"OnRecognizedStarted " + msg + ": " + String.valueOf(code));
        }

        //请求失败
        @Override
        public void OnTaskFailed(String msg, int code)
        {
            Log.d(TAG,"OnTaskFailed " + msg + ": " + String.valueOf(code));
//            fullResult = msg;
            handler.sendEmptyMessage(0);
        }

        //识别返回中间结果
        @Override
        public void OnRecognizedResultChanged(String msg, int code)
        {
            Log.d(TAG,"OnRecognizedResultChanged " + msg + ": " + String.valueOf(code));
            fullResult = msg;
            handler.sendEmptyMessage(0);
        }

        //识别结束，返回最终结果
        @Override
        public void OnRecognizedCompleted(String msg, int code)
        {
            Log.d(TAG,"OnRecognizedCompleted " + msg + ": " + String.valueOf(code));
            fullResult = msg;
            handler.sendEmptyMessage(0);
            isstart = false;
            isover = true;
            nlsRequest.release();
        }

        //请求结束，关闭请求
        @Override
        public void OnChannelClosed(String msg, int code) {
            Log.d(TAG, "OnChannelClosed " + msg + ": " + String.valueOf(code));
        }

    };


    @Override
    public void onDestroy()
    {
        client.ReleaseClient();
        mAudioRecorder.release();
        mAudioRecorder = null;
        super.onDestroy();
    }
}
