// ============================================================
// sync_control.ino
// パルス送信タイミングの記録・テンポ算出を担当するモジュール
// ※ LED点灯・消灯の制御は同期担当者のプログラムで行う
//    sendPulse()をLED点灯のタイミングで呼び出すこと
// ============================================================

static const int PULSES_PER_BEAT = 24;  // 1拍あたりのパルス数
static const int UPDATE_INTERVAL = 12;  // テンポ再計算を行うパルス数

// ---------- メンバ変数 ----------
static int           pulseCount      = 0;      // 現在の区間内パルスカウント
static long          totalPulseCount = 0;      // 累積総パルス数
static unsigned long lastPulseTime   = 0;      // 前回パルス送信時刻 [us]
static unsigned long interval        = 0;      // 直前のパルス間隔 [us]
static float         avgInterval     = 0.0f;   // 移動平均パルス間隔 [us]
static float         tempo           = 120.0f; // 算出テンポ [BPM]

// ---------- 公開関数 ----------

// パルス送信タイミングの記録
void sendPulse() {
    unsigned long now = micros();
    if (lastPulseTime != 0) {
        interval = now - lastPulseTime;
    }
    lastPulseTime = now;
}

// パルス間隔の移動平均を更新する
void measureInterval() {
    if (interval == 0) return;

    if (avgInterval == 0.0f) {
        avgInterval = (float)interval;
    } else {
        // 移動平均（平滑化係数 0.2）
        avgInterval = avgInterval * 0.8f + (float)interval * 0.2f;
    }
}

// 平均パルス間隔からテンポ[BPM]を算出する
// 1拍 = PULSES_PER_BEAT パルス
// 1拍の時間[us] = avgInterval * PULSES_PER_BEAT
// BPM = 60,000,000 / 1拍の時間[us]
float calculateTempo() {
    if (avgInterval <= 0.0f) return tempo;  // 未計測時は前回値を返す

    float beatInterval_us = avgInterval * (float)PULSES_PER_BEAT;
    tempo = 60000000.0f / beatInterval_us;

    // 範囲クランプ（テンポ40〜200BPM）
    if (tempo < 40.0f)  tempo = 40.0f;
    if (tempo > 200.0f) tempo = 200.0f;

    return tempo;
}

// pulseCountを0にリセットする
void resetPulseCount() {
    pulseCount = 0;
}

// パルスカウントをインクリメントし，テンポ更新タイミングか判定する
// 戻り値: true = UPDATE_INTERVAL(12)パルスに達したので更新すべき
bool updateTiming() {
    pulseCount++;
    totalPulseCount++;

    if (pulseCount >= UPDATE_INTERVAL) {
        resetPulseCount();
        return true;
    }
    return false;
}

// 累積総パルス数を返す（main.inoから終了判定に使用）
long getTotalPulseCount() {
    return totalPulseCount;
}

// 現在のテンポを返す
float getTempo() {
    return tempo;
}
