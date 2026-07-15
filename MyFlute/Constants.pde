// =======================================================
// オリジナル・フルート音色定数設定 (1次〜6次倍音)
// =======================================================

// 提示された正確な倍音目安
static final float[] FLUTE_HARMONICS = {
  1.000f, // 1次 (f)
  0.400f, // 2次 (2f)
  0.150f, // 3次 (3f)
  0.070f, // 4次 (4f)
  0.030f, // 5次 (5f)
  0.015f  // 6次 (6f)
};

// グラフ（Realの青線）を参考にしたADSR最適化
static final float FLUTE_ATK = 0.15f; 
static final float FLUTE_DEC = 0.30f; 
static final float FLUTE_SUS = 0.90f; 
static final float FLUTE_REL = 0.25f; 

// ビブラート設定
static final float VIBRATO_FREQ = 5.0f;
static final float VIBRATO_AMP  = 3.0f;
