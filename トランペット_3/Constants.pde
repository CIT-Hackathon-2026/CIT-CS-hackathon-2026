// --- システム定数 ---
static final int   BUFFER_SIZE = 512;
static final float DEFAULT_AMP = 0.5f;
static final int   SERIAL_BAUD = 9600;

// --- TrumpetVoice用定数 ---
// 倍音構成
static final float[] TRUMPET_HARMONICS = {
  0.421f, 0.726f, 1.000f, 0.567f, 0.333f, 0.200f, 0.123f, 0.084f, 0.0335f,
  0.021f, 0.008f, 0.004f, 0.002f, 0.001f, 0.001f, 0.004f, 0.003f, 0.006f, 0.000f, 0.000f,
  0.000f, 0.000f, 0.000f, 0.0002f, 0.0003f, 0.0003f, 0.0003f, 0.0002f, 0.0002f, 0.0002f, 0.0002f,
  0.0002f, 0.0002f
};

// ビブラート設定（振幅0なので実質オフ）
static final float VIBRATO_FREQ = 5.0f;
static final float VIBRATO_AMP  = 0.0f;

// ADSRエンベロープ設定
static final float TRUMPET_ATK_TIME = 0.50f;
static final float TRUMPET_DEC_TIME = 1.20f;
static final float TRUMPET_SUS_LVL  = 0.80f;
static final float TRUMPET_REL_TIME = 0.50f;
