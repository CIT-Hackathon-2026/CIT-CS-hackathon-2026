// --- システム定数 ---
static final int   BUFFER_SIZE = 512;
static final float DEFAULT_AMP = 0.5f;
static final int   SERIAL_BAUD = 9600;
static final int   SAMPLE_RATE = 48000; // 録音時と同じレートに統一

// --- SynthVoice用定数 ---
// 倍音構成パラメータ
static final float[] SYNTH_HARMONICS = {
  1.000f, 0.036f, 0.352f, 0.035f, 0.259f, 0.035f, 0.189f, 0.029f, 0.141f, 0.022f,
  0.107f, 0.018f, 0.065f, 0.011f, 0.011f, 0.006f, 0.006f, 0.001f, 0.004f, 0.001f,
  0.001f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f,
  0.000f, 0.000f, 0.000f, 0.000f
};

// 音量ADSRエンベロープ設定
static final float SYNTH_ATK_TIME = 0.02f;
static final float SYNTH_DEC_TIME = 0.0f;
static final float SYNTH_SUS_LVL  = 1.0f;
static final float SYNTH_REL_TIME = 0.06f;

// --- ノイズ1（ローパス＋ADSR変調）設定 ---
static final float SYNTH_NOISE_AMP = 0.06f;

// --- ノイズ2（固定ローパス）設定 ---
static final float SYNTH_NOISE2_AMP    = 0.002f;
static final float SYNTH_NOISE2_CUTOFF = 13500.0f;

// --- フィルター（ノイズ1専用）ADSR設定 ---
static final float SYNTH_FILTER_TARGET_FREQ = 8000.0f;
static final float SYNTH_FILTER_ATK_TIME = 0.02f;
static final float SYNTH_FILTER_DEC_TIME = 0.30f;
static final float SYNTH_FILTER_SUS_LVL  = 0.7f;
static final float SYNTH_FILTER_REL_TIME = 0.1f;
