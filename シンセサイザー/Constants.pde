// --- システム定数 ---
static final int   BUFFER_SIZE = 512;
static final float DEFAULT_AMP = 0.5f;
static final int   SERIAL_BAUD = 9600;

// --- SynthVoice用定数 ---
// 倍音構成パラメータ
static final float[] SYNTH_HARMONICS = { 1.0f, 0.0f, 0.333f, 0.0f, 0.2f, 0.0f, 0.143f, 0.0f, 0.111f, 0.0f };

// オシレーター・フィルター設定
static final float SYNTH_OSC_AMP     = 0.35f;
static final float SYNTH_DETUNE_RATE = 1.008f;
static final float SYNTH_FILTER_MULT = 3.0f;
static final float SYNTH_FILTER_RES  = 0.5f;

// トレモロ（LFO）設定
static final float TREMOLO_FREQ = 8.0f;
static final float TREMOLO_AMP  = 0.15f;

// ADSRエンベロープ設定
static final float SYNTH_ATK_TIME = 0.01f;
static final float SYNTH_DEC_TIME = 0.1f;
static final float SYNTH_SUS_LVL  = 0.8f;
static final float SYNTH_REL_TIME = 0.3f;
