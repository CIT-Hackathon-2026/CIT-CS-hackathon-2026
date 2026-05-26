// --- システム定数 ---
static final int   BUFFER_SIZE = 512;
static final float DEFAULT_AMP = 0.5f;
static final int   SERIAL_BAUD = 9600;

// --- TrumpetVoice用定数 ---
// 倍音構成
static final float[] TRUMPET_HARMONICS = { 0.7f, 1.0f, 0.9f, 0.7f, 0.5f, 0.3f, 0.2f, 0.15f, 0.1f, 0.05f };

// ビブラート設定
static final float VIBRATO_FREQ = 5.0f;
static final float VIBRATO_AMP  = 3.0f;

// ADSRエンベロープ設定
static final float TRUMPET_ATK_TIME = 0.08f;
static final float TRUMPET_DEC_TIME = 0.15f;
static final float TRUMPET_SUS_LVL  = 0.8f;
static final float TRUMPET_REL_TIME = 0.15f;
