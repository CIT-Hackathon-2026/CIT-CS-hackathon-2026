// --- システム定数 ---
static final int   BUFFER_SIZE = 512;
static final float DEFAULT_AMP = 0.5f;
static final int   SERIAL_BAUD = 9600;

// --- TrumpetVoice用定数 ---
// 倍音構成（実音源から検出られた33個の精密なデータに変更）
static final float[] TRUMPET_HARMONICS = { 
  0.406f, 0.805f, 1.000f, 0.818f, 0.636f, 0.510f, 0.126f, 0.089f, 0.019f,
  0.018f, 0.015f, 0.015f, 0.013f, 0.009f, 0.007f, 0.006f, 0.003f, 0.003f, 
  0.002f, 0.001f, 0.001f, 0.000f, 0.000f, 0.000f, 0.001f, 0.001f, 0.001f, 
  0.000f, 0.000f, 0.000f, 0.000f, 0.000f, 0.000f 
};

// ビブラート設定
static final float VIBRATO_FREQ = 5.0f;
static final float VIBRATO_AMP  = 0.0f;

// ADSRエンベロープ設定
static final float TRUMPET_ATK_TIME = 0.08f;
static final float TRUMPET_DEC_TIME = 0.15f;
static final float TRUMPET_SUS_LVL  = 0.8f;
static final float TRUMPET_REL_TIME = 0.15f;
